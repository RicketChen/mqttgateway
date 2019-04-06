#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "mqtt/async_client.h"

#include "openssl/hmac.h"
#include "modbus/modbus-rtu.h"
//#include "Serial.h"
#include "soft_serial.h"
#include "common/Serial.h"

using std::cout;
using std::endl;
const std::string SERVER_ADDRESS("tcp://192.168.41.128:1883");
const std::string CLIENT_ID("async_subcribe_cpp");
const std::string TOPIC("hello");

const int	QOS = 1;
const int	N_RETRY_ATTEMPTS = 5;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		std::cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		std::cout << name_ << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

public:
	action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
	public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		std::cout << "Connection attempt failed" << std::endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << TOPIC << "'\n"
			<< "\tfor client " << CLIENT_ID
			<< " using QoS" << QOS << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
		: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////
void fun1()
{
	while (1)
	{
		cout << "fun1" << endl;
		sleep(1);
	}
}
void fun2()
{
	while (1)
	{
		cout << "fun2" << endl;
		sleep(1);
	}
}
int main(int argc, char* argv[])
{
	Serial s;
	//²âÊÔopen()
	if (s.open("/dev/ttyS0", 115200, 8,'N', 1) != Serial::OK)
	{
		printf("Cannot open serial port!\n");
		return -1;
	}
	//²âÊÔwriteºÍint read(char* buf,int len,int timeout);
	const char* output = "hello,myself!\n";
	char buffer[1024];
	int recelen;
	while (1)
	{
		if (s.read(buffer, sizeof(buffer), "\r\n", 100, &recelen) == 1)
		{
			cout << buffer << endl;
			memset(buffer, 0, sizeof(buffer));
		}
	}

	return 0;
	char clientid[100] = { "clientId" };
	char devicename[100] = { "deviceName" };
	char productkey[100] = { "productKey" };

	strcat(clientid, "123");
	strcat(devicename, "123");
	strcat(productkey, "123");
	// The key to hash
	char key[100] = { "123" };

	// The data that we're going to hash using HMAC
	char data[100] = {0};
	unsigned char digest[EVP_MAX_MD_SIZE] = { '\0' };
	unsigned int digest_len = 0;
	sprintf(data, "%s%s%s", clientid, devicename, productkey);
	// Using sha1 hash engine here.
	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	HMAC(EVP_sha1(), key, strlen(key), (unsigned char*)data, strlen(data), digest, &digest_len);

	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine
	char mdString[41] = { '\0' };
	for (int i = 0; i < 20; i++)
		sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);

	printf("HMAC digest: %s\n", mdString);
	modbus_t *mb;
	mb = modbus_new_rtu("/dev/ttyS0",115200,'N',8,1);
	modbus_set_slave(mb, 1);
	modbus_set_debug(mb, 1);
	modbus_connect(mb);
//	struct timeval t;
//	t.tv_sec = 0;
//	t.tv_usec = 1000000;//1000ms
//	modbus_set_response_timeout(mb, &t);


	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);
	uint16_t buff[10];
	int regs = modbus_read_registers(mb, 0, 10, buff);
	for (int i = 0; i < 10; i++)
	{
		cout << buff[i] << endl;
	}
	cout << "finsh" << endl;
	while (1);
	mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

	callback cb(client, connOpts);
	client.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		client.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception&) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << std::endl;
		return 1;
	}

	// Just block till user tells us to quit.

	while (std::tolower(std::cin.get()) != 'q')
		;

	// Disconnect

	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		client.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc.what() << std::endl;
		return 1;
	}
	return 0;
}