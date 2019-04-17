#pragma once
#ifndef _SOFT_MQTT_H
#define _SOFT_MQTT_H_

class MyMqtt{
public:
	int mqtt_connect(char* host,int port,char* clientid,char* username,char* password);
};

#endif
