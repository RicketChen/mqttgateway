#pragma once
#ifndef _MY_DATA_H_
#define _MY_DADA_H_
//�豸��������Ϣ
typedef struct _DeviceInfo_t {
	int id;					//devinfo id
	int PortId;				//�󶨵�PortId
	int adress;				//�豸��ַ
	int regedian;			//�豸��С��
	int byteorder;			//�豸�Ĵ����ߵ�λ
	char DeviceName[100];	//�豸��
}DeviceInfo_t;

//�˿ڵĲ�������
typedef struct _PortInfo_t {
	int id;					//Portinfo id
	int PortId;				//�趨��PortId
	int PortType;			//�˿�����
	int baud;				//������
	char Parity[10];		//��żλ
	int DataBits;			//����λ
	int StopBits;			//ֹͣλ
	int DelayTime;			//�ȴ�ʱ��
	char PortName[20];		//��������
}PortInfo_t;

//����������ز���
typedef struct _ThemeCtrl_t {
	int id;					//ThemeCtrl id
	int Enable;				//ʹ�ܿ�������
	int MqttId;				//�󶨵�MqttId
	int	QosPub;				//"����"�����Qos
	int QosSub;				//"����"�����Qos
	char CtrlName[100];		//�������������
	char CtrlPub[100];		//����������
	char CtrlSub[100];		//���ĵ�����
	int uartID;
}ThemeCtrl_t;

//�ϴ�������ز���
typedef struct _ThemeUpload_t {
	int id;					//ThemeUpload Id
	int Enable;				//�����ϴ�����
	int MqttId;				//�󶨵�MqttId
	int UploadId;			//�趨�ķ�������Id
	int PubPeriod;			//������Ϣ��ʱ����
	int QosPub;				//���������Qos
	char CtrlPub[100];		//����������
	char UploadName[100];	//��ʱ�ϴ����������
	char Proto[20];			//ͨ��Э��
}ThemeUpload_t;

//�ϴ�����󶨵ı���
typedef struct _ThemeUploadList_t {
	int id;					//ThemeUploadList Id
	int Enable;				//ʹ���ϴ�
	int MqttId;				//�󶨵�MqttId
	int UploadId;			//�󶨵�UploadId
	int DevId;				//�󶨵��豸��Ϣ�е�DevInfo��id
	int VarId;				//�󶨱�����VarParam��Id
	char VarName[100];		//������
}ThemeUploadList_t;

typedef struct _VarParam_t {
	int id;					//VarParam Id
	char DataType[20];		//��������
	int DecimalsBit;		//С��λ
	int obj;				//
	int RegAdr;				//�Ĵ�����ַ
	char RegType[10];		//�Ĵ������ͣ�B0���߼���Ȧ����B1�����������룩��W3�����ּĴ�������W4������Ĵ�����
	char VarName[100];		//��������
	int PortId;				//�󶨵Ĵ���Id
	int DevId;				//�󶨵��豸Id
	float modules;			//ת��ϵ��
}VarParam_t;

typedef struct _ConnectInfo_t {
	int id;
	int mqttid;
	char url[100] = "";
	char productkey[100] = "";
	char devicename[100] = "";
	char devicesecret[100] = "";
	char username[100] = "";
	char password[100] = "";
	char clientid[100] = "";
	char securemode[20] = "";
	char signmethod[20] = "";
	char* hmacmd5 = "";
	char* hmacsha1 = "";
}ConnectInfo_t;

extern ConnectInfo_t MqttInfo[100];
#endif // !_MY_DATA_H_
