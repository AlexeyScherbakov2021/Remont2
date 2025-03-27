#include "changemoduldlg.h"
#include "selectdevicewindow.h"
#include "ui_changemoduldlg.h"

ChangeModulDlg::ChangeModulDlg(Items* dev, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeModulDlg), brokenDev(dev)
{
    ui->setupUi(this);

    QString nameType, nameIcon;
    dev->GetInfo(nameType, nameIcon);

    ui->lbDevice->setPixmap(QPixmap(nameIcon));
    ui->lbNumber->setText(dev->number);
    ui->lbType->setText(dev->type.typeName);
    ui->lbVNFT->setText(dev->type.VNFT);

    Items parentDev = repo.GetItem(dev->idParent);
    ui->wTree->AddItem(&parentDev);
    ui->wTree->SetSelectItem(dev->id, dev->type.indexType);
}

ChangeModulDlg::~ChangeModulDlg()
{
    delete ui;
}

//---------------------------------------------------------------------------
// поиск обрудования
//---------------------------------------------------------------------------
void ChangeModulDlg::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(brokenDev->type.indexType);
    Items *dev =  win->SelectDevice(true, {}, ui->leSwarch->text());
    if(dev != nullptr && dev->id > 0)
    {
        // qInfo() << dev->number;
        newDev = *dev;
        ui->lbNumber_2->setText(dev->number);
        ui->lbType_2->setText(dev->type.typeName);
        ui->lbVNFT_2->setText(dev->type.VNFT);
    }
}


//---------------------------------------------------------------------------
// Кнопка ОК
//---------------------------------------------------------------------------
void ChangeModulDlg::on_pbOK_clicked()
{
    brokenDev->AddStatus(*brokenDev, StatusItem::EXCHANGE, "", brokenDev->idParent);
    newDev.AddStatus(newDev, StatusItem::WORK);
    newDev.idParent = brokenDev->idParent;
    repo.UpdateItem(newDev);

    Items parent = repo.GetItem(brokenDev->idParent);
    do
    {
        bool res = parent.TestChildGoodStatus();
        if(res)
            parent.AddStatus(parent, StatusItem::WORK);

        parent = repo.GetItem(parent.idParent);
    } while(parent.idParent > 0);

    accept();
}

//---------------------------------------------------------------------------
// проверка исправности вложенного оборудования
//---------------------------------------------------------------------------
// bool ChangeModulDlg::testChildGoodStatus(Items *dev)
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
//         // res = testChildGoodStatus(&it);
//         // if(res == false)
//         //     break;
//     }
//     return res;
// }

