#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <chrono>
#include <unistd.h>
#include <thread>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#include "openssl/hmac.h"

#include "modbus/modbus-rtu.h"

#include "libserial/Serial.h"

#include "sqlite3pp/sqlite3pp.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/rapidjson.h"

#include "soft_sqlite3pp.h"

#include "MyData.h"

//#include "soft_mqtt.h"
#include "soft_aliyunmqtt.h"

#include "soft_mymodbus.h"
using std::cout;
using std::endl;
using std::string;

ConnectInfo_t MqttInfo[4];
DeviceInfo_t DevInfo[10];
PortInfo_t PortInfo[10];
ThemeCtrl_t ThemeCtrl[10];
ThemeUpload_t ThemeUpload[10];
ThemeUploadList_t ThemeUploadList[100];
VarParam_t VarParam[100];

int main(int argc, char* argv[])
{

	MySqlite db("test.conf");
	db.GetAllInfo();

	MyAliyunMqtt Mqtt;
	Mqtt.openthread();

/**********************************≤‚ ‘rapidjson************************************/
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writer.StartObject();
	writer.Key("test");
	writer.String("json");
	writer.EndObject();
	cout << buffer.GetString();
/**********************************≤‚ ‘rapidjson************************************/

/***********************************≤‚ ‘¥Æø⁄**************************************/
/*	Serial s;
	if (s.open("/dev/ttyS0", 115200, 8,'N', 1) != Serial::OK)
	{
		printf("Cannot open serial port!\n");
		return -1;
	}
	char test[] = "test data";
	s.write(test, strlen(test));
	s.close();*/
/***********************************≤‚ ‘¥Æø⁄**************************************/

/***********************************√‹‘øº∆À„**************************************/

	// The data that we're going to hash using HMAC
	char data[100] = {0};
	unsigned char digest[EVP_MAX_MD_SIZE] = { '\0' };
	unsigned int digest_len = 0;
//	sprintf(data, "clientId%sdeviceName%sproductKey%s", connectinfo.clientid, connectinfo.devicename, connectinfo.productkey);
//	strcpy(data, connectinfo.mqttclientid);
	// Using sha1 hash engine here.
	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
//	HMAC(EVP_sha1(), connectinfo.devicesecret, strlen(connectinfo.devicesecret), (unsigned char*)data, strlen(data), digest, &digest_len);

	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine
	char mdString[41] = { '\0' };
	for (int i = 0; i < 20; i++)
		sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
//	strcpy(connectinfo.password, mdString);

/***********************************√‹‘øº∆À„**************************************/


/***********************************modbus≤‚ ‘**************************************/

/***********************************modbus≤‚ ‘**************************************/
	while (1)
	{
		cout << "i am main thread" << endl;
		sleep(2);
	}
	return 0;
}