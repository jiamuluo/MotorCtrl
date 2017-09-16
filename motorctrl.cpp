#include "motorctrl.h"

MotorCtrl::MotorCtrl()
{
    memset(&_stMotorInfo,0,sizeof(_stMotorInfo));

	pinMode(SOFT_RX, INPUT);
    pinMode(SOFT_TX, OUTPUT);
    _mySerial = new SoftwareSerial(SOFT_RX,SOFT_TX);
	_mySerial->begin(2400);
	
    getMotorInfo(NULL);
	
}

MotorCtrl::~MotorCtrl()
{
    delete _mySerial;
}

int MotorCtrl::setRunPos(UINT8 ucPos)
{
    UINT16 usChannalTmp = this->_stMotorInfo.usMotorChanal;
    UINT8 ucId = this->_stMotorInfo.ucMotorId;
    UINT8 ucQueryCmd[10] = {0};
    UINT8 ucHigh = (usChannalTmp >> 8 ) & 0xff;
    UINT8 ucLow  = (usChannalTmp) & 0xff;
    UINT8 ucCheckVal = 0;

    ucCheckVal = ucId ^ ucHigh ^ ucLow ^ 0xdd ^ ucPos;

    PACK_DATA_HEADER(ucQueryCmd,ucId,usChannalTmp,0xdd,ucPos,ucCheckVal);

    execCmd(ucQueryCmd,7);

    return 0;
}

int MotorCtrl::getMotorInfo(MOTOR_INFO *pstInfo)
{
    UINT8 ucQueryCmd[7] = {0x9a,0x00,0x00,0x00,0xcc,0xcc};

    execCmd(ucQueryCmd,ARRAY_SIZE(ucQueryCmd));

    if(readData() < 0)
    {
        return ERROR_NODATA;
    }

    int i = 0;
    for(i = 0;i < _ucGetDataLen;i++)
    {
        if(0xd8 == _ucGetData[i])
        {
            break;
        }
    }

    if(i == _ucGetDataLen || ((_ucGetDataLen - i) < 9))
    {
        return ERROR_WRONGDATA;
    }


    UINT8 *pucRealData = &_ucGetData[i];
    UINT8 ucCheckVal = 0;
    for(i = 1;i < 9;i++)
    {
        ucCheckVal = ucCheckVal ^ pucRealData[i];
    }

    if(ucCheckVal != pucRealData[9])
    {
        return ERROR_WRONGDATA;
    }

    memset(&_stMotorInfo,0,sizeof(_stMotorInfo));
    memcpy(&_stMotorInfo,pucRealData,sizeof(_stMotorInfo));

	if(pstInfo != NULL)
	{
		memcpy(pstInfo,&_stMotorInfo,sizeof(_stMotorInfo));
	}
    return ERROE_NO;
}
int MotorCtrl::turnUp()
{
   UINT8 ucMotorId       = this->_stMotorInfo.ucMotorId;
   UINT16 usMotorChananl = this->_stMotorInfo.usMotorChanal;

   UINT8 ucCheckVal = 0;
   UINT8 ucHigh = (usMotorChananl >> 8 ) & 0xff;
   UINT8 ucLow  = (usMotorChananl & 0xff);

   UINT8 ucCmdBuf[7] = {0};
   ucCheckVal = ucMotorId ^ ucHigh ^ ucLow ^ CMD_CTRL_GROUP ^ CMD_TYPE_UP;

   PACK_DATA_HEADER(ucCmdBuf,ucMotorId,usMotorChananl,CMD_CTRL_GROUP,CMD_TYPE_UP,ucCheckVal);

   execCmd(ucCmdBuf,7);
}

int MotorCtrl::turnDown()
{
    UINT8 ucMotorId       = this->_stMotorInfo.ucMotorId;
    UINT16 usMotorChananl = this->_stMotorInfo.usMotorChanal;

    UINT8 ucCheckVal = 0;
    UINT8 ucHigh = (usMotorChananl >> 8 ) & 0xff;
    UINT8 ucLow  = (usMotorChananl & 0xff);

    UINT8 ucCmdBuf[7] = {0};
    ucCheckVal = ucMotorId ^ ucHigh ^ ucLow ^ CMD_CTRL_GROUP ^ CMD_TYPE_DOWN;

    PACK_DATA_HEADER(ucCmdBuf,ucMotorId,usMotorChananl,CMD_CTRL_GROUP,CMD_TYPE_DOWN,ucCheckVal);

    execCmd(ucCmdBuf,7);
}

int MotorCtrl::stopRun()
{
    UINT8 ucMotorId       = this->_stMotorInfo.ucMotorId;
    UINT16 usMotorChananl = this->_stMotorInfo.usMotorChanal;

    UINT8 ucCheckVal = 0;
    UINT8 ucHigh = (usMotorChananl >> 8 ) & 0xff;
    UINT8 ucLow  = (usMotorChananl & 0xff);

    UINT8 ucCmdBuf[7] = {0};
    ucCheckVal = ucMotorId ^ ucHigh ^ ucLow ^ CMD_CTRL_GROUP ^ CMD_TYPE_STOP;

    PACK_DATA_HEADER(ucCmdBuf,ucMotorId,usMotorChananl,CMD_CTRL_GROUP,CMD_TYPE_STOP,ucCheckVal);

    execCmd(ucCmdBuf,7);
}

int MotorCtrl::execCmd(UINT8 *pucBuf, UINT8 ucLen)
{
    int i = 0;

    for(i = 0;i < ucLen;i++)
    {
        _mySerial->write(pucBuf[i]);
    }
    return 0;
}

int MotorCtrl::readData(void)
{
    int slRetry = 300;

    _ucGetDataLen = 0;
    memset(_ucGetData,0,MAX_RECV_LEN);
    while(slRetry > 0)
    {
        slRetry--;
         while(_mySerial->available() > 0)
         {
            _ucGetData[_ucGetDataLen++] = _mySerial->read();
         }
         if(_ucGetDataLen > MAX_RECV_LEN)
         {
             break;
         }
         delay(10);
    }

    if(_ucGetDataLen > 0)
    {
        return ERROE_NO;
    }
    else
    {
        return ERROR_NODATA;
    }
}

int MotorCtrl::setSpeed(UINT8 ucSpeed)
{
    if(ucSpeed < MIN_SPEED || ucSpeed > MAX_SPEED)
    {
        return ERROR_INPUTINVALID;
    }

    UINT8 ucSetSpeed[7]= {0x9a,0x00,0x00,0x00,0xd9,ucSpeed};
    ucSetSpeed[6] = ucSetSpeed[1] ^ ucSetSpeed[2] ^ ucSetSpeed[3] ^ ucSetSpeed[4] ^ ucSetSpeed[5];
    execCmd(ucSetSpeed,7);

    return ERROE_NO;
}

int MotorCtrl::setMotorInfo(UINT8 ucSetId, UINT8 ucSetChanal)
{
    if(ucSetId > 99 || ucSetChanal > 16)
    {
        return ERROR_INPUTINVALID;
    }

    unsigned short usSetChanal = 0;
    UINT8 ucCmdBuf[10] = {0x9a,ucSetId,0x00,0x00,0xaa,0xaa};
    UINT8 ucCheckVal = 0;

    usSetChanal = usSetChanal | (1 << (ucSetChanal -1));
    memcpy(&ucCmdBuf[2],&usSetChanal,2);

    ucCheckVal  = ucCmdBuf[1] ^ ucCmdBuf[2] ^ ucCmdBuf[3] ^ ucCmdBuf[4] ^ ucCmdBuf[5];
    ucCmdBuf[6] = ucCheckVal;

    execCmd(ucCmdBuf,7);

    if(getMotorInfo(NULL) < 0)
    {
        return ERROR_NODATA;
    }
    if( !((_stMotorInfo.usMotorChanal >> (ucSetChanal -1) ) & 1) || _stMotorInfo.ucMotorId != ucSetId)
    {
         return ERROR_WRONGDATA;
    }
    return ERROE_NO;
}
