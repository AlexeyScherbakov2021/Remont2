#include "logindlg.h"
#include "ui_logindlg.h"

#include <QSettings>

LoginDlg::LoginDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

    repo.LoadUsers(listUsers);
    Users adm;
    adm.UserName = "Admin";
    adm.id = 100500;
    adm.UserFullName = adm.UserName;
    adm.Pass = "789";
    listUsers.push_back(adm);

    QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
    int idUser = setting.value("User", 0).toInt();

    int selectRow = -1;
    int row = 0;
    for(auto &it : listUsers)
    {
        ui->cbUsers->addItem(it.UserName + " (" + it.UserFullName + ")", it.id);
        if(idUser == it.id)
            selectRow = row;

        ++row;
    }

    ui->cbUsers->setCurrentIndex(selectRow);
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
LoginDlg::~LoginDlg()
{
    delete ui;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void LoginDlg::on_pbOK_clicked()
{
    int row = ui->cbUsers->currentIndex();
    idUser = listUsers[row].id;
    if(ui->lePass->text() == listUsers[row].Pass)
    {
        QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
        setting.setValue("User", idUser);
        accept();
    }
    else
    {
        ui->lePass->setFocus();
        ui->lePass->setStyleSheet("border-color: red; border-width: 2px;border-style: solid;");
    }
}


void LoginDlg::on_cbUsers_currentIndexChanged(int index)
{
    ui->lePass->setFocus();
}

