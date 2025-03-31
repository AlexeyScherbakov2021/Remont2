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


//-------------------------------------------------------------------------------------------------
// Кнопка Произвести замену
//-------------------------------------------------------------------------------------------------
// void AcceptRemontWindow::on_pbExchange_clicked()
// {
//     if(device.idParent == 0)
//     {
//         QMessageBox::critical(this, "Предупреждение", "Заменить можно только модуль в составе изделия.");
//         return;
//     }

//     // Items prod = repo.GetItem(device.idParent);
//     ChangeModulDlg *win = new ChangeModulDlg(&device, this);
//     if(win->exec() == QDialog::Accepted)
//     {
//         // qInfo() << "Замена произошла";
//         QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 земенен.")
//                 .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

//         accept();
//     }
// }


//------------------------------------------------------------------------
// Кнопка принять в ремонт
//------------------------------------------------------------------------
void AcceptRemontWindow::on_pbApply_clicked()
{
    if(device.id == 0)
        return;


    if(device.listStatus.last().idStatus == StatusItem::EXCHANGE)
    {
        device.idParent = 0;
        repo.UpdateItem(device);
    }

    device.AddStatus(device, StatusItem::FAULTY_ON_OSO, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 принят в ОСО.")
                .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    // Добавление в ремонт изделия, если есть
    Items parent = repo.GetItem(device.idParent);
    while(parent.id > 0)
    {
        // Claim claim = repo.GetClaimForItem(parent.id);
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



    // if(device.idParent > 0)
    // {
    //     Items parent = repo.GetItem(device.idParent);
    //     Remont remontParent = repo.GetRemontForItem(parent.id);
    //     if(remontParent.id == 0)
    //     {
    //         // создаем в ремонте, если не было
    //         remontParent.idClaim = claim.id;
    //         remontParent.idItem = parent.id;
    //         remontParent.startDate = ui->deDate->dateTime();
    //         repo.AddRemont(remontParent);
    //     }
    // }

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
    Items *dev = win->SelectDevice(true, {StatusItem::FAULTY_ON_OBJECT, StatusItem::EXCHANGE}, ui->leNumber->text(), true, LoadPartType::HAS_PARENT );
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);

        // device = *dev;
        // claim = repo.GetClaimForItem(dev->id);

        // QString nameType, iconName;
        // dev->GetInfo(nameType, iconName);
        // ui->lbNumber->setText(dev->number);
        // ui->lbVNFT->setText(dev->type.VNFT);
        // ui->lbTypeName->setText(dev->type.typeName);
        // ui->lbDevice->setToolTip(nameType);
        // ui->lbDevice->setPixmap(QPixmap(iconName));
        // ui->lbOrgName->setText(claim.nameOrganization);
        // ui->lbClaim->setText(claim.number + " (" + claim.dateCreate.toString("dd.MM.yyyy") + ")");
        // ui->leNumber->clear();
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
        // on_tbNumber_clicked();
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
    // if(dev->listStatus.last().idStatus == StatusItem::EXCHANGE)
    // bool isExch = !(dev->listStatus.last().idStatus == StatusItem::EXCHANGE) && dev->idParent > 0;

    // ui->pbExchange->setEnabled(isExch);

}

