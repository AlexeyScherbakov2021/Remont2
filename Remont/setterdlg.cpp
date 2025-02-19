#include "setterdlg.h"
#include "settereditdlg.h"
#include "ui_setterdlg.h"

#include <QMessageBox>

SetterDlg::SetterDlg(bool isSelect, bool _isFree, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetterDlg), isFree(_isFree)
{
    ui->setupUi(this);

    if(!isSelect)
    {
        ui->pbSelect->setVisible(false);
    }

    model = new SetterModel(this);
    model->prepareLoad("", isFree);
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
    model->prepareLoad(ui->leSearch->text(), isFree);
}


//-------------------------------------------------------------------------
// Кнопка Новый
//-------------------------------------------------------------------------
void SetterDlg::on_pbNew_clicked()
{
    SetterOut setter;
    SetterEditDlg *win = new SetterEditDlg(&setter, this);
    if(win->exec() == QDialog::Accepted)
    {
        model->AddItem(&setter);
    }
}


//-------------------------------------------------------------------------
// Кнопка удалить
//-------------------------------------------------------------------------
void SetterDlg::on_pbDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index == QModelIndex())
        return;

    SetterOut *setter = model->GetItem(index.row());

    if(QMessageBox::warning(this, "Предупреждение",QString("Удалить '%1 (%2)' ?").arg(setter->numberDoc).arg(setter->name), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        model->DeleteItem(index.row());
    }
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
        model->setData(index, setter);
        model->UpdateItem(index.row());

        RepoMSSQL repo;
        repo.ItemsSyncSet(setter->id, &win->track);

    }
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void SetterDlg::on_pbSelect_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index != QModelIndex())
    {
        selectSetter = model->GetItem(index.row());
        accept();
    }
}

