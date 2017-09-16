#ifndef MOTORCTRL_H
#define MOTORCTRL_H

#include <string.h>
#include <PubSubClient.h>

#include <SoftwareSerial.h>
#include <arduino.h>

#include "types.h"

#define MAX_RECV_LEN 20

#define CMD_CTRL_GROUP 		0x0a
#define CMD_TYPE_UP    		0Xdd
#define CMD_TYPE_DOWN  		0Xee
#define CMD_TYPE_STOP  		0xcc

#define MIN_SPEED 50
#define MAX_SPEED 150

#define LED 16
//use TXD2 RXD2
#define SOFT_TX 15
#define SOFT_RX 13

#define PACK_DATA_HEADER(ucBuf,ucId,usChannal,ucCmdGroup,ucCmdType,ucCheck) do{\
                        ucBuf[0] = 0x9a; \
                        ucBuf[1] = ucId; \
                        ucBuf[2] = usChannal & 0xff; \
                        ucBuf[3] = (usChannal >> 8 ) & 0xff; \
                        ucBuf[4] = ucCmdGroup; \
                        ucBuf[5] = ucCmdType; \
                        ucBuf[6] = ucCheck; \
                        }while(0)
typedef struct
{
    UINT8  ucHead;
    UINT8  ucMotorId;
    UINT16 usMotorChanal;
    UINT8  ucCurrent;  //0.01ma
    UINT8  ucVoltage;  //V
    UINT8  ucSpeed;   //RPM/min
    UINT8  ucPos;
    UINT8  bitRun:1;
    UINT8  bitSetRoute:1;
    UINT8  bitLimitPos1:1;
    UINT8  bitLimitPos2:1;
    UINT8  bitLimitPos3:1;
    UINT8  bitLimitPos4:1;
    UINT8  bitRsvd:2;
}MOTOR_INFO;

class MotorCtrl
{
public:
    explicit MotorCtrl();
    ~MotorCtrl();
    int setSpeed(UINT8 ucSpeed);
    int setRunPos(UINT8 ucPos);
    int setMotorInfo(UINT8 ucSetId,UINT8 ucSetChanal);
    int getMotorInfo(MOTOR_INFO *pstInfo);
    int turnUp(void);
    int turnDown(void);
    int stopRun(void);
private:
    int execCmd(UINT8 *pucBuf,UINT8 ucLen);
    int readData(void);
private:
   MOTOR_INFO _stMotorInfo;
   SoftwareSerial *_mySerial;

   UINT8 _ucGetDataLen;
   UINT8 _ucGetData[MAX_RECV_LEN];
};

#endif // MOTORCTRL_H
