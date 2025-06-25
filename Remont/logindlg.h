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
    inline static Users CurrUser;


private slots:
    void on_pbOK_clicked();

    void on_cbUsers_currentIndexChanged(int index);

private:
    Ui::LoginDlg *ui;
    RepoMSSQL repo;
    QList<Users> listUsers;

};

#endif // LOGINDLG_H
