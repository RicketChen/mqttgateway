#include "soft_mqtt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "tcp://192.168.41.128:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;
using namespace mqtt
MyMqtt::MyMqtt()
{
	
}
MyMqtt::~MyMqtt()
{

}

void MyMqtt::delivered(void* context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}
int MyMqtt::msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message)
{
	int i;
	char* payloadptr;
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");
	payloadptr = (char*)message->payload;
	for (i = 0; i < message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}
void MyMqtt::connlost(void* context, char* cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}
int MyMqtt::mqttconnect(char* url, char* clientid, char* username, char* password)
{
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;
	MQTTClient_create(&client, url, clientid,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 60;
	conn_opts.cleansession = 1;
	conn_opts.username = username;
	conn_opts.password = password;
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
		"Press Q<Enter> to quit\n\n", TOPIC, clientid, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);
/*	do
	{
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);*/
	return rc;
}
int MyMqtt::mqttconnect(ConnectInfo_t mqttparam)
{

}