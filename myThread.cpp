#include "mythread.h"
#include <QClipboard>
//用来判断线程退出的标志
bool myFlag;

extern QClipboard *board;

MyThread::MyThread(QObject *parent) : QThread(parent)
{
	
}
int linkNum=1 ;//链接计数器
QString port=10809;//代理端口

//接受主线程的端口号
void MyThread::recMegFromMain(QString uiPort){
    port=uiPort;
}

void MyThread::run()
{
	
	
	QString oldStr;


	while (myFlag)
	{
		sleep(1);

		//获取剪切板并处理字符串
	    
	    QString str = board->text();
		int endAddr = str.indexOf("&");
		if (endAddr != -1) {
			QString link = str.mid(0, endAddr);
            link = "youtube-dl --proxy 127.0.0.1:"+ port + " " + link;
		

			//检查剪贴板内容是否有变化
		
			if (oldStr != link) {
				//发送消息给主线程
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
