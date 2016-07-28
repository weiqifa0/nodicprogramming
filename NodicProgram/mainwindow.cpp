#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QDateTime>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //app.seWindowIcon(QIcon("nodic.ico"));
    //setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮
    //setWindowFlags(Qt::FramelessWindowHint);//设置无边框
    //setAttribute(Qt::WA_TranslucentBackground, true);//设置背景透明
    this->setFixedSize( this->width(),this->height());//设置窗体固定大小，不能改变窗体大小
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray readCmd;
    QByteArray readCmd2;
    //执行cmd的相关命令
    QProcess p(0);
    //擦除
    p.start(ERASE_ALL_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("擦除失败");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("擦除成功");
    }
    //下载
    p.start(DOWNLOAD_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    qDebug()<< readCmd;
    //在控件上面输出结果
    ui->textBrowser->append(readCmd);
    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("烧录失败");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("烧录成功");
    }
    //复位
    p.start(RESET_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("复位失败");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("烧录成功");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray readCmd;
    QProcess p(0);
    p.start(ALL_IN_ONE_CMD_1);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    readCmd="";
    p.start(ALL_IN_ONE_CMD_2);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    readCmd="";
    p.start("del temp.hex");
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->setText("1、安装了nRF5x-Command-Line-Tools_8_4_0_Installer.exe\r\n\
2、C:\\Program Files\\NordicSemiconductor\\nrf5x\bin\\nrfjprog.ini 中Family改为NRF52\r\n\
3、ts102.hex放在与本该程序相同的目录下\r\n\
4、存放的路径不能有中文，桌面也不行\r\n\
5、合并文件时把，nrf52832_xxaa_s132_bootloader.hex s132_nrf52_2.0.0_softdevice.hex nrf52832_xxaa_s132.hex 放在此exe同一个目录下\r\n\
6、联系QQ：329410527 ");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);
    ui->label->setPalette(pa);
    ui->label->setText("烧录结果");
}
