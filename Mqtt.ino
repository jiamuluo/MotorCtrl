#include <PubSubClient.h>
#include <string.h>
#include <arduino.h>
#include "motorctrlmqtt.h"

WiFiClient espClient;
MotorCtrlMqtt *gpstMotorCtrlMqtt = NULL;
String glReadData;

char* ssid = "peter598";
char* password = "hylyp130311hjl";
char* mqtt_server = "192.168.1.104";

#define LED 16

static void ledToggle(void)
{
	static UINT8 ucLedStatus = HIGH;
	ucLedStatus = ucLedStatus ^ HIGH;
	digitalWrite(LED, ucLedStatus);
	delay(1000);
}

static void getData(String &getData)
{
	int slRetry = 500;
	while(slRetry > 0)
	{
		if(Serial.available() > 0)
		{
			glReadData = Serial.readString();

			if(glReadData.length() <= 0)
			{
				delay(500);
				continue;
			}
			else
			{
				return;
			}
		}
		else
		{
			delay(500);
			continue;
		}
	}
}
static void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

char debugBuf[100] = {0};
void setup() {
  // put your setup code here, to run once:
	gpstMotorCtrlMqtt = new MotorCtrlMqtt(espClient,mqttCallback);
   	Serial.begin(9600); 
	while (!Serial) 
	{
	    ; // wait for serial port to connect. Needed for native USB
	}
    pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);
	Serial.write("start init wifiInfo\n");
	//info must config by serial
	gpstMotorCtrlMqtt->setWifiInfo(ssid,password);
	gpstMotorCtrlMqtt->setServer(mqtt_server,MQTT_PORT);
	gpstMotorCtrlMqtt->setLogInfo("jiamu","jiamu","abcd");
	Serial.write("start  wifi\n");
	if(gpstMotorCtrlMqtt->initWifi() < 0)
	{
		Serial.write("init wifi or connect to server failed \n");
	}
	Serial.write("set up ok\n");
	
}

void loop() {
  // put your main code here, to run repeatedly:
	ledToggle();
  	gpstMotorCtrlMqtt->loop();
	memset(debugBuf,0,100);
	gpstMotorCtrlMqtt->getWifiStatus(debugBuf );
	Serial.write(debugBuf);

	memset(debugBuf,0,100);
	gpstMotorCtrlMqtt->getServerStatus(debugBuf );
	Serial.write(debugBuf);
	
	gpstMotorCtrlMqtt->publish("motorCtrlPut", "This is motor agent\n");
	gpstMotorCtrlMqtt->subscribe("motorCtrlGet");
}
