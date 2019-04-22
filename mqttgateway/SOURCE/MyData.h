#pragma once
#ifndef _MY_DATA_H_
#define _MY_DADA_H_
//设备的设置信息
typedef struct _DeviceInfo_t {
	int id;					//devinfo id
	int PortId;				//绑定的PortId
	int address;				//设备地址
	int regedian;			//设备大小端
	int byteorder;			//设备寄存器高低位
	char DeviceName[100];	//设备名

	int devcount;
}DeviceInfo_t;

//端口的参数设置
typedef struct _PortInfo_t {
	int id;					//Portinfo id
	int PortId;				//设定的PortId
	int PortNum;
	int PortType;			//端口类型
	int baud;				//波特率
	char Parity[10];		//奇偶位
	int DataBits;			//数据位
	int StopBits;			//停止位
	int DelayTime;			//等待时间
	char PortName[20];		//串口命名

	int portcount;
}PortInfo_t;

//控制主题相关参数
typedef struct _ThemeCtrl_t {
	int id;					//ThemeCtrl id
	int Enable;				//使能控制主题
	int MqttId;				//绑定的MqttId
	int	QosPub;				//"发布"主题的Qos
	int QosSub;				//"订阅"主题的Qos
	char proto[10];
	char CtrlName[100];		//控制主题的命名
	char CtrlPub[100];		//发布的主题
	char CtrlSub[100];		//订阅的主题
	int uartID;
}ThemeCtrl_t;

//上传主题相关参数
typedef struct _ThemeUpload_t {
	int id;					//ThemeUpload Id
	int Enable;				//是能上传主题
	int MqttId;				//绑定的MqttId
	int UploadId;			//设定的发布主题Id
	int PubPeriod;			//发布消息的时间间隔
	int QosPub;				//发布主题的Qos
	char CtrlPub[100];		//发布的主题
	char UploadName[100];	//定时上传主题的命名
	char Proto[20];			//通信协议
}ThemeUpload_t;

//上传主题绑定的变量
typedef struct _ThemeUploadList_t {
	int id;					//ThemeUploadList Id
	int Enable;				//使能上传
	int MqttId;				//绑定的MqttId
	int UploadId;			//绑定的UploadId
	int DevId;				//绑定的设备信息中的DevInfo的id
	int VarId;				//绑定变量表VarParam的Id
	char VarName[100];		//变量名

	int UploadCount;
}ThemeUploadList_t;

typedef struct _VarParam_t {
	int id;					//VarParam Id
	char DataType[20];		//变量类型
	int DecimalsBit;		//小数位
	int obj;				//
	int RegAdr;				//寄存器地址
	char RegType[10];		//寄存器类型：B0（逻辑线圈），B1（开关量输入），W3（保持寄存器），W4（输入寄存器）
	char VarName[100];		//变量命名
	int PortId;				//绑定的串口Id
	int DevId;				//绑定的设备Id
	float modules;			//转换系数

	int VarCount;		//变量的数量
}VarParam_t;

typedef struct _ConnectInfo_t {
	int id;
	int Session;
	int Enable;
	int KeepAlive;
	int ServerPort;

	char ClientId[100];
	char UserName[100];
	char Password[100];
	int PeriodDef;
	int EnableDef;
	char TopicDef[100];
	int QosDef;
	int EnableAlarm;
	int QosAlarm;
	char TopicAlarm[100];

	char ServerLink[100];
	char MqttName[100];

	int MqttCount;
	
}ConnectInfo_t;

extern ConnectInfo_t MqttInfo[4];
extern DeviceInfo_t DevInfo[10];
extern PortInfo_t PortInfo[10];
extern ThemeCtrl_t ThemeCtrl[10];
extern ThemeUpload_t ThemeUpload[10];
extern ThemeUploadList_t ThemeUploadList[100];
extern VarParam_t VarParam[100];

#endif // !_MY_DATA_H_
