#include "soft_serial.h"
#include "serial.h"
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;
//using namespace serial;
//Serial* s;
/*
int MySerial::SerialInit(char* port,int baud)
{
	Serial BaseSerial(port,baud)
	s = new Serial("/dev/ttyS0", 115200, Timeout::simpleTimeout(1000), databits, parity, stopbits);

}

int init()
{
	bytesize_t databits = eightbits;
	parity_t parity = parity_none;
	stopbits_t stopbits = stopbits_one;
	s = new Serial("/dev/ttyS0", 115200, Timeout::simpleTimeout(1000), databits, parity, stopbits);
	uint8_t data[100] = {"hello,i am new"};
//	s.write(data, 14);
	s->write("i am writer\n");
//	std::thread th(readthread);
//	th.join();
}
void readthread()
{
	while (1)
	{
		uint8_t readbuff[1024];
		s->read(readbuff, 1024);
		cout << readbuff << endl;
		memset(readbuff, 0, 1024);
	}
}*/