#ifndef LOGINDLG_H
#define LOGINDLG_H

#include "repomssql.h"

#include <QDialog>

#include <infrastructure/users.h>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

    int idUser;

private slots:
    void on_pbOK_clicked();

private:
    Ui::LoginDlg *ui;
    RepoMSSQL repo;
    QList<Users> listUsers;

};

#endif // LOGINDLG_H
