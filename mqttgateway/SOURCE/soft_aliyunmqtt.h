#pragma once
#ifndef _SOFT_ALIYUNMQTT_H
#define _SOFT_ALIYUNMQTT_H_

#include "mqtt_api.h"
#include "thread"
class MyAliyunMqtt{
public:
	MyAliyunMqtt();
	int MqttInit(char* host, int port, char* clientid, char* username, char* password);

	~MyAliyunMqtt();

	/*
	¶©ÔÄº¯Êý
	handle£º¾ä±ú,pClient
	*/
	int subscribe(char* subscribetopic, int Qos);
	int publish(char* publishtopic, int Qos,char* payload);
	int openthread();
private:
	void* pclient;
	static void event_handle(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg);
	static void message_arrive(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg);

	std::thread threadid;
	static int MqttMain(void* Param);
};

#endif
