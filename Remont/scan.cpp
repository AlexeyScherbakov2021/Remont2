#include "scan.h"
#include "ui_scan.h"
#include <QSettings>

Scan::Scan(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Scan)
{
    ui->setupUi(this);

    oldPortName = Scan::scan.getPort();
    Scan::scan.close();
    scanPort();
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
    QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
    QString port = setting.value("COMport").toString();
    ui->cbCOM->setCurrentText(port);

}

Scan::~Scan()
{
    delete ui;
}

//---------------------------------------------------------------------
// Изменение выбранного порта в списке
//---------------------------------------------------------------------
void Scan::on_cbCOM_currentIndexChanged(int /*index*/)
{
    ui->lbResult->clear();
    Scan::scan.close();
    Scan::scan.open(ui->cbCOM->currentText());
}

//---------------------------------------------------------------------
// Сканирование открытых портов
//---------------------------------------------------------------------
void Scan::scanPort()
{
    QSerialPort sp;

    for(int i = 1; i < 255; i++)
    {
        QString s = QString("COM%1").arg(i);
        sp.setPortName(s);
        if(sp.open(QIODeviceBase::ReadWrite))
        {
            ui->cbCOM->addItem(s);
            sp.close();
        }
    }
}

//---------------------------------------------------------------------
// Чтение сканера
//---------------------------------------------------------------------
void Scan::slotReadScan(QString s)
{
    ui->lbResult->setText(ui->lbResult->text() + " " + s);
}


//---------------------------------------------------------------------
// Закрыть с отменой
//---------------------------------------------------------------------
void Scan::on_pbClose_clicked()
{
    Scan::scan.close();
    Scan::scan.open(oldPortName);
    reject();
}


//---------------------------------------------------------------------
// Закрыть с сохранением
//---------------------------------------------------------------------
void Scan::on_pbOK_clicked()
{
    QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
    // qDebug() << setting.isWritable();
    // qDebug() << setting.value("COMport").toString();
    setting.setValue("COMport", ui->cbCOM->currentText());
    // qDebug() << setting.value("COMport").toString();
    accept();
}

