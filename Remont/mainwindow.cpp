#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardprodwindow.h"
#include "selectdevicewindow.h"
#include "createprodwindow.h"
#include "platewindow.h"
#include "complectproductwindow.h"
#include "otkcontrolwindow.h"
#include "listshipwindow.h"
#include "startworkwindow.h"
#include "claimwindow.h"
#include "acceptremontwindow.h"
#include "remontwindow.h"
#include "endremontwindow.h"
#include "scan.h"
#include "platelistwindow.h"
#include "Itemvnftwindow.h"
#include "platefwwindow.h"
#include "logwindow.h"
#include "qrwindow.h"
#include "otkwindow.h"
#include "prodvnftwindow.h"
#include "modvnftwindow.h"
#include "createmodwindow.h"
#include "setterdlg.h"
#include "shipwindow.h"
#include "usersetdlg.h"
#include "logindlg.h"
#include "changemoduldlg.h"
#include <models/listdevice.h>
#include <models/ItemsType.h>
#include <infrastructure/users.h>

#include <QLabel>
#include <QSettings>

MainWindow::MainWindow(int idUser, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    Users user; //= Users::getInstance();

    if(idUser == 100500)
    {
        user.UserName = "Admin";
        user.UserFullName = user.UserName;
        user.id = idUser;
    }
    else
    {
        RepoMSSQL repo;
        user = repo.LoadUser(idUser);
        user.LoadRoles(idUser);
    }

    SetRoleEnv(user);
    ui->statusbar->addWidget(new QLabel("Login: "));
    ui->statusbar->addWidget(new QLabel(user.UserFullName));

    QSettings setting("HKEY_CURRENT_USER\\Software\\Remont2", QSettings::NativeFormat);
    QString port = setting.value("COMport").toString();
    Scan::scan.open(port);
    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
}

MainWindow::~MainWindow()
{
    disconnect(conn);
    delete ui;
}


//----------------------------------------------------------------------------------------------
// Кнопка Работа с претензиями
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbClaim_clicked()
{
    ClaimWindow *win = new ClaimWindow;
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Карточка устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbCard_clicked()
{
    ui->aCardDevice->trigger();
}


//----------------------------------------------------------------------------------------------
// Кнопка Принять в ремонт
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbApplyRemont_clicked()
{
    AcceptRemontWindow *win = new AcceptRemontWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Регистрация устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbRegister_clicked()
{
    ui->aRegProduct->trigger();
}


//----------------------------------------------------------------------------------------------
// Кнопка Проверка ОТК
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbOTK_clicked()
{
    ui->aControlOTK->trigger();
}


//----------------------------------------------------------------------------------------------
// Кнопка Регистрация платы
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbCreatePlate_clicked()
{
    ui->aRegPlate->trigger();
}


//----------------------------------------------------------------------------------------------
// Кнопка Отгрузка
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbShip_clicked()
{
    ui->aShipBefore->trigger();

}


//----------------------------------------------------------------------------------------------
// Кнопка Установка в оборудование
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbComplect_clicked()
{
    ui->aInstallModul->trigger();

}


//----------------------------------------------------------------------------------------------
// Кнопка Принять в ремонт
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbStartRemont_clicked()
{
    RemontWindow *win = new RemontWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Запущен в работу
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbStartWork_clicked()
{
    ui->aStartWork->trigger();
}


//----------------------------------------------------------------------------------------------
// Кнопка Окончание ремонта
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbEndRemont_clicked()
{
    ui->aEndRepair->trigger();
}


//----------------------------------------------------------------------------------------------
// Регистрация платы
//----------------------------------------------------------------------------------------------
void MainWindow::on_aRegPlate_triggered()
{
    PlateWindow *win = new PlateWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Регистрация устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_aRegProduct_triggered()
{
    CreateProductWindow *win = new CreateProductWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Установка в оборудование
//----------------------------------------------------------------------------------------------
void MainWindow::on_aInstallModul_triggered()
{
    ComplectProductWindow *win = new ComplectProductWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Проверка ОТК
//----------------------------------------------------------------------------------------------
void MainWindow::on_aControlOTK_triggered()
{
    OTKWindow *win = new OTKWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Принять в работу
//----------------------------------------------------------------------------------------------
void MainWindow::on_aStartWork_triggered()
{
    StartWorkWindow *win = new StartWorkWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Реламации
//----------------------------------------------------------------------------------------------
void MainWindow::on_aClaim_triggered()
{
    ClaimWindow *win = new ClaimWindow;
    win->show();
}


//----------------------------------------------------------------------------------------------
// Принять в ОСО
//----------------------------------------------------------------------------------------------
void MainWindow::on_aGetOSO_triggered()
{
    AcceptRemontWindow *win = new AcceptRemontWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Принять в ремонт
//----------------------------------------------------------------------------------------------
void MainWindow::on_aGetRepair_triggered()
{
    RemontWindow *win = new RemontWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Окончание ремонта
//----------------------------------------------------------------------------------------------
void MainWindow::on_aEndRepair_triggered()
{
    EndRemontWindow *win = new EndRemontWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Карточка устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_aCardDevice_triggered()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->AddSelectedType(IndexType::Plate);
    QVector<StatusItem> stat;
    Items *dev = win->SelectDevice(false, stat, "", true, LoadPartType::ANY_PARENT);
    if(dev != nullptr)
    {
        CardProdWindow *winCard = new CardProdWindow(dev, this);
        winCard->setAttribute(Qt::WA_DeleteOnClose);
        winCard->show();
    }

}


//----------------------------------------------------------------------------------------------
// Замена модуля
//----------------------------------------------------------------------------------------------
void MainWindow::on_aExchModul_triggered()
{
    // SelectDeviceWindow *win = new SelectDeviceWindow(this);
    // // win->setTypeSearch(SelectDeviceWindow::MODU);
    // Items *dev = win->SelectDevice(false, "", Status::NONE );
    // if(dev != nullptr)
    // {
    //     // Items *mod = static_cast<Items*>(dev);
    //     // RepoMSSQL repo;
    //     // Items prod = repo.GetProduct(mod->idParent);
    //     // if(prod.id > 0)
    //     // {
    //     //     ComplectProductWindow *win = new ComplectProductWindow(this, &prod);
    //     //     win->show();
    //     // }
    // }

}


//----------------------------------------------------------------------------------------------
// Настройка сканера
//----------------------------------------------------------------------------------------------
void MainWindow::on_aScaner_triggered()
{
    Scan *win = new Scan();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->open();
}


//----------------------------------------------------------------------------------------------
// Срабатывание сканера
//----------------------------------------------------------------------------------------------
void MainWindow::slotReadScan(QString s)
{
    if(!isActiveWindow())
        return;

    // CardProdWindow *winCard;
    RepoMSSQL repo;

    QVector<StatusItem> stat;
    Items dev = repo.GetItem2( s, stat, true, true);

    if(dev.id > 0)
    {
        CardProdWindow *winCard = new CardProdWindow(&dev, this);
        winCard->setAttribute(Qt::WA_DeleteOnClose);
        winCard->show();
    }

}


//----------------------------------------------------------------------------------------------
// Список плат
//----------------------------------------------------------------------------------------------
void MainWindow::on_aListPlate_triggered()
{
    PlateListWindow *win = new PlateListWindow(this);
    // win->SelectPlate();
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->exec();

}


//----------------------------------------------------------------------------------------------
// Справочник плат ВНФТ
//----------------------------------------------------------------------------------------------
void MainWindow::on_aPlateVNFT_triggered()
{
    ItemVNFTWindow *win = new ItemVNFTWindow(IndexType::Plate, this);
    win->open();
}


//----------------------------------------------------------------------------------------------
// Замена прошивки
//----------------------------------------------------------------------------------------------
void MainWindow::on_aReplaceFW_triggered()
{
    PlateFWWindow *win = new PlateFWWindow(this);
    win->open();
}


//----------------------------------------------------------------------------------------------
// Отгруженная продукция
//----------------------------------------------------------------------------------------------
void MainWindow::on_aShipping_triggered()
{
    ListShipWindow *win = new ListShipWindow(this, true);
    win->show();
}

//----------------------------------------------------------------------------------------------
// Готовые к отгрузке
//----------------------------------------------------------------------------------------------
void MainWindow::on_aShipBefore_triggered()
{
    ListShipWindow *win = new ListShipWindow(this, false);
    win->show();
}


void MainWindow::on_aLog_triggered()
{
    LogWindow* log = LogWindow::getInstance();

    if(ui->aLog->isChecked())
        log->show();
    else
        log->hide();

}


//----------------------------------------------------------------------------------------------
// Генерация QR
//----------------------------------------------------------------------------------------------
void MainWindow::on_aGenQR_triggered()
{
    QRWindow *win = new QRWindow(this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// Меню Справочник ВНФТ изделий
//----------------------------------------------------------------------------------------------
void MainWindow::on_aProdVNFT_triggered()
{
    ItemVNFTWindow *win = new ItemVNFTWindow(IndexType::Product, this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// Меню Справочник ВНФТ модулей
//----------------------------------------------------------------------------------------------
void MainWindow::on_aModVNFT_triggered()
{
    ItemVNFTWindow *win = new ItemVNFTWindow(IndexType::Modul, this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// Регистрация модулей модулей
//----------------------------------------------------------------------------------------------
void MainWindow::on_aRegModul_triggered()
{
    CreateModulWindow *win = new CreateModulWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Открыть наборы
//----------------------------------------------------------------------------------------------
void MainWindow::on_aSetter_triggered()
{
    SetterDlg *win = new SetterDlg(false, this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
void MainWindow::on_aNewShip_triggered()
{
    Shipment ship;
    QScopedPointer<ShipWindow> win(new ShipWindow(&ship));
    win->exec();
}


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
void MainWindow::SetRoleEnv(Users &user)
{
    if(user.id == 100500)
        return;

    ui->aRegPlate->setVisible(false);
    ui->aListPlate->setVisible(false);
    ui->pbCreatePlate->setVisible(false);
    ui->aReplaceFW->setVisible(false);
    ui->aPlateVNFT->setVisible(false);
    ui->aRegProduct->setVisible(false);
    ui->pbRegister->setVisible(false);
    ui->aRegModul->setVisible(false);
    ui->aInstallModul->setVisible(false);
    ui->pbComplect->setVisible(false);
    ui->aControlOTK->setVisible(false);
    ui->pbOTK->setVisible(false);
    ui->aProdVNFT->setVisible(false);
    ui->aModVNFT->setVisible(false);
    ui->aSetter->setVisible(false);
    ui->aNewShip->setVisible(false);
    ui->aShipBefore->setVisible(false);
    ui->aShipping->setVisible(false);
    ui->pbShip->setVisible(false);
    ui->aStartWork->setVisible(false);
    ui->pbStartWork->setVisible(false);
    ui->aClaim->setVisible(false);
    ui->pbClaim->setVisible(false);
    ui->aGetOSO->setVisible(false);
    ui->pbApplyRemont->setVisible(false);
    ui->aGetRepair->setVisible(false);
    ui->pbStartRemont->setVisible(false);
    ui->aEndRepair->setVisible(false);
    ui->pbEndRemont->setVisible(false);
    ui->aCardDevice->setVisible(false);
    ui->pbCard->setVisible(false);
    ui->aGenQR->setVisible(false);
    ui->aRole->setVisible(user.id == 100500);

    for(RolesType role : user.listRoles)
    {
        switch (role)
        {
        case RolesType::RegisterPlate:
            ui->aRegPlate->setVisible(true);
            ui->aListPlate->setVisible(true);
            ui->pbCreatePlate->setVisible(true);
            break;

        case RolesType::FWPlate:
            ui->aReplaceFW->setVisible(true);
            break;

        case RolesType::VNFTPlate:
            ui->aPlateVNFT->setVisible(true);
            break;

        case RolesType::RegisterProd:
            ui->aRegProduct->setVisible(true);
            ui->pbRegister->setVisible(true);
            break;

        case RolesType::RegisterModul:
            ui->aRegModul->setVisible(true);
            break;

        case RolesType::ComplectDevice:
            ui->aInstallModul->setVisible(true);
            ui->pbComplect->setVisible(true);
            break;

        case RolesType::CheckOTK:
            ui->aControlOTK->setVisible(true);
            ui->pbOTK->setVisible(true);
            break;

        case RolesType::VNFTProd:
            ui->aProdVNFT->setVisible(true);
            break;

        case RolesType::VNFTModul:
            ui->aModVNFT->setVisible(true);
            break;

        case RolesType::ComplectSetter:
            ui->aSetter->setVisible(true);
            break;

        case RolesType::ComplectShip:
            ui->aNewShip->setVisible(true);
            ui->aShipBefore->setVisible(true);
            ui->aShipping->setVisible(true);
            ui->pbShip->setVisible(true);
            break;

        case RolesType::StartWork:
            ui->aStartWork->setVisible(true);
            ui->pbStartWork->setVisible(true);
            break;

        case RolesType::EditClaim:
            ui->aClaim->setVisible(true);
            ui->pbClaim->setVisible(true);
            break;

        case RolesType::ApplyOSO:
            ui->aGetOSO->setVisible(true);
            ui->pbApplyRemont->setVisible(true);
            break;

        case RolesType::ApplyRepair:
            ui->aGetRepair->setVisible(true);
            ui->pbStartRemont->setVisible(true);
            break;

        case RolesType::EndRepair:
            ui->aEndRepair->setVisible(true);
            ui->pbEndRemont->setVisible(true);
            break;

        case RolesType::CardDevice:
            ui->aCardDevice->setVisible(true);
            ui->pbCard->setVisible(true);
            break;

        case RolesType::GenerateQR:
            ui->aGenQR->setVisible(true);
            break;
        }
    }

}


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
void MainWindow::on_aRole_triggered()
{
    UserSetDlg *win = new UserSetDlg(this);
    win->exec();
}

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
void MainWindow::on_aEnterWork_triggered()
{
    EnterWorkWindow *win = new EnterWorkWindow(this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// произвести замену устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_aExchange_triggered()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Modul, this);
    win->AddSelectedType(IndexType::Product);
    win->AddSelectedType(IndexType::Plate);
    Items *dev = win->SelectDevice(false, {StatusItem::FAULTY_ON_OBJECT}, "", true, LoadPartType::HAS_PARENT);
    if(dev != nullptr)
    {
        ChangeModulDlg *win = new ChangeModulDlg(dev, this);
        win->exec();
    }
}

