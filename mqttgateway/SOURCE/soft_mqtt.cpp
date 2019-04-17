#include "soft_mqtt.h"

#include <iostream>
#include <string>
#include <stdint.h>

#include "mqtt_api.h"
#include "dev_sign_api.h"

using std::cout;
using std::endl;

static char g_topic_name[CONFIG_MQTT_TOPIC_MAXLEN];

char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = { "a1D8ZmAY7J6" };
char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = { "uRiD38Mfbp2mwjocOPrX" };
char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = { "naEe4i0gmyW7nXqYOBAxBSKKnc0PLNdB" };

void message_arrive(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg)
{
	iotx_mqtt_topic_info_t* topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

	switch (msg->event_type) {
	case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
		/* print topic name and topic message */
		printf("Message Arrived:");
		printf("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
		printf("Payload: %.*s", topic_info->payload_len, topic_info->payload);
		printf("\n");
		break;
	default:
		break;
	}
}

int subscribe(void* handle)
{
	int res = 0;
	const char* fmt = "/%s/%s/user/get";
	char* topic = NULL;
	int topic_len = 0;

	topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
	topic = (char*)malloc(topic_len);
	if (topic == NULL) {
		printf("memory not enough");
		return -1;
	}
	memset(topic, 0, topic_len);
	snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

	res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS1, message_arrive, NULL);
	if (res < 0) {
		printf("subscribe failed");
		free(topic);
		return -1;
	}

	free(topic);
	return 0;
}

int publish(void* handle)
{
	int             res = 0;
	const char* fmt = "/%s/%s/user/get";
	char* topic = NULL;
	int             topic_len = 0;
	char* payload = "{\"message\":\"hello!\"}";

	topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
	topic = (char*)malloc(topic_len);
	if (topic == NULL) {
		printf("memory not enough");
		return -1;
	}
	memset(topic, 0, topic_len);
	snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

	res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS1, payload, strlen(payload));
	if (res < 0) {
		printf("publish failed, res = %d", res);
		free(topic);
		return -1;
	}

	free(topic);
	return 0;
}

void event_handle(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg)
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

int MyMqtt::mqtt_connect(char* host,int port,char* clientid,char* username,char* password)
{
	void* pclient = NULL;
	int                     res = 0;
	int                     loop_cnt = 0;
	iotx_mqtt_param_t       mqtt_params;

	memset(&mqtt_params, 0x0, sizeof(mqtt_params));

	mqtt_params.write_buf_size = 1024;
	mqtt_params.read_buf_size = 1024;

/*	mqtt_params.host = "a1D8ZmAY7J6.iot-as-mqtt.cn-shanghai.aliyuncs.com";
	mqtt_params.client_id = "clientid|securemode=3,signmethod=hmacsha1|";
	mqtt_params.password = "833CFFC0315466986F5E4B3F051A22A5A1E98435";
	mqtt_params.username = "uRiD38Mfbp2mwjocOPrX&a1D8ZmAY7J6";*/

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
	res = subscribe(pclient);
	if (res < 0) {
		cout << "res < 0;" << endl;
		IOT_MQTT_Destroy(&pclient);
		return -1;
	}
	while (1) {
		if (0 == loop_cnt % 20) {
			publish(pclient);
		}

		IOT_MQTT_Yield(pclient, 200);

		loop_cnt += 1;
	}
}