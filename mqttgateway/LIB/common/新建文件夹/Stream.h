//���ļ�ʵ��һ���ַ���
#ifndef STREAM_H
#define STREAM_H

class Stream
{

private:
	//������
	char* buffer;
	//����������
	int capacity;
	//���Ŀ�ͷ
	int start;
	//���ĳ���
	int length;

public:
	//��ʼ��һ����������Ϊ����ʼ����
	Stream(int initCapacity = 16);
	//����
	~Stream();
	//��ȡ���ĳ���
	int getLength();
	//������ĩβ����һ�ַ�
	void append(char aChar);
	//������ĩβ���Ӷ���ַ�
	void append(const char* buf, int len);
	//�鿴���ĵ�һ���ַ����������Ϊ0�򷵻�0
	char peek();
	//�鿴����ͷ�Ķ���ַ�������ʵ�ʲ鿴���ĳ���
	int peek(char* buf, int len);
	//ȡ�����ĵ�һ���ַ����������Ϊ0�򷵻�0
	char take();
	//ȡ������ͷ�Ķ���ַ�������ʵ��ȡ���ĳ���
	int take(char* buf, int len);

private:
	//��������������
	void expand();

};

#endif