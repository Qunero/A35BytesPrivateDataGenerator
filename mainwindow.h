#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void iSetDefaultDataForAll();

private slots:

    bool closeFile();
    long convertHexStrToLong(QString str);
    QString convertDecStrToHex(QString str, int groups);
    void iSetDefaultCheckBox();
    void iSetDefaultLineEdit();

    void on_checkBox_1_toggled(bool checked);
    void on_checkBox_2_toggled(bool checked);
    void on_checkBox_3_toggled(bool checked);
    void on_checkBox_5_toggled(bool checked);
    void on_checkBox_6_toggled(bool checked);
    void on_checkBox_7_toggled(bool checked);
    void on_checkBox_8_toggled(bool checked);
    void on_checkBox_9_toggled(bool checked);
    void on_checkBox_10_toggled(bool checked);
    void on_checkBox_12_toggled(bool checked);

    void on_comboBox_4_currentIndexChanged(int index);
    void on_comboBox_11_currentIndexChanged(int index);

    void on_lineEdit_1_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);
    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_lineEdit_7_textChanged(const QString &arg1);
    void on_lineEdit_8_textChanged(const QString &arg1);
    void on_lineEdit_9_textChanged(const QString &arg1);
    void on_lineEdit_10_textChanged(const QString &arg1);
    void on_lineEdit_12_textChanged(const QString &arg1);

    void on_pushButton_default_clicked();
    void on_pushButton_generate_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_saveas_clicked();

    bool openFile();
    bool parsePrivateData(QStringList sl);
    bool process_line(QString line);

    bool saveFile(bool bSaveas=false);
    bool selectFile(bool bForSave=false);
    void showAboutMessageBox();
    void showHelpMessageBox();
    bool showPrivateData();

private:
    bool bHaveSelectedFile;
    QRegExpValidator * _pHexStrValidator;
    QRegExpValidator * _pDecStrValidator;
    QString strFileName;
    QString strManufactoryID;
    QString strFrequency;
    QString strSymRate;
    QString strQam;
    QString strPid;
    QString strStartSerialNo;
    QString strEndSerialNo;
    QString strSoftVer;
    QString strHardwareVer;
    QString strHwControl;
    QString strOtaMode;
    QString strSerialControl;
    QString strResult;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
