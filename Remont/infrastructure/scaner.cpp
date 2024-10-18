#include "scaner.h"
#include <qdebug.h>
#include <qmessagebox.h>

Scaner::Scaner()
{
    conn.setBaudRate(QSerialPort::Baud9600);
    conn.setDataBits(QSerialPort::Data8);
    conn.setStopBits(QSerialPort::OneStop);
    conn.setParity(QSerialPort::NoParity);

    connect(&conn,SIGNAL(errorOccurred(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&conn, SIGNAL(readyRead()), SLOT(slotRead()));

}

bool Scaner::open(QString port)
{
    conn.setPortName(port);
    return conn.open(QIODeviceBase::ReadWrite);
}

void Scaner::close()
{
    conn.close();
}

QString Scaner::getPort()
{
    return conn.portName();
}

void Scaner::slotRead()
{
    QByteArray bytes = conn.readAll();
    QString s(bytes.removeLast());

    // qDebug() << s;

    emit sigRead(s);

}

void Scaner::handleError(QSerialPort::SerialPortError error)
{
    // qDebug() << "Произошла ошибка." << error;
    // if(error != QSerialPort::NoError)
    //     QMessageBox::critical(0, "Ошибка", QString("Произошла ошибка порта %2 '%1'\nСканер работать не будет.").arg(conn.errorString()).arg(conn.portName()));
}
