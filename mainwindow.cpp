#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lBitStatusForDisableGenerator = 0;
    _pHexStrValidator = new QRegExpValidator(QRegExp("(?:[_\\dA-Fa-f]{2})(?::[_\\dA-Fa-f]{2}){0,3}"), this);
    _pDecStrValidator = new QRegExpValidator(QRegExp("\\d{1,10}"), this); // 0xffffffff==4294967295,10numbers
    strInputMaskHex1 = ">HH;_";
    strInputMaskHex2 = ">HH:HH;_";
    strInputMaskHex4 = ">HH:HH:HH:HH;_";
    bHaveSelectedFile = false;
    strMsgAboutSoftware =
            "          PrivateDataGenerator V" APP_REVISION "      \n"
            "This software is for generating Private Data that\n"
            "follows the COSHIP OTC Standard.\n"
            "-------------------------------------------------\n"
            "********   Author: QuNengrong (Neo Qu)   ********\n"
            "********   Email : Quner612#qq.com       ********\n"
            "-------------------------------------------------\n"
            "Copyright (c) 2013, Coship, All rights reserved.";
    strMsgHelpDetail =
            "        PrivateDataGenerator User Guide        \n"
            "1. Click 'Default' or 'Open' to get basic data;\n"
            "2. Edit the data as you wish\n"
            "3. Click 'Generate' to get your private data\n"
            "4. Copy the private data, or Save it to a file\n"
            "-------------------------------------------------\n"
            "        PrivateDataGenerator Edit Tips        \n"
            "A). Hex data only accept data like this: ##, ##:##,\n"
            "   ##:##:##:##, where, # stands for hex numbers;\n"
            "B). While edit, you may uncheck the CheckBox in the\n"
            "   Same line, to see the Decimal value of the line.\n"
            "-------------------------------------------------\n"
            "********   Author: QuNengrong (Neo Qu)   ********\n"
            "********   Email : Quner612#qq.com       ********\n"
            "-------------------------------------------------\n"
            "Copyright (c) 2013, Coship, All rights reserved.";
    QObject::connect(this->ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
    QObject::connect(this->ui->actionClose,SIGNAL(triggered()),this,SLOT(closeFile()));
    QObject::connect(this->ui->actionHelp,SIGNAL(triggered()),this,SLOT(showHelpMessageBox()));
    QObject::connect(this->ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAboutMessageBox()));

    // set default status
    iSetDefaultDataForAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::closeFile()
{
    if (!bHaveSelectedFile)
    {
        // nothing to close
        return true;
    }
    qDebug() << "try close Ts file...";

    // confirm close
    if (QMessageBox::Cancel ==
            QMessageBox::warning(this, "Confirm close File",
                                 "Are you really want to close the current file?",
                                 QMessageBox::Ok|QMessageBox::Cancel,
                                 QMessageBox::Cancel))
    {
        return false;
    }

    // set status
    this->bHaveSelectedFile = false;

    // clean up data
    iSetDefaultDataForAll();

    return true;
}

QString MainWindow::convertDecStrToHex(QString str, int groups)
{
    QString strHex;
    QString f;
    long value = str.toLong(0, 10);
    //qDebug() << "str =>" << str << ", groups =>" << groups;
    f = f.sprintf("%%0%dX", groups*2);
    strHex = strHex.sprintf(f.toLatin1().data(), value);

    // add some :
    int padding = (strHex.length()-1) / 2;
    int result_len = strHex.length() + padding;
    int i = 0;
    value = 1; // indicate the ':' already added
    for(i=2; i<result_len;)
    {
        strHex.insert(i, ':');
        //qDebug() << "in convertDecStrToHex: i="<< i << "strHex =>" << strHex;
        i += 3;
    }
    //qDebug() << "in convertDecStrToHex: str => strHex:" << str << "=>" << strHex;
    return strHex;
}

/*
 * input string format: 1F:45
 * result long value  : (0x1F << 8) + 0x45
*/
long MainWindow::convertHexStrToLong(QString str)
{
    long result = 0;
    bool bNeedShift = false;
    QString s;
    QStringList sl = str.split(':');
    foreach(s, sl)
    {
        if (bNeedShift)
        {
            result = result<<8;
        }

        result += s.toInt(0, 16);
        //qDebug() << s << " => " << s.toInt(0, 16);
        //qDebug() << "result" << result;
        bNeedShift = true;
    }

    //qDebug() << "return " << result;
    return result;
}

void MainWindow::iSetDefaultCheckBox()
{
    ui->checkBox_1->setChecked(true);
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setChecked(true);
    ui->checkBox_5->setChecked(true);
    ui->checkBox_6->setChecked(true);
    ui->checkBox_7->setChecked(true);
    ui->checkBox_8->setChecked(true);
    ui->checkBox_9->setChecked(true);
    ui->checkBox_10->setChecked(true);
    ui->checkBox_12->setChecked(true);
    emit ui->checkBox_1->toggled(true);
    emit ui->checkBox_2->toggled(true);
    emit ui->checkBox_3->toggled(true);
    emit ui->checkBox_5->toggled(true);
    emit ui->checkBox_6->toggled(true);
    emit ui->checkBox_7->toggled(true);
    emit ui->checkBox_8->toggled(true);
    emit ui->checkBox_9->toggled(true);
    emit ui->checkBox_10->toggled(true);
    emit ui->checkBox_12->toggled(true);
}

void MainWindow::iSetDefaultDataForAll()
{
    //qDebug() << "in iSetDefaultDataForAll...";
    iSetDefaultCheckBox();
    iSetDefaultLineEdit();
    showPrivateData();
}

void MainWindow::iSetDefaultLineEdit()
{
    strManufactoryID = "00:00:00:88";
    strFrequency = "00:06:22:50";
    strSymRate = "00:00:1A:DB";
    strQam = "00:05";
    strPid = "1F:45";
    strStartSerialNo = "00:00:00:00";
    strEndSerialNo = "00:00:00:00";
    strSoftVer = "00:00:00:01";
    strHardwareVer = "00:00:00:63";
    strHwControl = "00";
    strOtaMode = "01";
    strSerialControl = "00";
    strResult.clear();
}

void MainWindow::on_checkBox_1_toggled(bool checked)
{
    //qDebug() << "Checked status:" << checked;
    ui->lineEdit_1->clear(); // clear it, in case old InputMask cause textChanged()
    //qDebug() << "Cleared data: ui->lineEdit_1->text() =>" << ui->lineEdit_1->text();
    if (checked)
    {
        // show hex
        strManufactoryID = convertDecStrToHex(strManufactoryID, 4);
        ui->lineEdit_1->setValidator(_pHexStrValidator);
        ui->lineEdit_1->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strManufactoryID = QString::number(convertHexStrToLong(strManufactoryID));
        ui->lineEdit_1->setValidator(_pDecStrValidator);
        ui->lineEdit_1->setInputMask("");
        setGeneratorStatusByBite(1);
    }

    //Notice, should set content first, then call setInputMask
    //qDebug() << "update strManufactoryID =>" << strManufactoryID;
    ui->lineEdit_1->setText(strManufactoryID);
}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    ui->lineEdit_2->clear();
    if (checked)
    {
        // show hex
        strFrequency = convertDecStrToHex(strFrequency, 4);
        ui->lineEdit_2->setValidator(_pHexStrValidator);
        ui->lineEdit_2->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strFrequency = QString::number(convertHexStrToLong(strFrequency));
        ui->lineEdit_2->setValidator(_pDecStrValidator);
        ui->lineEdit_2->setInputMask("");
        setGeneratorStatusByBite(2);
    }
    ui->lineEdit_2->setText(strFrequency);
}

void MainWindow::on_checkBox_3_toggled(bool checked)
{
    ui->lineEdit_3->clear();
    if (checked)
    {
        // show hex
        strSymRate = convertDecStrToHex(strSymRate, 4);
        ui->lineEdit_3->setValidator(_pHexStrValidator);
        ui->lineEdit_3->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strSymRate = QString::number(convertHexStrToLong(strSymRate));
        ui->lineEdit_3->setValidator(_pDecStrValidator);
        ui->lineEdit_3->setInputMask("");
        setGeneratorStatusByBite(3);
    }
    ui->lineEdit_3->setText(strSymRate);
}

void MainWindow::on_checkBox_5_toggled(bool checked)
{
    ui->lineEdit_5->clear();
    if (checked)
    {
        // show hex
        strPid = convertDecStrToHex(strPid, 2);
        ui->lineEdit_5->setValidator(_pHexStrValidator);
        ui->lineEdit_5->setInputMask(strInputMaskHex2);
    }
    else
    {
        // show dec
        strPid = QString::number(convertHexStrToLong(strPid));
        ui->lineEdit_5->setValidator(_pDecStrValidator);
        ui->lineEdit_5->setInputMask("");
        setGeneratorStatusByBite(5);
    }
    ui->lineEdit_5->setText(strPid);
}

void MainWindow::on_checkBox_6_toggled(bool checked)
{
    ui->lineEdit_6->clear();
    if (checked)
    {
        // show hex
        strStartSerialNo = convertDecStrToHex(strStartSerialNo, 4);
        ui->lineEdit_6->setValidator(_pHexStrValidator);
        ui->lineEdit_6->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strStartSerialNo = QString::number(convertHexStrToLong(strStartSerialNo));
        ui->lineEdit_6->setValidator(_pDecStrValidator);
        ui->lineEdit_6->setInputMask("");
        setGeneratorStatusByBite(6);
    }
    ui->lineEdit_6->setText(strStartSerialNo);
}

void MainWindow::on_checkBox_7_toggled(bool checked)
{
    ui->lineEdit_7->clear();
    if (checked)
    {
        // show hex
        strEndSerialNo = convertDecStrToHex(strEndSerialNo, 4);
        ui->lineEdit_7->setValidator(_pHexStrValidator);
        ui->lineEdit_7->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strEndSerialNo = QString::number(convertHexStrToLong(strEndSerialNo));
        ui->lineEdit_7->setValidator(_pDecStrValidator);
        ui->lineEdit_7->setInputMask("");
        setGeneratorStatusByBite(7);
    }
    ui->lineEdit_7->setText(strEndSerialNo);
}

void MainWindow::on_checkBox_8_toggled(bool checked)
{
    ui->lineEdit_8->clear();
    if (checked)
    {
        // show hex
        strSoftVer = convertDecStrToHex(strSoftVer, 4);
        ui->lineEdit_8->setValidator(_pHexStrValidator);
        ui->lineEdit_8->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strSoftVer = QString::number(convertHexStrToLong(strSoftVer));
        ui->lineEdit_8->setValidator(_pDecStrValidator);
        ui->lineEdit_8->setInputMask("");
        setGeneratorStatusByBite(8);
    }
    ui->lineEdit_8->setText(strSoftVer);
}

void MainWindow::on_checkBox_9_toggled(bool checked)
{
    ui->lineEdit_9->clear();
    if (checked)
    {
        // show hex
        strHardwareVer = convertDecStrToHex(strHardwareVer, 4);
        ui->lineEdit_9->setValidator(_pHexStrValidator);
        ui->lineEdit_9->setInputMask(strInputMaskHex4);
    }
    else
    {
        // show dec
        strHardwareVer = QString::number(convertHexStrToLong(strHardwareVer));
        ui->lineEdit_9->setValidator(_pDecStrValidator);
        ui->lineEdit_9->setInputMask("");
        setGeneratorStatusByBite(9);
    }
    ui->lineEdit_9->setText(strHardwareVer);
}

void MainWindow::on_checkBox_10_toggled(bool checked)
{
    ui->lineEdit_10->clear();
    if (checked)
    {
        // show hex
        strHwControl = convertDecStrToHex(strHwControl, 1);
        ui->lineEdit_10->setValidator(_pHexStrValidator);
        ui->lineEdit_10->setInputMask(strInputMaskHex1);
    }
    else
    {
        // show dec
        strHwControl = QString::number(convertHexStrToLong(strHwControl));
        ui->lineEdit_10->setValidator(_pDecStrValidator);
        ui->lineEdit_10->setInputMask("");
        setGeneratorStatusByBite(10);
    }
    ui->lineEdit_10->setText(strHwControl);
}

void MainWindow::on_checkBox_12_toggled(bool checked)
{
    ui->lineEdit_12->clear();
    if (checked)
    {
        // show hex
        strSerialControl = convertDecStrToHex(strSerialControl, 1);
        ui->lineEdit_12->setValidator(_pHexStrValidator);
        ui->lineEdit_12->setInputMask(strInputMaskHex1);
    }
    else
    {
        // show dec
        strSerialControl = QString::number(convertHexStrToLong(strSerialControl));
        ui->lineEdit_12->setValidator(_pDecStrValidator);
        ui->lineEdit_12->setInputMask("");
        setGeneratorStatusByBite(12);

    }
    ui->lineEdit_12->setText(strSerialControl);
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    strQam = strQam.sprintf("00:%02X", index+1);
    ui->lineEdit_result->clear();
    //qDebug() << "new strQam =>" << strQam;
}

void MainWindow::on_comboBox_11_currentIndexChanged(int index)
{
    strOtaMode = strOtaMode.sprintf("%02X", index);
    ui->lineEdit_result->clear();
    //qDebug() << "new strOtaMode =>" << strOtaMode;
}

void MainWindow::on_lineEdit_1_textChanged(const QString &arg1)
{
    if (ui->checkBox_1->checkState() == Qt::Checked)
    {
        //qDebug() << "textChanged, arg1 =>" << arg1;
        //qDebug() << "ui->lineEdit_1->inputMask() =>" << ui->lineEdit_1->inputMask();
        //qDebug() << "textChanged, lineEdit_1->text() =>" << ui->lineEdit_1->text();
        if (ui->lineEdit_1->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strManufactoryID = ui->lineEdit_1->text();
            clearGeneratorStatusByBite(1);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(1);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strManufactoryID = ui->lineEdit_1->text();
    }
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (ui->checkBox_2->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_2->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strFrequency = ui->lineEdit_2->text();
            clearGeneratorStatusByBite(2);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(2);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strFrequency = ui->lineEdit_2->text();
    }
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (ui->checkBox_3->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_3->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strSymRate = ui->lineEdit_3->text();
            clearGeneratorStatusByBite(3);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(3);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strSymRate = ui->lineEdit_3->text();
    }
}

void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    if (ui->checkBox_5->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_5->hasAcceptableInput()
                && arg1.length() == 5)
        {
            strPid = ui->lineEdit_5->text();
            clearGeneratorStatusByBite(5);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(5);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strPid = ui->lineEdit_5->text();
    }
}

void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    if (ui->checkBox_6->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_6->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strStartSerialNo = ui->lineEdit_6->text();
            clearGeneratorStatusByBite(6);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(6);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strStartSerialNo = ui->lineEdit_6->text();
    }
}

void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    if (ui->checkBox_7->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_7->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strEndSerialNo = ui->lineEdit_7->text();
            clearGeneratorStatusByBite(7);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(7);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strEndSerialNo = ui->lineEdit_7->text();
    }
}

void MainWindow::on_lineEdit_8_textChanged(const QString &arg1)
{
    if (ui->checkBox_8->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_8->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strSoftVer = ui->lineEdit_8->text();
            clearGeneratorStatusByBite(8);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(8);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strSoftVer = ui->lineEdit_8->text();
    }
}

void MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{
    if (ui->checkBox_9->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_9->hasAcceptableInput()
                && arg1.length() == 11)
        {
            strHardwareVer = ui->lineEdit_9->text();
            clearGeneratorStatusByBite(9);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(9);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strHardwareVer = ui->lineEdit_9->text();
    }
}

void MainWindow::on_lineEdit_10_textChanged(const QString &arg1)
{
    if (ui->checkBox_10->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_10->hasAcceptableInput()
                && arg1.length() == 2)
        {
            strHwControl = ui->lineEdit_10->text();
            clearGeneratorStatusByBite(10);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(10);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strHwControl = ui->lineEdit_10->text();
    }
}

void MainWindow::on_lineEdit_12_textChanged(const QString &arg1)
{
    if (ui->checkBox_12->checkState() == Qt::Checked)
    {
        if (ui->lineEdit_12->hasAcceptableInput()
                && arg1.length() == 2)
        {
            strSerialControl = ui->lineEdit_12->text();
            clearGeneratorStatusByBite(12);
        }
        else
        {
            // no valid data
            ui->lineEdit_result->clear();
            setGeneratorStatusByBite(12);
        }
    }
    else
    {
        if (arg1.isEmpty() || arg1.contains(':'))
        {
            // it's not tigger by editting the decimal number
            return;
        }
        strSerialControl = ui->lineEdit_12->text();
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    closeFile();
}

void MainWindow::on_pushButton_default_clicked()
{
    iSetDefaultDataForAll();
}


void MainWindow::on_pushButton_generate_clicked()
{
    strResult = strManufactoryID + ':'
            + strFrequency + ':'
            + strSymRate + ':'
            + strQam + ':'
            + strPid + ':'
            + strStartSerialNo + ':'
            + strEndSerialNo + ':'
            + strSoftVer + ':'
            + strHardwareVer + ':'
            + strHwControl + ':'
            + strOtaMode + ':'
            + strSerialControl;
    this->ui->lineEdit_result->setText(strResult);

    ui->statusBar->showMessage("Your private data generated successfully!!! Copy or Save...", 5000);
}

void MainWindow::on_pushButton_open_clicked()
{
    openFile();
}

void MainWindow::on_pushButton_save_clicked()
{
    saveFile();
}

void MainWindow::on_pushButton_saveas_clicked()
{
    saveFile(true);
}

bool MainWindow::openFile()
{
    // check status
    if (bHaveSelectedFile)
    {
        if (!closeFile())
        {
            // close failed, cancel open new file
            return false;
        }
    }

    selectFile();

    qDebug() << "Selected File:" << strFileName;
    QFile dataFile(strFileName);
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&dataFile);
    if (!in.atEnd())
    {
        QString line = in.readLine();
        if (!process_line(line))
        {
            dataFile.close();
            return false;
        }
    }

    ui->statusBar->showMessage("Opened file: "+strFileName, 5000);
    showPrivateData();

    return true;
}

bool MainWindow::process_line(QString line)
{
    QStringList slData = line.split(':');
    if (slData.count() != 35)
    {
        QMessageBox::critical(this, "Reading ERROR",
                              "Please check your file format",
                              QMessageBox::Ok);
        qDebug() << "number of data contained: " << slData.count();

        // clear file status
        bHaveSelectedFile = false;
        return false;
    }

    iSetDefaultCheckBox();
    parsePrivateData(slData);

    return true;
}

bool MainWindow::parsePrivateData(QStringList sl)
{
    strManufactoryID = sl.at(0) + ':' + sl.at(1) + ':' + sl.at(2) + ':' + sl.at(3);
    strFrequency = sl.at(4) + ':' + sl.at(5) + ':' + sl.at(6) + ':' + sl.at(7);
    strSymRate = sl.at(8) + ':' + sl.at(9) + ':' + sl.at(10) + ':' + sl.at(11);
    strQam = sl.at(12) + ':' + sl.at(13);
    strPid = sl.at(14) + ':' + sl.at(15);
    strStartSerialNo = sl.at(16) + ':' + sl.at(17) + ':' + sl.at(18) + ':' + sl.at(19);
    strEndSerialNo = sl.at(20) + ':' + sl.at(21) + ':' + sl.at(22) + ':' + sl.at(23);
    strSoftVer = sl.at(24) + ':' + sl.at(25) + ':' + sl.at(26) + ':' + sl.at(27);
    strHardwareVer = sl.at(28) + ':' + sl.at(29) + ':' + sl.at(30) + ':' + sl.at(31);
    strHwControl = sl.at(32);
    strOtaMode = sl.at(33);
    strSerialControl = sl.at(34);

    return true;
}

bool MainWindow::saveFile(bool bSaveas)
{
    if (bSaveas)
    {
        if(! selectFile(true))
        {
            return false;
        }
    }
    else if (!bHaveSelectedFile)
    {
        if(! selectFile())
        {
            return false;
        }
    }

    QFile dataFile(strFileName);
    if (!dataFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&dataFile);
    out << strResult;

    dataFile.close();
    ui->statusBar->showMessage("Saved to file: "+strFileName, 5000);

    return true;
}

bool MainWindow::selectFile(bool bForSave)
{
    qDebug() << "try select file...";
    if (bForSave)
    {
        strFileName = QFileDialog::getSaveFileName(this, tr("Save Your Pravate data to File"),
                                                   ".",
                                                   tr("Text File (*.txt *.text);;All(*.*)"));
    }
    else
    {
        strFileName = QFileDialog::getOpenFileName(this, tr("Open Your Pravate data File"),
                                                   ".",
                                                   tr("Text File (*.txt *.text);;All(*.*)"));
    }
    if (strFileName.isNull())
    {
        return false;
    }

    if (bForSave && (! strFileName.endsWith(".txt")))
    {
        // add default suffix
        strFileName += ".txt";
    }

    this->bHaveSelectedFile = true;

    return true;
}

void MainWindow::showAboutMessageBox()
{
    QMessageBox::about(this,
                       "About PrivateDataGenerator",
                       strMsgAboutSoftware);
}

void MainWindow::showHelpMessageBox()
{
    QMessageBox::about(this,
                       "PrivateDataGenerator Help",
                       strMsgHelpDetail);
}

bool MainWindow::showPrivateData()
{
    this->ui->lineEdit_1->setText(strManufactoryID);
    this->ui->lineEdit_2->setText(strFrequency);
    this->ui->lineEdit_3->setText(strSymRate);
    this->ui->comboBox_4->setCurrentIndex(convertHexStrToLong(strQam)-1);
    this->ui->lineEdit_5->setText(strPid);
    this->ui->lineEdit_6->setText(strStartSerialNo);
    this->ui->lineEdit_7->setText(strEndSerialNo);
    this->ui->lineEdit_8->setText(strSoftVer);
    this->ui->lineEdit_9->setText(strHardwareVer);
    this->ui->lineEdit_10->setText(strHwControl);
    this->ui->comboBox_11->setCurrentIndex(convertHexStrToLong(strOtaMode));
    this->ui->lineEdit_12->setText(strSerialControl);
    this->ui->lineEdit_result->setText(strResult);

    return true;
}

void MainWindow::setGeneratorStatusByBite(int bit)
{
    if (bit<0 || bit>24)
    {
        return;
    }
    lBitStatusForDisableGenerator |= (1U<<(bit));
    ui->pushButton_generate->setEnabled(false);
    ui->label_notice->setText("[Useful Tips]\n1. Don't leave blank data;\n2. Check all the checkbox(beside 'Hex');");
}

void MainWindow::clearGeneratorStatusByBite(int bit)
{
    if (bit<0 || bit>24)
    {
        return;
    }
    lBitStatusForDisableGenerator &= ~(1U<<(bit));
    if (!lBitStatusForDisableGenerator)
    {
        ui->pushButton_generate->setEnabled(true);
        ui->label_notice->setText("[Status Ready]\nAll data seems correct,Click <Generator> to get your data!!!");
    }
}

void MainWindow::on_lineEdit_result_selectionChanged()
{
    if ((!ui->lineEdit_result->text().isEmpty())
            && ui->lineEdit_result->selectedText() == ui->lineEdit_result->text())
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ui->lineEdit_result->text());
        ui->statusBar->showMessage("Your private data has been copyed :)", 5000);
    }
}
