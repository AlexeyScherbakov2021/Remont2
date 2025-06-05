#ifndef STATUSDLG_H
#define STATUSDLG_H

#include <QDialog>
#include <infrastructure/IStatus.h>

namespace Ui {
class StatusDlg;
}

class StatusDlg : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDlg(Status& stat, QWidget *parent = nullptr);
    ~StatusDlg();

private slots:
    void on_pbOK_clicked();

private:
    Ui::StatusDlg *ui;
    Status *status;
    QMap<QString, int> listStatus;
};

#endif // STATUSDLG_H
