#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    char IntToStr(char aChar);
    char StrToInt(char aChar);
    ~MainWindow();
    #define ALL_IN_ONE_CMD_1 "mergehex.exe -m nrf52832_xxaa_s132_bootloader.hex s132_nrf52_2.0.0_softdevice.hex -o temp.hex"
    #define ALL_IN_ONE_CMD_2 "mergehex.exe -m temp.hex nrf52832_xxaa_s132.hex -o ts102.hex"
    #define ERASE_ALL_CMD "nrfjprog.exe -f NRF52 --eraseall"
    #define DOWNLOAD_CMD "nrfjprog.exe -f NRF52 --program ts102.hex --verify"
    #define RESET_CMD "nrfjprog.exe -f NRF52 --reset"
    #define DEL_TEMP "del temp.hex\n\r"
    #define READ_MAC_CMD "nrfjprog.exe --memrd 0x100000a3 --n 7"
    Ui::MainWindow *ui;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_writeHButton_3_clicked();

    void on_readHButton_2_clicked();

    void on_readHButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_writeHButton_2_clicked();

    void on_checkBox_3_clicked(bool checked);

private:
};

#endif // MAINWINDOW_H
