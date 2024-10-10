#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardprodwindow.h"
#include "selectdevicewindow.h"
#include "createdevicewindow.h"
#include "platewindow.h"
#include "complectproductwindow.h"
#include "otkcontrolwindow.h"
#include "listshipwindow.h"
#include "startworkwindow.h"
#include "claimwindow.h"
#include "acceptremontwindow.h"
#include "remontwindow.h"
#include "endremontwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
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
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    IDevice *dev = win->SelectDevice(false, "", Status::NONE);
    if(dev != nullptr)
    {
        CardProdWindow *winCard;
        if(dev->typeDevice == ev::PRODUCT )
        {
            Product* prod = static_cast<Product*>(dev);
            winCard = new CardProdWindow(*prod, this);
        }
        else if(dev->typeDevice == ev::MODUL)
        {
            Modul* modul = static_cast<Modul*>(dev);
            winCard = new CardProdWindow(*modul, this);
        }
        else
            return;

        winCard->show();
    }
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
    CreateDeviceWindow *win = new CreateDeviceWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Проверка ОТК
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbOTK_clicked()
{
    OTKControlWindow *win = new OTKControlWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Регистрация платы
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbCreatePlate_clicked()
{
    PlateWindow *win = new PlateWindow(this);
    win->show();

}


//----------------------------------------------------------------------------------------------
// Кнопка Отгрузка
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbShip_clicked()
{
    ListShipWindow *win = new ListShipWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Установка в оборудование
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbComplect_clicked()
{
    ComplectProductWindow *win = new ComplectProductWindow(this);
    win->show();
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
    StartWorkWindow *win = new StartWorkWindow(this);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Окончание ремонта
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbEndRemont_clicked()
{
    EndRemontWindow *win = new EndRemontWindow(this);
    win->show();
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
void MainWindow::on_aRegDevice_triggered()
{
    CreateDeviceWindow *win = new CreateDeviceWindow(this);
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
    OTKControlWindow *win = new OTKControlWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}


//----------------------------------------------------------------------------------------------
// Готовые к отгрузке
//----------------------------------------------------------------------------------------------
void MainWindow::on_aShipBefore_triggered()
{
    ListShipWindow *win = new ListShipWindow(this);
    win->show();

}

