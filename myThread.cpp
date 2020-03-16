#include "mythread.h"
#include <QClipboard>
//�����ж��߳��˳��ı�־
bool myFlag;

extern QClipboard *board;

MyThread::MyThread(QObject *parent) : QThread(parent)
{
	
}
int linkNum=1 ;//���Ӽ�����
QString port=10809;//����˿�

//�������̵߳Ķ˿ں�
void MyThread::recMegFromMain(QString uiPort){
    port=uiPort;
}

void MyThread::run()
{
	
	
	QString oldStr;


	while (myFlag)
	{
		sleep(1);

		//��ȡ���а岢�����ַ���
	    
	    QString str = board->text();
		int endAddr = str.indexOf("&");
		if (endAddr != -1) {
			QString link = str.mid(0, endAddr);
            link = "youtube-dl --proxy 127.0.0.1:"+ port + " " + link;
		

			//�������������Ƿ��б仯
		
			if (oldStr != link) {
				//������Ϣ�����߳�
				emit sendMsgToMain(link,linkNum);

			linkNum++;
			oldStr = link;
			}
		}

	}
}

void MyThread::flagStatusFalse()
{
	myFlag = false;
}

void MyThread::flagStatusTrue() {
	myFlag = true;
}

void MyThread::linkNumToZero() {
	linkNum = 1;
}
