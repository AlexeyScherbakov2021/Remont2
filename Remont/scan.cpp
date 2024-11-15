#include "scan.h"
#include "ui_scan.h"
#include <QSettings>
#include <logwindow.h>

Scan::Scan(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Scan)
{
    ui->setupUi(this);

    oldPortName = Scan::scan.getPort();
    // qDebug() << "Использовался" << oldPortName;
    Scan::scan.close();
    scanPort();

    con = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
    connect(ui->cbCOM, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));
    // qDebug() << "Подключение сигнала в SCAN";

    QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
    QString port = setting.value("COMport").toString();
    ui->cbCOM->setCurrentText(port);

    // qDebug() << "Из реестра" << port;


}

Scan::~Scan()
{
    // qDebug() << "Отключение сигнала в SCAN";
    disconnect(con);
    delete ui;
}

//---------------------------------------------------------------------
// Изменение выбранного порта в списке
//---------------------------------------------------------------------
void Scan::currentIndexChanged(int /*index*/)
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
        // qDebug() << "Проверка открытия" << s;
        if(sp.open(QIODeviceBase::ReadWrite))
        {
            // qDebug() << "Открыт" << s;
            ui->cbCOM->addItem(s);
            sp.close();
            // qDebug() << "Закрыт" << s;
        }
    }
}

//---------------------------------------------------------------------
// Чтение сканера
//---------------------------------------------------------------------
void Scan::slotReadScan(QString s)
{
    LogWindow::AddLine(QString("Передана в слот строка %1.").arg(s));

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

