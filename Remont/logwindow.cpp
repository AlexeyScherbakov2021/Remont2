#include "logwindow.h"
#include "ui_logwindow.h"

LogWindow::LogWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogWindow)
{
    ui->setupUi(this);
}

LogWindow::~LogWindow()
{
    delete ptr;
    delete ui;
}

LogWindow *LogWindow::getInstance()
{
    if(ptr == nullptr)
        ptr = new LogWindow();

    return ptr;
}

void LogWindow::AddLine(QString s)
{
    if(ptr == nullptr)
        getInstance();

    ptr->ui->teLog->append(s);
}

void LogWindow::close()
{
    delete ptr;
}


void LogWindow::on_pbClear_clicked()
{
    ptr->ui->teLog->clear();
}

