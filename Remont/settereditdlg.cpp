#include "selectdevicewindow.h"
#include "settereditdlg.h"
#include "ui_settereditdlg.h"

#include <QMessageBox>

SetterEditDlg::SetterEditDlg(SetterOut* _setter, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetterEditDlg), setter(_setter)
{
    ui->setupUi(this);
    setWindowTitle("Набор " + setter->numberDoc + setter->name);

    ui->leOrder->setText(setter->numberDoc);
    ui->leName->setText(setter->name);
    ui->deDate->setDateTime(setter->dateCreate);
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
    auto item = ui->listWidget->currentItem();
    if(item != nullptr)
    {
        if(QMessageBox::warning(this, "Предупреждение",
                QString("Удалить %1 ?").arg(item->text()),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            delete item;
        }
    }
}


//-----------------------------------------------------------------------------
// Поиск и добавление устройства
//-----------------------------------------------------------------------------
void SetterEditDlg::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->AddSelectedType(IndexType::Plate);
    QVector<int> stat {Status::CORRECT, Status::CORRECT_OSO };
    Items* dev = win->SelectDevice(true, stat, ui->leSearch->text());
    if(dev->id != 0)
    {
        AddLineToWidget(dev, ui->listWidget->count());
    }

}


void SetterEditDlg::on_pbOk_clicked()
{
    setter->name = ui->leName->text();
    setter->dateCreate = ui->deDate->dateTime();
    setter->numberDoc = ui->leOrder->text();
    accept();
}

