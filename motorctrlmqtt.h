#ifndef MOTORCTRLMQTT_H
#define MOTORCTRLMQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <arduino.h>
#include "types.h"

#define MQTT_PORT 1883

typedef  void (*Callback)(char* topic, UINT8* payload, unsigned int length);

class MotorCtrlMqtt : public PubSubClient
{
public:
     explicit MotorCtrlMqtt(WiFiClient &_espClient,Callback mqttCallback);
     ~MotorCtrlMqtt();

public:
    int setWifiInfo(char *pucssid,char *pucKey);
    int setLogInfo(char *pucUser,char *pucKey,char *pucLogId);
    int setServerInfo(char *pucServerIp,unsigned short usServerPort = 1883);

    int getWifiInfo(String &getSSID,String &getKey);
    int getLogInfo(String &getLogUsr,String &getLogKey,String &getLogId);
    int getServerInfo(String &getServerIp,unsigned short &getPort);

    int getWifiStatus(String &status);
    int getWifiStatus(char *pucStatus);

    int getServerStatus(String &status);
    int getServerStatus(char *pucStatus);

	int initWifi(void);
	int initWifi(char *pucSSID,char *pucKey);
	int disconnectWifi(bool val = true);
	int connectToServer(void);
private:
   // void mqttCallback(char* topic, UINT8* payload, unsigned int length);
    int  setUpWifi(void);
    int  reConnectToserver(void);
private:
    String _wifiSSID;
    String _wifiKey;

    String _ServerIpAddr;
    unsigned short _ServerPort = 1883;

    bool _wifiInit   = false;
	bool _serverInit = false;
	
    String _logInId;
    String _logInUsr;
    String _logInKey;
};

#endif // MOTORCTRLMQTT_H
