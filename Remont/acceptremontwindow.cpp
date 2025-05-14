#include "acceptremontwindow.h"
#include "changemoduldlg.h"
#include "complectproductwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_acceptremontwindow.h"
// #include <models/product.h>


#include <QMessageBox>

#include <models/remont.h>

AcceptRemontWindow::AcceptRemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AcceptRemontWindow)
{
    ui->setupUi(this);
    ui->deDate->setDateTime(QDateTime::currentDateTime());
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

AcceptRemontWindow::~AcceptRemontWindow()
{
    delete ui;
}


//------------------------------------------------------------------------
// Кнопка принять в ремонт
//------------------------------------------------------------------------
void AcceptRemontWindow::on_pbApply_clicked()
{
    if(device.id == 0)
        return;


    Status stat;
    stat.dateStatus = ui->deDate->dateTime();
    stat.idStatus = StatusItem::FAULTY_ON_OSO;
    stat.Comment = ui->leDescript->text();
    stat.numberDoc = ui->leDoc->text();

    device.AddStatus(device, stat);

    // device.AddStatus(device, StatusItem::FAULTY_ON_OSO, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 принят в ОСО.")
                .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    // Добавление в ремонт изделия, если есть
    Items parent = repo.GetItem(device.idParent);
    while(parent.id > 0)
    {
        Remont remontParent = repo.GetRemontForItem(parent.id, claim.id);
        if(remontParent.id == 0)
        {
            // создаем в ремонте, если не было
            remontParent.idClaim = claim.id;
            remontParent.idItem = parent.id;
            remontParent.startDate = ui->deDate->dateTime();
            repo.AddRemont(remontParent);
        }
        parent = repo.GetItem(parent.idParent);
    }

    // Добавление в ремонт
    Remont remont;
    remont.idClaim = claim.id;
    remont.idItem = device.id;
    remont.startDate = ui->deDate->dateTime();
    repo.AddRemont(remont);

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;

}


//-------------------------------------------------------------------------------------------------
// Кнопка Найти устройство
//-------------------------------------------------------------------------------------------------
void AcceptRemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::FAULTY_ON_OBJECT, StatusItem::EXCHANGE}, ui->leNumber->text(), true, LoadPartType::ANY_PARENT );
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
    }
}

void AcceptRemontWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumber->setText(s);
        Items item = repo.GetItem2(s, {StatusItem::FAULTY_ON_OBJECT}, true, true);
        if(item.id > 0)
        {
            AddDevice(&item);
        }
    }
}

void AcceptRemontWindow::AddDevice(Items *dev)
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

    Q_ASSERT(dev->listStatus.size() > 0);

}

