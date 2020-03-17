#include "getclipboard.h"
#include <QClipboard>
#include<QFile>
#include <QDesktopServices>
#include<QDebug>
#include<QProcess>
#pragma execution_character_set("utf-8")		//��ֹ����
getclipboard::getclipboard(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    connect(ui.label,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));  //��.h���涨��һ����

	//�����߳�
	thread = new MyThread(this);

	//�رս���ʱ��ɱ���߳�
	connect(this, &getclipboard::destroyed, this, &getclipboard::stopThread);

	//��ʼ�߳�
	connect(ui.start_btn, SIGNAL(clicked()), this, SLOT(start_btn_Clicked()));
	
	//��ť�����߳�
	connect(ui.stop_btn, SIGNAL(clicked()), this, SLOT(stop_btn_Clicked()));

	//�������̷߳���������
	connect(thread, &MyThread::sendMsgToMain, this, &getclipboard::receiveMsgFromThread);

    //���͸����̶߳˿ں�
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


//�������а�ָ��
QClipboard *board = QApplication::clipboard();

getclipboard::~getclipboard() {

}

void getclipboard::stopThread() {
	//ֹͣ�߳�
	thread->quit();
	//�ȴ��̴߳��������ϵ�����
	//���̻߳�δ������ʱ������
	thread->wait();
}

void getclipboard::stop_btn_Clicked() {
	
	//ֹͣ�߳�
	thread->quit();
	//����߳��е���ѭ��
	thread->flagStatusFalse();
	
	ui.latest_label->setText("����������");
	
}

void getclipboard::sendMsgToThreadFun(){
    emit sengMsgToThread(QString::number(ui.portSpinBox->value()));
}

void getclipboard::start_btn_Clicked() {
	//�����̣߳���������
	thread->start();
	thread->flagStatusTrue();
	ui.latest_label->setText("������");
    sendMsgToThreadFun();

}


//�������߳���Ϣ
void getclipboard::receiveMsgFromThread(QString link, int linkNum) {
	ui.clipboard_text->append(link);
	QString LN = QString::number(linkNum);
	ui.latest_label->setText("�����У�"+LN);

}

//����ı����������
void getclipboard::clear_btn_Clicked() {
	ui.clipboard_text->setText("");
	ui.latest_label->setText("����������");
	thread->linkNumToZero();
}

//���ı�������ݸ��Ƶ�������
void getclipboard::copy_btn_Clicked() {
	QString link = ui.clipboard_text->toPlainText();
	QClipboard *board = QApplication::clipboard();
	board->setText(link);
	ui.clipboard_text->append("�Ѹ��Ƶ�������");
}

//��������������
void getclipboard::batBtnClicked(){
    QString link = ui.clipboard_text->toPlainText();
    QFile file("youtubeһ�����ص���ǰĿ¼.bat");
    //�Ѷ�д��ʽ���ļ���
        //����ļ������ڻ��Զ������ļ�
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"��ʧ��";
        }else{
            qDebug()<<"�򿪳ɹ�";
        }
        //����ļ���С
            qint64 pos;
            pos = file.size();
            //���¶�λ�ļ�����λ�ã������Ƕ�λ���ļ�β��
            file.seek(pos);

            //д���ļ�
            qint64 length = -1;
            length = file.write(link.toLatin1(),link.length());

            if(length == -1){
                qDebug()<<"д���ļ�ʧ��";
            }else{
                qDebug()<<"д���ļ��ɹ�";
            }

            //�ر��ļ�
            file.close();
}

//ִ�����������
void getclipboard::downMVBtnClicked(){
    QProcess downMV(this);
    downMV.startDetached("youtubeһ�����ص���ǰĿ¼.bat");
}
