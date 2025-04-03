#include "endremontwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_endremontwindow.h"
#include <models/claim.h>
#include <models/remont.h>
#include <QMessageBox>

EndRemontWindow::EndRemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EndRemontWindow)
{
    ui->setupUi(this);

    ui->deDate->setDateTime(QDateTime::currentDateTime());

    QMap<int, QString> listReason;
    repo.LoadRemontReason(listReason);

    for( auto it = listReason.begin(); it != listReason.end(); ++it)
    {
        ui->cbReason->addItem(*it, it.key());
    }

    ui->cbReason->setCurrentIndex(0);
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

EndRemontWindow::~EndRemontWindow()
{
    delete ui;
}


//---------------------------------------------------------------------------------------
// Кнопка Поиск по номеру
//---------------------------------------------------------------------------------------
void EndRemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::REMONT}, ui->leNumber->text(), true, LoadPartType::ANY_PARENT );
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
    }
}


//---------------------------------------------------------------------------------------
// Кнопка Закончить ремонт
//---------------------------------------------------------------------------------------
void EndRemontWindow::on_pbEndRemont_clicked()
{
    StatusItem stat = ui->cbScrap->isChecked() ? StatusItem::END_WORK : StatusItem::CORRECT_OSO;

    if(device.id == 0)
        return;

    // отметка для рекламации о завершении ремонта этого устройства
    repo.SetEndWorkClaim(device.id, claim.id);

    // внести изменения в таблицы ремонтов
    Remont rem = repo.GetRemontForItem(device.id, claim.id);
    Q_ASSERT(rem.id != 0);
    Q_ASSERT(ui->cbReason->currentData(Qt::UserRole) != QVariant());

    rem.action = ui->leAction->text();
    rem.defect = ui->leDefect->text();
    rem.endDate = ui->deDate->dateTime();
    rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
    rem.remark = ui->ptRemark->document()->toPlainText();

    repo.UpdateRemont(rem);

    device.AddStatus(device, stat, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 ремонт завершен.")
                .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;

}

//---------------------------------------------------------------------------
// проверка исправности вложенного оборудования
//---------------------------------------------------------------------------
// bool EndRemontWindow::testChildGoodStatus(Items *dev)
// {
//     bool res = true;
//     repo.LoadChildItems(dev->id, dev->childItems);
//     for(auto &it : dev->childItems)
//     {
//         if(it.listStatus.size() > 0 && it.listStatus.last().typeStatus == 1)
//         {
//             res = false;
//             break;
//         }
//     }
//     return res;
// }



void EndRemontWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumber->setText(s);
        Items item = repo.GetItem2(s, {StatusItem::REMONT}, true, true);
        if(item.id > 0)
        {
            AddDevice(&item);
        }
        // on_tbNumber_clicked();
    }
}


void EndRemontWindow::AddDevice(Items *dev)
{
    device = *dev;
    claim = repo.GetClaimForItem(dev->id);

    QString nameType, iconName;
    dev->GetInfo(nameType, iconName);
    ui->lbNumber->setText(dev->number);
    ui->lbVNFT->setText(dev->type.VNFT);
    ui->lbTypeName->setText(dev->type.typeName);
    ui->lbDevice->setToolTip(nameType);
    ui->lbDevice->setPixmap(QPixmap(iconName));
    ui->lbOrgName->setText(claim.nameOrganization);
    ui->lbClaim->setText(claim.number + " (" + claim.dateCreate.toString("dd.MM.yyyy") + ")");
    ui->leNumber->clear();

}
