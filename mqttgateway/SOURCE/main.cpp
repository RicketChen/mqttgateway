#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <chrono>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#include "openssl/hmac.h"
#include "modbus/modbus-rtu.h"
#include "libserial/Serial.h"
#include "sqlite3pp/sqlite3pp.h"
#include "soft_mqtt.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/rapidjson.h"

using std::cout;
using std::endl;
using std::string;
typedef struct _ConnectInfo_t {
	char location[100] = "cn-shanghai";

	char url[100] = "";
	char port[10] = "1883";
	char productkey[100] = "a1D8ZmAY7J6";
	char devicename[100] = "abcd";
	char devicesecret[100] = "fFbESw05SaEJ3Jo0p9I0wAzmxZ2uWaCv";
	char username[100] = "";
	char password[100] = "";
	char clientid[100] = "test";
	char securemode[20] = "securemode=";
	char signmethod[20] = "signmethod=";
	char* hmacmd5 = "hmacmd5";
	char* hmacsha1 = "hmacsha1";
	char mqttclientid[100] = "";
}ConnectInfo_t;
static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
	unsigned int *ecx, unsigned int *edx)
{
	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
		: "=a" (*eax),
		"=b" (*ebx),
		"=c" (*ecx),
		"=d" (*edx)
		: "0" (*eax), "2" (*ecx));
}
int main(int argc, char* argv[])
{
	ConnectInfo_t connectinfo;

	sprintf(connectinfo.url, "tcp://%s.iot-as-mqtt.%s.aliyuncs.com", connectinfo.productkey, connectinfo.location);
	sprintf(connectinfo.username, "%s&%s", connectinfo.devicename, connectinfo.productkey);
	sprintf(connectinfo.mqttclientid, "%s|%s%c,%s%s|", connectinfo.clientid, connectinfo.securemode, '3', connectinfo.signmethod, connectinfo.hmacsha1);

	cout << "url is " << connectinfo.url << endl << "username is " << connectinfo.username << endl << "clientid is " << connectinfo.mqttclientid << endl;
/**********************************²âÊÔrapidjson************************************/
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writer.StartObject();
	writer.Key("test");
	writer.String("json");
	writer.EndObject();
	cout << buffer.GetString();
/**********************************²âÊÔrapidjson************************************/


/***********************************¶ÁÈ¡cpuid**************************************/
	char cpuid[100];
	FILE* fp_cpuId = popen("sudo cat /sys/class/dmi/id/board_serial", "r");

	fgets(cpuid, sizeof(cpuid), fp_cpuId);
	pclose(fp_cpuId);
//	strcpy((char*)clientid.c_str(), cpuid);
	unsigned eax, ebx, ecx, edx;
	/* EDIT */
	eax = 3; /* processor serial number */
	native_cpuid(&eax, &ebx, &ecx, &edx);

	/** see the CPUID Wikipedia article on which models return the serial
		number in which registers. The example here is for
		Pentium III */
	printf("serial number 0x%08x%08x\n", edx, ecx);

/***********************************¶ÁÈ¡cpuid**************************************/

/***********************************²âÊÔ´®¿Ú**************************************/
/*	Serial s;
	if (s.open("/dev/ttyS0", 115200, 8,'N', 1) != Serial::OK)
	{
		printf("Cannot open serial port!\n");
		return -1;
	}
	char test[] = "test data";
	s.write(test, strlen(test));
	s.close();*/
/***********************************²âÊÔ´®¿Ú**************************************/

/***********************************ÃÜÔ¿¼ÆËã**************************************/


	// The data that we're going to hash using HMAC
	char data[100] = {0};
	unsigned char digest[EVP_MAX_MD_SIZE] = { '\0' };
	unsigned int digest_len = 0;
	sprintf(data, "clientId%sdeviceName%sproductKey%s", connectinfo.clientid, connectinfo.devicename, connectinfo.productkey);

	// Using sha1 hash engine here.
	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	HMAC(EVP_sha1(), connectinfo.devicesecret, strlen(connectinfo.devicesecret), (unsigned char*)data, strlen(data), digest, &digest_len);

	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine
	char mdString[41] = { '\0' };
	for (int i = 0; i < 20; i++)
		sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
	strcpy(connectinfo.password, mdString);

/***********************************ÃÜÔ¿¼ÆËã**************************************/


/***********************************modbus²âÊÔ**************************************/
	modbus_t *mb;
	mb = modbus_new_rtu("/dev/ttyS0",115200,'N',8,1);
	modbus_set_slave(mb, 1);
	modbus_set_debug(mb, 1);
	modbus_connect(mb);

//	while (1);
	uint16_t buff[10];
	int regs = modbus_read_registers(mb, 0, 10, buff);
	for (int i = 0; i < 10; i++)
	{
		cout << buff[i] << endl;
	}
/***********************************modbus²âÊÔ**************************************/
	
	MyMqtt mqtttest;
	mqtttest.connect(connectinfo.url, connectinfo.mqttclientid, connectinfo.username, connectinfo.password);
	return 0;
}