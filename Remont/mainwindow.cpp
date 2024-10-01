#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardprodwindow.h"
#include "selectdevicewindow.h"
#include "createdevicewindow.h"
#include "platewindow.h"
#include "complectproductwindow.h"
#include "otkcontrolwindow.h"
#include "listshipwindow.h"

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
    // ClaimWindow *win = new ClaimWindow;
    // win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Карточка устройства
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbCard_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    if(win->exec() == QDialog::Accepted)
    {
        CardProdWindow *winCard;
        if(win->prod.id != 0 )
            winCard = new CardProdWindow(win->prod, this);
        else if(win->modul.id != 0)
            winCard = new CardProdWindow(win->modul, this);
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
    // AcceptRemontWindow *win = new AcceptRemontWindow(this);
    // win->show();
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
    win->exec();

}


//----------------------------------------------------------------------------------------------
// Кнопка Отгрузка
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbShip_clicked()
{
    ListShipWindow *win = new ListShipWindow(this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// Кнопка Установка в оборудование
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbComplect_clicked()
{
    ComplectProductWindow *win = new ComplectProductWindow(this);
    win->exec();
}


//----------------------------------------------------------------------------------------------
// Кнопка Принять в ремонт
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbStartRemont_clicked()
{
    // RemontWindow *win = new RemontWindow(this);
    // win->show();
}


//----------------------------------------------------------------------------------------------
// Кнопка Запущен в работу
//----------------------------------------------------------------------------------------------
void MainWindow::on_pbStartWork_clicked()
{
    // StartWorkWindow *win = new StartWorkWindow(this);
    // win->exec();
}


void MainWindow::on_pbEndRemont_clicked()
{
    // EndRemontWindow *win = new EndRemontWindow(this);
    // win->exec();
}

