#include "soft_aliyunmqtt.h"

#include <iostream>
#include <string>
#include <stdint.h>
#include <unistd.h>

#include "mqtt_api.h"
#include "dev_sign_api.h"
#include "MyData.h"
#include "soft_mymodbus.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using std::cout;
using std::endl;

static char g_topic_name[CONFIG_MQTT_TOPIC_MAXLEN];

void MyAliyunMqtt::message_arrive(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg)
{
	cout << "pcontext is " << (char*)pcontext << endl;
	iotx_mqtt_topic_info_t* topic_info = (iotx_mqtt_topic_info_pt)msg->msg;
	cout << "get message" << endl;
	switch (msg->event_type) {
	case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
		/* print topic name and topic message */
		printf("Message Arrived: \n");
		printf("Topic  : %.*s\n", topic_info->topic_len, topic_info->ptopic);
		printf("Payload: %.*s\n", topic_info->payload_len, topic_info->payload);
		printf("\n");
		break;
	default:
		break;
	}
}
int MyAliyunMqtt::subscribe(char* subscribetopic, int Qos)
{
	int res = 0;
	iotx_mqtt_qos_t iotx_qos;
	if (Qos == 0)
		iotx_qos = IOTX_MQTT_QOS0;
	else if (Qos == 1)
		iotx_qos = IOTX_MQTT_QOS1;
	else if (Qos == 2)
		iotx_qos = IOTX_MQTT_QOS2;
	else
	{
		printf("Unsupport Qos Setting!\n");
		return -1;
	}

	res = IOT_MQTT_Subscribe_Sync(pclient, subscribetopic, iotx_qos, message_arrive, (void*)"args",1000);
	if (res < 0) {
		printf("subscribe failed\n");
		return -1;
	}
	else
	{
		printf("topic is %s\n", subscribetopic);
	}

	return 0;
}/*
int subscribe(char* subscribetopic, int Qos)
{
	int res = 0;
	char product_key[IOTX_PRODUCT_KEY_LEN] = { 0 };
	char device_name[IOTX_DEVICE_NAME_LEN] = { 0 };
	const char* fmt = "/%s/%s/user/get";
	char* topic = NULL;
	int topic_len = 0;

	topic_len = strlen(fmt) + strlen(product_key) + strlen(device_name) + 1;
	if (topic_len > CONFIG_MQTT_TOPIC_MAXLEN) {
		printf("topic too long\n");
		return -1;
	}
	topic = g_topic_name;
	memset(topic, 0, CONFIG_MQTT_TOPIC_MAXLEN);
	printf(topic, topic_len, fmt, product_key, device_name);

	res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
	if (res < 0) {
		printf("subscribe failed\n");
		return -1;
	}

	return 0;
}*/

int MyAliyunMqtt::publish(char* publishtopic, int Qos, char* payload)
{
	int res = 0;
	iotx_mqtt_topic_info_t topic_msg;

	topic_msg.qos = Qos;
	topic_msg.retain = 0;
	topic_msg.dup = 0;
	topic_msg.payload = (char*)payload;
	topic_msg.payload_len = strlen(payload);

	res = IOT_MQTT_Publish(pclient, publishtopic, &topic_msg);
	if (res < 0) {
		printf("publish failed\n");
		return -1;
	}

	return 0;
}
void MyAliyunMqtt::event_handle(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg)
{
	printf("msg->event_type : %d", msg->event_type);
}

/*
 *  NOTE: About demo topic of /${productKey}/${deviceName}/get
 *
 *  The demo device has been configured in IoT console (https://iot.console.aliyun.com)
 *  so that its /${productKey}/${deviceName}/get can both be subscribed and published
 *
 *  We design this to completely demonstrate publish & subscribe process, in this way
 *  MQTT client can receive original packet sent by itself
 *
 *  For new devices created by yourself, pub/sub privilege also requires being granted
 *  to its /${productKey}/${deviceName}/get for successfully running whole example
 */
MyAliyunMqtt::~MyAliyunMqtt()
{

}
int MyAliyunMqtt::MqttInit(char* host,int port,char* clientid,char* username,char* password)
{
	int                     res = 0;
	iotx_mqtt_param_t       mqtt_params;

	memset(&mqtt_params, 0x0, sizeof(mqtt_params));

	mqtt_params.write_buf_size = 1024;
	mqtt_params.read_buf_size = 1024;

	mqtt_params.host = host;
	mqtt_params.client_id = clientid;
	mqtt_params.password = password;
	mqtt_params.username = username;
	mqtt_params.keepalive_interval_ms = 60000;
	mqtt_params.request_timeout_ms = 20000;
	mqtt_params.port = port;
	mqtt_params.clean_session = 1;

	mqtt_params.handle_event.h_fp = event_handle;
	mqtt_params.handle_event.pcontext = NULL;

	pclient = IOT_MQTT_Construct(&mqtt_params);
	if (NULL == pclient) {
		printf("MQTT construct failed");
		return -1;
	}
	return 0;
}
MyAliyunMqtt::MyAliyunMqtt()
{
}
int MyAliyunMqtt::openthread()
{
	threadid = std::thread(MqttMain, this);
}
int MyAliyunMqtt::MqttMain(void* Params)
{
	MyAliyunMqtt *point = (MyAliyunMqtt*)Params;

	int loop_cnt = 0;

	point->MqttInit(MqttInfo[0].ServerLink, MqttInfo[0].ServerPort, MqttInfo[0].ClientId, MqttInfo[0].UserName, MqttInfo[0].Password);
	int res = point->subscribe(ThemeCtrl[0].CtrlSub, 1);
	if (res < 0) {
		IOT_MQTT_Destroy(&(point->pclient));
		return -1;
	}
//	uint16_t bufftest[10];
//	modbusread("c1d2test0", bufftest);
//	printf("%d\n", modbusread(ThemeUploadList[5].VarName));
	while (1)
	{
		rapidjson::StringBuffer stringbuff;
		rapidjson::Writer<rapidjson::StringBuffer> write(stringbuff);
		char payload[2048];
		time_t nowtime;
		nowtime = time(NULL); //获取日历时间   
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&nowtime));

		write.StartObject();
		
		write.Key("id");
		write.String(tmp);
		write.Key("method");
		write.String("method.event.property.post");
		write.Key("params");
		write.StartObject();

	//	write.Key(ThemeUploadList[0].VarName);
	//	write.Int(1);
	//	write.Int(*(int*)modbusread(ThemeUploadList[0].VarName));
		uint16_t temp[10];

		for (int i = 0; i < ThemeUploadList[0].UploadCount; i++)
		{
			write.Key(ThemeUploadList[i].VarName);
			modbusread(ThemeUploadList[i].VarName, temp);
			write.Int(int(temp[0]));
		}
		write.EndObject();
		write.EndObject();

		point->publish(ThemeUpload[0].CtrlPub , ThemeUpload[0].PubPeriod ,(char*)stringbuff.GetString());

		sleep(ThemeUpload[0].PubPeriod);

		IOT_MQTT_Yield((point->pclient), 200);

		loop_cnt += 1;
	}
}