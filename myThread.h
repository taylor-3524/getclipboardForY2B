#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QThread>

//myThread.h 
class MyThread : public QThread
{
	Q_OBJECT
public:
	explicit MyThread(QObject *parent = 0);
	void flagStatusFalse();	//�����߳��е��õĺ��������������߳���while(flag)����ѭ��
	void flagStatusTrue();	//����while��ѭ��
	void linkNumToZero();//�������Ӽ�����

protected:
	//QThread���麯��
	//�̴߳�����
	//����ֱ�ӵ��ã�ͨ��start()��ӵ���
	void run();

signals:
	//�����̷߳���Ϣ
	void sendMsgToMain(QString,int);


public slots:
    //�������̵߳���Ϣ
    void recMegFromMain(QString);

};
#endif // MYTHREAD_H
