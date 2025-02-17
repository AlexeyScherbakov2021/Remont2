#include "setterdlg.h"
#include "settereditdlg.h"
#include "ui_setterdlg.h"

SetterDlg::SetterDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetterDlg)
{
    ui->setupUi(this);

    model = new SetterModel(this);
    model->prepareLoad("", true);
    ui->tableView->setModel(model);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
SetterDlg::~SetterDlg()
{
    delete ui;
}

//-------------------------------------------------------------------------
// Кнопка поиска
//-------------------------------------------------------------------------
void SetterDlg::on_tbSearch_clicked()
{
    model->prepareLoad(ui->leSearch->text(), true);
}


//-------------------------------------------------------------------------
// Кнопка Новый
//-------------------------------------------------------------------------
void SetterDlg::on_pbNew_clicked()
{

}


//-------------------------------------------------------------------------
// Кнопка удалить
//-------------------------------------------------------------------------
void SetterDlg::on_pbDelete_clicked()
{

}

//-------------------------------------------------------------------------
// Кнопка Редактировать
//-------------------------------------------------------------------------
void SetterDlg::on_pbEdit_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index != QModelIndex())
    {
        on_tableView_doubleClicked(index);
    }

}

//-------------------------------------------------------------------------
// Двойной щелчок
//-------------------------------------------------------------------------
void SetterDlg::on_tableView_doubleClicked(const QModelIndex &index)
{
    SetterOut *setter = model->GetItem(index.row());

    SetterEditDlg *win = new SetterEditDlg(setter, this);
    if(win->exec() == QDialog::Accepted)
    {

    }
}

