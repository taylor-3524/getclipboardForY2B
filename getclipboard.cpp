#include "getclipboard.h"
#include <QClipboard>
#include<QFile>
#include <QDesktopServices>
#include<QDebug>
#include<QProcess>
#pragma execution_character_set("utf-8")		//防止乱码
getclipboard::getclipboard(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    connect(ui.label,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));  //在.h里面定义一个槽

	//创建线程
	thread = new MyThread(this);

	//关闭界面时，杀死线程
	connect(this, &getclipboard::destroyed, this, &getclipboard::stopThread);

	//开始线程
	connect(ui.start_btn, SIGNAL(clicked()), this, SLOT(start_btn_Clicked()));
	
	//按钮结束线程
	connect(ui.stop_btn, SIGNAL(clicked()), this, SLOT(stop_btn_Clicked()));

	//接收子线程发来的数据
	connect(thread, &MyThread::sendMsgToMain, this, &getclipboard::receiveMsgFromThread);

    //发送给子线程端口号
    connect(this, &getclipboard::sengMsgToThread, thread, &MyThread::recMegFromMain);


    connect(ui.downMVBtn,SIGNAL(clicked()),this,SLOT(downMVBtnClicked()));
	connect(ui.clear_btn, SIGNAL(clicked()), this, SLOT(clear_btn_Clicked()));
	connect(ui.copy_btn, SIGNAL(clicked()), this, SLOT(copy_btn_Clicked()));
    connect(ui.batBtn,SIGNAL(clicked()),this,SLOT(batBtnClicked()));

}
void getclipboard::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}


//创建剪切板指针
QClipboard *board = QApplication::clipboard();

getclipboard::~getclipboard() {

}

void getclipboard::stopThread() {
	//停止线程
	thread->quit();
	//等待线程处理完手上的事情
	//在线程还未结束的时候会假死
	thread->wait();
}

void getclipboard::stop_btn_Clicked() {
	
	//停止线程
	thread->quit();
	//打断线程中的死循环
	thread->flagStatusFalse();
	
	ui.latest_label->setText("剪贴板自由");
	
}

void getclipboard::sendMsgToThreadFun(){
    emit sengMsgToThread(QString::number(ui.portSpinBox->value()));
}

void getclipboard::start_btn_Clicked() {
	//启动线程，处理数据
	thread->start();
	thread->flagStatusTrue();
	ui.latest_label->setText("监视中");
    sendMsgToThreadFun();

}


//接收子线程消息
void getclipboard::receiveMsgFromThread(QString link, int linkNum) {
	ui.clipboard_text->append(link);
	QString LN = QString::number(linkNum);
	ui.latest_label->setText("监视中："+LN);

}

//清空文本框里的内容
void getclipboard::clear_btn_Clicked() {
	ui.clipboard_text->setText("");
	ui.latest_label->setText("剪贴板自由");
	thread->linkNumToZero();
}

//将文本框的内容复制到剪贴板
void getclipboard::copy_btn_Clicked() {
	QString link = ui.clipboard_text->toPlainText();
	QClipboard *board = QApplication::clipboard();
	board->setText(link);
	ui.clipboard_text->append("已复制到剪贴板");
}

//生成批处理命令
void getclipboard::batBtnClicked(){
    QString link = ui.clipboard_text->toPlainText();
    QFile file("youtube一键下载到当前目录.bat");
    //已读写方式打开文件，
        //如果文件不存在会自动创建文件
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"打开失败";
        }else{
            qDebug()<<"打开成功";
        }
        //获得文件大小
            qint64 pos;
            pos = file.size();
            //重新定位文件输入位置，这里是定位到文件尾端
            file.seek(pos);

            //写入文件
            qint64 length = -1;
            length = file.write(link.toLatin1(),link.length());

            if(length == -1){
                qDebug()<<"写入文件失败";
            }else{
                qDebug()<<"写入文件成功";
            }

            //关闭文件
            file.close();
}

//执行批处理程序
void getclipboard::downMVBtnClicked(){
    QProcess downMV(this);
    downMV.startDetached("youtube一键下载到当前目录.bat");
}
