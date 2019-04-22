#pragma once
#ifndef _SOFT_MQTT_H_
#define _SOFT_MQTT_H_

#include "MQTTClient.h"
#include "MyData.h"

class MyMqtt {
public:
	MyMqtt();
	~MyMqtt();
	int mqttconnect(char* url,char* clientid,char* username,char* password);
	int mqttconnect(ConnectInfo_t mqttparam);
private:
	MQTTClient client;
	static void delivered(void* context, MQTTClient_deliveryToken dt);
	static int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message);
	static void connlost(void* context, char* cause);
};


#endif // !_SOFT_MQTT_H_
