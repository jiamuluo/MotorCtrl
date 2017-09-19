#include "motorctrlmqtt.h"

MotorCtrlMqtt::MotorCtrlMqtt(WiFiClient &_espClient,Callback mqttCallback):
    PubSubClient(_espClient)
{
	 this->setCallback(mqttCallback);
}

MotorCtrlMqtt::~MotorCtrlMqtt()
{

}

int MotorCtrlMqtt::setWifiInfo(char *pucssid, char *pucKey)
{
    _wifiSSID = pucssid;
    _wifiKey  = pucKey;
    return ERROE_NO;
}

int MotorCtrlMqtt::setLogInfo(char *pucUser, char *pucKey, char *pucLogId)
{
    _logInUsr = pucUser;
    _logInKey = pucKey;
    _logInId  = pucLogId;
    return ERROE_NO;
}

int MotorCtrlMqtt::setServerInfo(char *pucServerIp, unsigned short usServerPort)
{
    _ServerIpAddr = pucServerIp;
    _ServerPort   = usServerPort;

    if(_wifiInit != true)
    {
         if(setUpWifi() < 0)
         {
             return ERROR_REQUSETFAILED;
         }

    }

    this->disconnect();
    this->setServer(_ServerIpAddr.c_str(),_ServerPort);
   

    return ERROE_NO;
}

int MotorCtrlMqtt::getWifiInfo(String &getSSID, String &getKey)
{
    getSSID = _wifiSSID;
    getKey  = _wifiKey;
    return ERROE_NO;
}

int MotorCtrlMqtt::getLogInfo(String &getLogUsr, String &getLogKey, String &getLogId)
{
    getLogUsr = _logInUsr;
    getLogKey = _logInKey;
    getLogId  = _logInId;
    return ERROE_NO;
}

int MotorCtrlMqtt::getServerInfo(String &getServerIp, unsigned short &getPort)
{
    getServerIp = _ServerIpAddr;
    getPort     = _ServerPort;
    return ERROE_NO;
}

int MotorCtrlMqtt::getWifiStatus(String &status)
{
    String info("\nWifi info:\n");
	
	
    if(_wifiInit == true)
    {
    	info += "Mac:";
		info = info + WiFi.macAddress();
     
        info += "Ip:";
		String ip = WiFi.localIP().toString();
		info = info + ip;
    }
    else
    {
        info = info + String("Wifi is disconnect\n");
    }

    status = info;
    return ERROE_NO;
}

int MotorCtrlMqtt::getWifiStatus(char *pucStatus)
{
    String info("\nWifi info:\n");
    if(_wifiInit == true)
    {
        info = info + String(" Mac: ") + WiFi.macAddress();
        info = info + String(" Ip: ")  + WiFi.localIP().toString();
    }
    else
    {
        info = info + String("Wifi is disconnect\n");
    }
	const char *p = info.c_str();
    memcpy(pucStatus,p,info.length());
    return ERROE_NO;
}
int MotorCtrlMqtt::getServerStatus(String &status)
{
	status = status + String("\n Server info:\n");
	if(true == _serverInit)
	{
		status = status + String(" Server ip: ")+ _ServerIpAddr;
		status = status + String(" User: ") + _logInUsr;
		status = status + String(" Key: ") + _logInKey;
		
	}
	else
	{
		status = status + String("sever is disconnected\n");
	}
	return ERROE_NO;
}
int MotorCtrlMqtt::getServerStatus(char *pucStatus)
{
	String getString("");
	getServerStatus(getString);

	const char *p = getString.c_str();
    memcpy(pucStatus,p,getString.length());
	return ERROE_NO;
}

int MotorCtrlMqtt::initWifi(void)
{
	disconnectWifi();
	
	if(setUpWifi() < 0)
	{
		return ERROR_REQUSETFAILED;
	}
	else
	{
		//return 0;
		return connectToServer();
	}
	
}
int MotorCtrlMqtt::initWifi(char *pucSSID,char *pucKey)
{
	setWifiInfo(pucSSID,pucKey);
	
	disconnectWifi();
	return setUpWifi();
}
int  MotorCtrlMqtt::disconnectWifi(bool val)
{
	if(_wifiInit == true)
	{
		_wifiInit = false;
		this->disconnect();
		return WiFi.disconnect(val);
	}
	else
	{
		return 0;
	}
}

int MotorCtrlMqtt::connectToServer(void)
{
	connect(_logInId.c_str(),_logInUsr.c_str(),_logInKey.c_str());

	return reConnectToserver();
}

int MotorCtrlMqtt::setUpWifi()
{
     delay(10);
     WiFi.begin(_wifiSSID.c_str(), _wifiKey.c_str());

     int slRetry = 20;
     while (WiFi.status() != WL_CONNECTED && slRetry > 0)
     {
       delay(500);
       slRetry--;
     }

     if(slRetry < 0)
     {
        _wifiInit = false;
        return ERROR_REQUSETFAILED;
     }
     else
     {
         _wifiInit = true;
         return ERROE_NO;
     }
}

int  MotorCtrlMqtt::reConnectToserver(void)
{
	if(this->connected())
	{
		_serverInit = true;
		return ERROE_NO;
	}

    int reTry = 10;
  	while (reTry > 0)
  	{
  		connect(_logInId.c_str(),_logInUsr.c_str(),_logInKey.c_str());
		
	    if (this->connected())
		{
		  _serverInit = true;
	      publish("logIn", "I am mqtt");
		  return ERROE_NO;
	    }
		else 
		{
	      delay(500);
	    }
		reTry--;
  	}
  	return ERROR_TIMEOUT;
}

