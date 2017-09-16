#include "motorctrl.h"

MotorCtrl *_pstMotorCtrl;
String glReadData;
int    slLedStatus = HIGH;

void setup() {
  // put your setup code here, to run once:

	_pstMotorCtrl = new MotorCtrl;

  	Serial.begin(9600); 
	while (!Serial) 
	{
	    ; // wait for serial port to connect. Needed for native USB
	}
    pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);
}
char scDebugBuf[200];
void loop() {
  // put your main code here, to run repeatedly:

	if(Serial.available() > 0)
	{
		glReadData = Serial.readString();

		if(glReadData.length() <= 0)
		{
			delay(100);
		}
		else
		{
			digitalWrite(LED,HIGH);
			delay(1000);
			digitalWrite(LED,LOW);
			delay(1000);
			digitalWrite(LED,HIGH);
			delay(1000);
			digitalWrite(LED,LOW);
			delay(1000);
			
			Serial.write("Get cmd \n");
			Serial.write(glReadData.c_str());
			Serial.write("\n");
			
			if(glReadData == "up")
			{
				_pstMotorCtrl->turnUp();
			}
			else if(glReadData == "down")
			{
				_pstMotorCtrl->turnDown();
			}
			else if(glReadData == "stop")
			{
				_pstMotorCtrl->stopRun();
			}
			else if(glReadData == "60" || glReadData == "100")
			{
				
				int slSpeed = 80;
				_pstMotorCtrl->setSpeed(slSpeed);
			}
			else if(glReadData == "info")
			{
				MOTOR_INFO stMotorInfo;
				memset(&stMotorInfo,0,sizeof(stMotorInfo));
				
				if(_pstMotorCtrl->getMotorInfo(&stMotorInfo) < 0)
				{
					Serial.write("Get Info failed\n");
				}

				char scRun[5] = {0};
				if(stMotorInfo.bitRun == 1)
				{
					sprintf(scRun,"Run");
				}
				else
				{
					sprintf(scRun,"Stop");
				}
				memset(scDebugBuf,0,200);
				sprintf(scDebugBuf,"\nNow show all motor infomation: \n"
								   "Id:		 %d \n"
								   "Channal: %d \n"
								   "Current: %d mA \n"
								   "Voltage: %d V \n"
								   "Speed:   %d RPM/min \n"
								   "Pos: 	 %d  \n"
								   "Run:     %s  \n"
								   "Setroute:%d  \n"
								   "Limit1:	 %d  \n"
								   "Limit2:  %d  \n"
								   "Limit3:  %d  \n"
								   "Limit4:  %d  \n",stMotorInfo.ucMotorId,stMotorInfo.usMotorChanal,
								   					 stMotorInfo.ucCurrent,stMotorInfo.ucVoltage,
								   					 stMotorInfo.ucSpeed,stMotorInfo.ucPos,
								   					 scRun,stMotorInfo.bitSetRoute,
								   					 stMotorInfo.bitLimitPos1,stMotorInfo.bitLimitPos2,
								   					 stMotorInfo.bitLimitPos3,stMotorInfo.bitLimitPos4);
				Serial.write(scDebugBuf);
			}
			else if(glReadData == "route")
			{
				_pstMotorCtrl->setRunPos(80);
			}
			else
			{
				Serial.write("Get cmd wrong\n");
			}
			
		}
	}

	
}
