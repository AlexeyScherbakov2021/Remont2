#ifndef SCAN_H
#define SCAN_H

#include <QDialog>

#include <infrastructure/scaner.h>

namespace Ui {
class Scan;
}

class Scan : public QDialog
{
    Q_OBJECT

public:
    explicit Scan(QWidget *parent = nullptr);
    ~Scan();

    inline static Scaner scan;

private slots:
    void currentIndexChanged(int index);

private:
    Ui::Scan *ui;
    void scanPort();
    QString oldPortName;
    QMetaObject::Connection con;

private slots:
    void slotReadScan(QString s);
    void on_pbClose_clicked();
    void on_pbOK_clicked();
};

#endif // SCAN_H
