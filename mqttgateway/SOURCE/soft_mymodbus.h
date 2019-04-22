#pragma once
#ifndef _SOFT_MYMODBUS_H_
#define _SOFT_MYMODBUS_H_

#include"modbus/modbus.h"

class MyModbus {
public:
	MyModbus();
	~MyModbus();
	int modbusconfig(int portid, int baud, char* parity,int data_bit, int stop_bit);
	
	int modbusread1(int slave);
private:
	modbus_t* mb;
};

void* modbusread(char* varname, uint16_t buff[]);

#endif // _SOFT_MYMODBUS_H_
