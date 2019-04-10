#pragma once
#ifndef _SOFT_MQTT_H_
#define _SOFT_MQTT_H_

class MyMqtt
{
public:
	MyMqtt();
	~MyMqtt();
	int connect(char* ipaddress,char* clientid,char* username,char* password);
private:

};

#endif // !_SOFT_MQTT_H_
