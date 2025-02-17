#include "settereditdlg.h"
#include "ui_settereditdlg.h"

SetterEditDlg::SetterEditDlg(SetterOut* _setter, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetterEditDlg), setter(_setter)
{
    ui->setupUi(this);
    setWindowTitle("Набор " + setter->numberDoc + setter->name);

    repo.LoadChildSetter(*setter);

    int row = 0;
    for(auto &it : setter->childItems)
    {
        AddLineToWidget(&it, row);
        ++row;
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SetterEditDlg::~SetterEditDlg()
{
    delete ui;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SetterEditDlg::AddLineToWidget(Items *item, int row)
{
    QString nameType, nameIcon;
    QListWidgetItem *line = new QListWidgetItem();
    line->setText(item->GetDefaultName());
    item->GetInfo(nameType, nameIcon);
    line->setIcon(QIcon(nameIcon));

    ui->listWidget->addItem(line);
}

//-----------------------------------------------------------------------------
// Кнопка Удалить из списка устройства
//-----------------------------------------------------------------------------
void SetterEditDlg::on_tbDelete_clicked()
{

}


//-----------------------------------------------------------------------------
// Поиск и добавление устройства
//-----------------------------------------------------------------------------
void SetterEditDlg::on_tbSearch_clicked()
{

}

