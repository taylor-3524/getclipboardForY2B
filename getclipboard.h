#ifndef GETCLIPBOARD_H
#define GETCLIPBOARD_H
#pragma once
#include "mythread.h"
#include <QtWidgets/QMainWindow>
#include "ui_getclipboard.h"

class getclipboard : public QMainWindow
{
	Q_OBJECT
public:

	explicit getclipboard(QWidget *parent = 0);
	~getclipboard();

	//停止线程槽函数
	void stopThread();

public slots:
	void receiveMsgFromThread(QString, int);   //接收子线程的消息
    void sendMsgToThreadFun();   //发送给子线程的消息
	void start_btn_Clicked();
	void clear_btn_Clicked();
	void copy_btn_Clicked();
	void stop_btn_Clicked();	//停止线程槽函数
    void openUrl(QString);
    void batBtnClicked();
    void downMVBtnClicked();

private:
	Ui::getclipboardClass ui;
	MyThread *thread;

signals:
    //给子线程发消息
    void sengMsgToThread(QString);
};

#endif // !GETCLIPBOARD_H
