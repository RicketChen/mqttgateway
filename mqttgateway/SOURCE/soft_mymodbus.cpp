#include "soft_mymodbus.h"

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "MyData.h"
using std::cout;
using std::endl;

MyModbus::MyModbus()
{

}
MyModbus::~MyModbus()
{

}

int MyModbus::modbusconfig(int portid, int baud, char* parity,int data_bit, int stop_bit)
{
	char serialport[20];
	sprintf(serialport, "%s%d", "/dev/ttyS", portid-1);
	cout << "create modbus ,port is " << serialport << endl;
	mb = modbus_new_rtu(serialport, 115200, 'N', 8, 1);
//	modbus_set_slave(mb, 1);
	modbus_set_debug(mb, 1);
//	modbus_connect(mb);
}
int MyModbus::modbusread1(int slave)
{
	uint16_t buff[10];
	uint8_t buff8[10];
	modbus_set_slave(mb,slave);
	modbus_connect(mb);

	int regs = modbus_read_bits(mb, 1, 5, buff8);
//	int regs = modbus_read_input_bits(md, 10, 1, buff);
//	int regs = modbus_read_registers(mb, 1, 2, buff);
//	int regs = modbus_read_input_registers(mb, 1, 10, buff);

	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", buff8[i]);
	}
	modbus_close(mb);
}

void* modbusread(char* varname,uint16_t buff[])
{
	modbus_t* mb;
	int varsubscript;
	int portid;
	int portsubscript;
	int devsubscript;
	char serialport[20];
	for (int i = 0; i < VarParam[0].VarCount;i++)
	{
		if (!strcmp(VarParam[i].VarName, varname))
		{
			varsubscript = i;
			portid = VarParam[i].PortId;
			break;
		}
	}
	for (int i = 0; i < PortInfo[0].portcount; i++)
	{
		if (PortInfo[i].PortId = portid)
		{
			portsubscript = i;
			break;
		}
	}
	for (int i = 0; i < DevInfo[0].devcount; i++)
	{
		if (VarParam[varsubscript].DevId == DevInfo[i].id)
			devsubscript = i;
	}
	sprintf(serialport, "%s%d", "/dev/ttyS", PortInfo[portsubscript].PortId- 1);
//	cout << "create modbus ,port is " << serialport << endl;
	char parity = PortInfo[portsubscript].Parity[0];
	mb = modbus_new_rtu(serialport,PortInfo[portsubscript].baud, parity,8 - PortInfo[portsubscript].DataBits,1 - PortInfo[portsubscript].StopBits);
//	modbus_set_debug(mb, 1);
	modbus_set_slave(mb, DevInfo[devsubscript].address);
	modbus_connect(mb);
	memset(buff, 0, sizeof(buff));
	if (!strcmp("B0", VarParam[varsubscript].RegType))
	{
		modbus_read_bits(mb, VarParam[varsubscript].RegAdr, 1, (uint8_t*)buff);
//		printf("buff8 is %d\n", buff8[0]);
	}
	else if (!strcmp("B1", VarParam[varsubscript].RegType))
	{
		modbus_read_input_bits(mb, VarParam[varsubscript].RegAdr, 1, (uint8_t*)buff);
//		printf("buff8 is %d\n", buff8[0]);
	}
	else if (!strcmp("W3", VarParam[varsubscript].RegType))
	{
		modbus_read_registers(mb, VarParam[varsubscript].RegAdr, 1, (uint16_t*)buff);
//		printf("buff16 is %d\n", buff16[0]);

	}
	else if (!strcmp("W4", VarParam[varsubscript].RegType))
	{
		modbus_read_input_registers(mb, VarParam[varsubscript].RegAdr, 1, (uint16_t*)buff);
//		printf("buff16 is %d\n", buff16[0]);

	}
	modbus_close(mb);
}