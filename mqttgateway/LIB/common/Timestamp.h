//���ļ�����һ�����뼶��ʱ���������
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

//ʱ�������
typedef long long timestamp_t;

class Timestamp
{

public:
	//��ȡ�Ժ���Ƶ�ʱ���
	static timestamp_t now();

};

#endif