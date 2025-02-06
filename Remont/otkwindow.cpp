#include "otkwindow.h"
#include "ui_otkwindow.h"

OTKWindow::OTKWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OTKWindow)
{
    ui->setupUi(this);
}


OTKWindow::OTKWindow(ListDevice *dev, QWidget *parent) : OTKWindow(parent)
{
}

// OTKWindow::OTKWindow(ListDevice<Modul> *dev, QWidget *parent) : OTKWindow(parent)
// {
// }

// // template <typename T>
// OTKWindow::OTKWindow(ListDevice<Plate> *dev, QWidget *parent) : OTKWindow(parent)
// {
// }

OTKWindow::~OTKWindow()
{
    delete ui;
}
