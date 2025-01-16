#include "scaner.h"
#include <logwindow.h>
#include <qdebug.h>
#include <qmessagebox.h>

Scaner::Scaner()
{
    conn.setBaudRate(QSerialPort::Baud9600);
    conn.setDataBits(QSerialPort::Data8);
    conn.setStopBits(QSerialPort::OneStop);
    conn.setParity(QSerialPort::NoParity);
    // conn.setFlowControl();

    connect(&conn, SIGNAL(readyRead()), SLOT(slotRead()));
    connect(&conn,SIGNAL(errorOccurred(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
}

bool Scaner::open(QString port)
{
    blockSignals(false);
    conn.setPortName(port);
    bool res = conn.open(QIODeviceBase::ReadWrite);
    LogWindow::AddLine(QString("Открыт порт %1.").arg(port));

    LogWindow::AddLine(QString("%1 бод, %2 бит, %3 стоп, %4 четность, %5 поток")
                           .arg(conn.baudRate())
                           .arg(conn.dataBits())
                           .arg(conn.stopBits())
                           .arg(conn.parity())
                           .arg(conn.flowControl()));


    // qDebug() << "Открытие и подключение сигналов." << port << res;
    return res;
}

void Scaner::close()
{
    if(conn.isOpen())
    {
        conn.close();
        blockSignals(true);
        // qDebug() << "Закрытие и откл. сигналов" << getPort();
    }
}

QString Scaner::getPort()
{
    return conn.portName();
}


void Scaner::slotRead()
{

    QByteArray bytes = conn.readAll();

    if(bytes.endsWith('\r'))
    {
        bytes.removeLast();

        LogWindow::AddLine(QString("Слот чтения. Получена строка %1.").arg(bytes));
        QString s(bytes);
        s = readLine + s;
        readLine.clear();
        // s = s.removeLast();
    // qDebug() << s;

        emit sigRead(s);
    }
    else
        readLine += QString(bytes);
}

void Scaner::handleError(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        LogWindow::AddLine(QString("Ошибка %2 %1.").arg(conn.errorString()).arg(getPort()));
        qDebug() << "Слот ошибки"  << getPort() << conn.errorString();
    }

    // qDebug() << "Произошла ошибка." << error;
    // if(error != QSerialPort::NoError)
    //     QMessageBox::critical(0, "Ошибка", QString("Произошла ошибка порта %2 '%1'\nСканер работать не будет.").arg(conn.errorString()).arg(conn.portName()));
}
