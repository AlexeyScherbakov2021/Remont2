#ifndef SCANER_H
#define SCANER_H

#include <QObject>
#include <qserialport.h>

class Scaner : public QObject
{
    Q_OBJECT

public:
    Scaner();
    ~Scaner() { if(conn.isOpen()) conn.close(); }

    bool open(QString port);
    void close();
    QString getPort();

private:
    QSerialPort conn;

private slots:
    void slotRead();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void sigRead(QString s);

};


#endif // SCANER_H
