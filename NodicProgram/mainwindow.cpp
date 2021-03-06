#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QDateTime>
#include <QProcess>
#include <mythread.h>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 0) }");
    QString readCmd;//=QStringLiteral("你好中文！");
    //执行cmd的相关命令
    QProcess p(0);

    //判断文件是否存在 "cmd", QStringList()<<"/c"<<"dir"
    p.start("cmd", QStringList()<<"/c"<<"if exist ts102.hex (echo Ts102.hex File exists ) else (echo Ts102.hex File does not exist,please copy here)");
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    qDebug()<<readCmd;
    if(ui->checkBox_3->isChecked())
    {
        ui->lineEdit_2->setEnabled(false);
    }
    //设置快捷键
    ui->pushButton->setFocus();
    ui->pushButton->setShortcut(QKeySequence::InsertParagraphSeparator);//设置快捷键为键盘的“回车”键
    ui->pushButton->setShortcut(Qt::Key_Enter);
    ui->pushButton->setShortcut(Qt::Key_Return);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//函 数 名：HexToAsc()
//功能描述：把16进制转换为ASCII
char MainWindow::IntToStr(char aChar)
{
    char ss;
    printf("%s %d\n",__FUNCTION__,aChar);
    switch(aChar)
    {
        case 0: ss= '0';break;
        case 1: ss= '1';break;
        case 2: ss= '2';break;
        case 3: ss= '3';break;
        case 4: ss= '4';break;
        case 5: ss= '5';break;
        case 6: ss= '6';break;
        case 7: ss= '7';break;
        case 8: ss= '8';break;
        case 9: ss= '9';break;

        case 10: ss= 'A';break;
        case 11: ss= 'B';break;
        case 12: ss= 'C';break;
        case 13: ss= 'D';break;
        case 14: ss= 'E';break;
        case 15: ss= 'F';break;
        default:break;
    }
    printf("%s %c\n",__FUNCTION__,ss);
    return ss;
}
//函 数 名：StrToInt()
//功能描述：把字符转换成对应的数字，比如a转换成10
char MainWindow::StrToInt(char aChar)
{
    char ss;
    printf("%s %c\n",__FUNCTION__,aChar);
    switch(aChar)
    {
        case '0': ss= 0;break;
        case '1': ss= 1;break;
        case '2': ss= 2;break;
        case '3': ss= 3;break;
        case '4': ss= 4;break;
        case '5': ss= 5;break;
        case '6': ss= 6;break;
        case '7': ss= 7;break;
        case '8': ss= 8;break;
        case '9': ss= 9;break;

        case 'A': ss= 10;break;
        case 'B': ss= 11;break;
        case 'C': ss= 12;break;
        case 'D': ss= 13;break;
        case 'E': ss= 14;break;
        case 'F': ss= 15;break;
        default:break;
    }
    printf("%s %d\n",__FUNCTION__,ss);
    return ss;
}
void MainWindow::on_pushButton_clicked()
{
    QByteArray readCmd;
    QByteArray readCm;
    QByteArray readCmdMac;
    char readCharMac;
    static unsigned long erase=0;
    int i=0;
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->label->setPalette(pa);
    ui->label->setText("开始...");
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);

    //执行cmd的相关命令
    QProcess p(0);

    p.start("cmd", QStringList()<<"/c"<<"if exist ts102.hex (echo ts102.hex File exists ) else (echo ts102.hex File does not exist,please copy here)");
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    qDebug()<<readCmd;


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
    MyThread thread;
    thread.start();
    int thrCount=0;
    ui->progressBar->setRange(0,5000);
    do
    {
        QCoreApplication::processEvents();/*Don't move it*/
        thrCount++;
        ui->progressBar->setValue(thrCount);
        QThread::usleep(3000);
    }while(thread.stop==false);
    thread.stop=false;
    thread.quit();
    ui->progressBar->setValue(thrCount+(5000-thrCount)/2);

    //读取MAC地址
    p.start(READ_MAC_CMD);
    p.waitForStarted();
    p.waitForFinished();
    //获取MAC地址
    readCm  = p.readAllStandardOutput().trimmed();
    if(ui->checkBox_3->isChecked()==true)
    {
        QByteArray macValue=ui->lineEdit->text().right(8).toLatin1();
        qDebug()<<macValue;
        bool ok;
        int macValueInt=macValue.toInt(&ok,16);
        qDebug("%x",macValueInt);
        //调用cmd相关命令
        QByteArray readCmd;
        QProcess p(0);
        //写入数据
        QString cmdWriteValue="nrfjprog.exe --memwr 0x10001094 --val ";
        ui->textBrowser->append(cmdWriteValue+ui->lineEdit->text());
        cmdWriteValue.append(QString::number(macValueInt));
        qDebug()<<cmdWriteValue;
        p.start(cmdWriteValue);
        p.waitForStarted();
        p.waitForFinished();
        readCmd = p.readAllStandardOutput();
        readCmd+= p.readAllStandardError();
        ui->textBrowser->append(readCmd);
        if(readCmd.contains("ERROR"))
        {
            ui->label->setText("硬件失败");
            return;
        }
        else
        {
            ui->textBrowser->append(tr("##########################写入硬件版本号成功.###############"));
        }
    }
    //向flash写入值
    if(ui->checkBox_4->isChecked()==true)
    {
        QByteArray readCmd;
        QProcess p(0);
        //写入数据
        QString cmdWriteValue="nrfjprog.exe --memwr 0x10001098 --val 0xFF12FF34";
        ui->textBrowser->append(cmdWriteValue);
        qDebug()<<cmdWriteValue;
        p.start(cmdWriteValue);
        p.waitForStarted();
        p.waitForFinished();
        readCmd = p.readAllStandardOutput();
        readCmd+= p.readAllStandardError();
        ui->textBrowser->append(readCmd);
        if(readCmd.contains("ERROR"))
        {
            ui->label->setText("FLASH写入失败");
            return;
        }
        else
        {
            ui->textBrowser->append(tr("##########################FLASH写入硬件版本号成功.###############"));
        }
    }
    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("复位失败");
        return;
    }
    qDebug()<<readCm;
    p.close();
    //判断数据是否正确，不正确就返回
    if(readCm.size()<5)
    {
         ui->textBrowser->append(readCmdMac);
         ui->textBrowser->append(tr("[1]查看是否安装了jlink驱动，连接了jlink并连接了设备......."));
         return;
    }

    readCmd = readCm.mid(15,17);
    //去掉空格
    readCmdMac.resize(12);
    for(int j=0;j<readCmd.size();j++)
    {
        if(readCmd.at(j)!=' ')
        {
            readCmdMac[i++]=(readCmd.at(j));
        }
    }
    readCmdMac[i]='\0';
    //把最后一个字节的那个位改变一下
    qDebug("%x",readCmdMac.data()[10]);
    readCharMac=readCmdMac.data()[10];
    readCharMac=StrToInt(readCharMac);
    qDebug("%x",readCharMac);
    readCharMac|=0xC;
    qDebug("%x",readCharMac);
    readCmdMac.data()[10]=IntToStr(readCharMac);
    qDebug()<<readCmdMac.data()[10];
    qDebug()<<readCmdMac;
    qDebug("%d",readCmdMac.size());
    if(readCmdMac.size()<13)
    {
        ui->textBrowser->append(readCmdMac);
        ui->textBrowser->append(tr("[2]查看是否安装了jlink驱动，连接了jlink并连接了设备......."));
        return;
    }
    QByteArray readCmdMacNew;
    readCmdMacNew.resize(12);//大小修改成12
    readCmdMacNew.data()[0]=readCmdMac.data()[10];
    readCmdMacNew.data()[1]=readCmdMac.data()[11];
    readCmdMacNew.data()[2]=readCmdMac.data()[8];
    readCmdMacNew.data()[3]=readCmdMac.data()[9];
    readCmdMacNew.data()[4]=readCmdMac.data()[6];
    readCmdMacNew.data()[5]=readCmdMac.data()[7];
    readCmdMacNew.data()[6]=readCmdMac.data()[4];
    readCmdMacNew.data()[7]=readCmdMac.data()[5];
    readCmdMacNew.data()[8]=readCmdMac.data()[2];
    readCmdMacNew.data()[9]=readCmdMac.data()[3];
    readCmdMacNew.data()[10]=readCmdMac.data()[0];
    readCmdMacNew.data()[11]=readCmdMac.data()[1];
    //把mac地址保存到文件里面
    QFile file("macAdress.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream( &file );
        stream << readCmdMacNew << "\r\n";
        file.close();
        ui->textBrowser->append(tr("保存MAC地址成功..."));
    }
    else
    {
        ui->textBrowser->append(tr("打开文件失败..."));
    }
    erase++;
    QString s;
    ui->progressBar->setValue(5000);
    //ui->progressBar->setVisible(false);
    ui->textBrowser->append(tr("烧录成功第")+tr("#####[%1]#####").arg(erase)+tr("次")+tr("MAC")+tr("###################[%1]##########################").arg(s.append(readCmdMacNew)));
    {
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->label->setPalette(pa);
    ui->label->setText("烧录成功\r\n"+QString::number(erase, 10));
    }
    //复位
    p.start(RESET_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
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
    p.start("cmd.exe");
    p.write(DEL_TEMP);
    p.write ("exit\n\r");
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    p.close();
}

void MainWindow::on_writeHButton_3_clicked()
{
    QByteArray readCmd;
    QProcess p(0);
    //写入数据
    QString cmdWriteValue="nrfjprog.exe --memwr 0x10001098 --val 0xFF12FF34";
    ui->textBrowser->append(cmdWriteValue);
    qDebug()<<cmdWriteValue;
    p.start(cmdWriteValue);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+= p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    if(readCmd.contains("not erased"))
    {
        ui->textBrowser->append(tr("！！！！！！！！！！！！flash里面已经有值，不能覆盖写入"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("不能重复写入");
    }
    else if(readCmd.contains("ERROR"))
    {
        ui->textBrowser->append(tr("！！！！！！！！！！！！写入出错，请检查。。。。"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("失败");
    }
    else
    {
        ui->textBrowser->append(tr("############写入成功"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("写入成功");
    }
}

void MainWindow::on_readHButton_2_clicked()
{
    //nrfjprog.exe --memrd 0x10001094 --n 8
    QByteArray readCmd;
    QProcess p(0);
    //写入数据
    QString cmdWriteValue="nrfjprog.exe --memrd 0x10001094 --n 4";
    ui->textBrowser->append(cmdWriteValue);
    qDebug()<<cmdWriteValue;
    p.start(cmdWriteValue);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+= p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    qDebug()<<readCmd;
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("读取失败");
    }
    else
    {
        ui->textBrowser->append(tr("########################读取成功"));
        QString stringSuccessShow=readCmd.right(12);
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("读取成功");
    }
}

void MainWindow::on_readHButton_3_clicked()
{
    QByteArray readCmd;
    QProcess p(0);
    //写入数据
    QString cmdWriteValue="nrfjprog.exe --memrd 0x10001098 --n 4";
    ui->textBrowser->append(cmdWriteValue);
    qDebug()<<cmdWriteValue;
    p.start(cmdWriteValue);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+= p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    qDebug()<<readCmd;
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("读取失败");
    }
    else
    {
        ui->textBrowser->append(tr("########################读取成功"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("读取成功");
    }
}

void MainWindow::on_pushButton_5_clicked()
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

void MainWindow::on_writeHButton_2_clicked()
{
    QByteArray macValue=ui->lineEdit->text().right(8).toLatin1();
    qDebug()<<macValue;
    bool ok;
    int macValueInt=macValue.toInt(&ok,16);
    qDebug("%x",macValueInt);
    //调用cmd相关命令
    QByteArray readCmd;
    QProcess p(0);
    //写入数据
    QString cmdWriteValue="nrfjprog.exe --memwr 0x10001094 --val ";
    ui->textBrowser->append(cmdWriteValue+ui->lineEdit->text());
    cmdWriteValue.append(QString::number(macValueInt));
    qDebug()<<cmdWriteValue;
    p.start(cmdWriteValue);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+= p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    if(readCmd.contains("not erased"))
    {
        ui->textBrowser->append(tr("！！！！！！！！！！！！已经写入硬件版本号，不能覆盖写入"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("不能重复写入");
    }
    else if(readCmd.contains("ERROR"))
    {
        ui->textBrowser->append(tr("！！！！！！！！！！！！写入出错，请检查。。。。"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("失败");
    }
    else
    {
        ui->textBrowser->append(tr("############写入成功"));
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("写入成功");
    }
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    ui->lineEdit_2->setEnabled(!checked);
}
