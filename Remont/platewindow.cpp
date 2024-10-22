#include "platewindow.h"
#include "scan.h"
#include "ui_platewindow.h"

#include <QMessageBox>

PlateWindow::PlateWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateWindow)
{
    ui->setupUi(this);
    ui->deCreateDate->setDateTime(QDateTime::currentDateTime());

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

PlateWindow::~PlateWindow()
{
    disconnect(conn);
    delete ui;
}

void PlateWindow::on_pbAdd_clicked()
{
    ui->leNumber->setFocus();

    if(ui->leNumber->text().isEmpty())
        return;

    Plate plate;
    plate.dateRegister = ui->deCreateDate->dateTime();
    plate.number = ui->leNumber->text();
    plate.number2 = ui->leNumberFW->text();
    plate.numberDoc = ui->leNumberDoc->text();
    plate.VNFT = ui->leVNFT->text();

    if(!repo.AddItem(plate))
    {
        QMessageBox::critical(this, "Ошибка", QString("Серийный № %1 уже существует.").arg(plate.number));
        return;
    }
    else
    {
        QListWidgetItem *item = new QListWidgetItem(plate.number + " (прош." + plate.number2 + ")");
        item->setData(Qt::UserRole, plate.id);
        ui->listWidget->addItem(item);
        ui->leNumber->clear();

    }

}


void PlateWindow::on_tbDelete_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();

    if(item == nullptr)
        return;

    // Plate plate;
    int id = item->data(Qt::UserRole).toInt();

    // удаление платы из базы
    if(repo.DeletePlate(id))
        delete ui->listWidget->currentItem();
}


void PlateWindow::on_leNumber_textChanged(const QString &arg1)
{
    ui->pbAdd->setEnabled(!arg1.isEmpty());
}


void PlateWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->tbDelete->setEnabled(currentRow >= 0);
}


//-------------------------------------------------------------------------
// строка от сканера
//-------------------------------------------------------------------------
void PlateWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumber->setText(s);
        emit ui->pbAdd->click();
    }
}

