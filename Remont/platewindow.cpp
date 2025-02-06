#include "platewindow.h"
#include "repofp.h"
#include "scan.h"
#include "ui_platewindow.h"

#include <QMessageBox>

// #include <models/platetype.h>

PlateWindow::PlateWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateWindow)
{
    ui->setupUi(this);
    ui->deCreateDate->setDateTime(QDateTime::currentDateTime());

    repo.LoadTypeItem(ItemType::Plate, listVNFT);

    for(auto const &it : listVNFT)
    {
        ui->cbVNFT->addItem(it.VNFT + " " + it.typeName, it.id);
    }
    ui->cbVNFT->setCurrentIndex(-1);

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

PlateWindow::~PlateWindow()
{
    disconnect(conn);
    delete ui;
}

void PlateWindow::on_pbAdd_clicked()
{
    if(ui->cbVNFT->currentIndex() < 0)
    {
        QMessageBox::critical(this, "Ошибка", QString("Необходимо выбрать обозначение."));
        return;
    }

    ui->leNumber->setFocus();

    if(ui->leNumber->text().isEmpty())
        return;

    Items plate;
    plate.dateCreate = ui->deCreateDate->dateTime();
    plate.number = ui->leNumber->text();
    plate.number2 = ui->leNumberFW->text();
    plate.numberDoc = ui->leNumberDoc->text();
    plate.idType = ui->cbVNFT->currentData().toInt();
    plate.garantMonth = listVNFT[ui->cbVNFT->currentIndex()].garantMonth;

    if(!repo.AddItem(plate))
    {
        QMessageBox::critical(this, "Ошибка", QString("Серийный № %1 уже существует.").arg(plate.number));
        return;
    }
    else
    {
        plate.AddStatus(plate, Status::CREATE);
        QListWidgetItem *item = new QListWidgetItem(plate.number + " (прош." + plate.number2 + ")");
        item->setData(Qt::UserRole, plate.id);
        ui->listWidget->addItem(item);
        ui->leNumber->clear();
        ++countUse;
        UpdateUseCount();
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
    if(repo.DeleteItem(id))
    {
        delete ui->listWidget->currentItem();
        --countUse;
        UpdateUseCount();
    }
}


void PlateWindow::on_leNumber_textChanged(const QString &arg1)
{
    ui->pbAdd->setEnabled(!arg1.isEmpty());
}


void PlateWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->tbDelete->setEnabled(currentRow >= 0);
}

void PlateWindow::UpdateUseCount()
{
    if(countFromDoc > 0)
        ui->lbCount->setText(QString("%1 (рег. %2)").arg(countFromDoc).arg(countUse));
    else
        ui->lbCount->clear();

    if(countUse > countFromDoc)
        ui->lbCount->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCount->setStyleSheet("border: 1px solid #000000;");
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


void PlateWindow::on_tbDoc_clicked()
{
    RepoFP repoFP;
    Nakl nakl;
    // int useCount;
    QList<Nakl> listNakl;
    repoFP.getDoc(ui->leNumberDoc->text(), listNakl);

    size_t size = listNakl.size();

    if(size == 0)
    {
        // накладная не найдена
    }
    else
    {
        if(size == 1)
        {
            int i = 0;
            nakl = listNakl.first();
            ui->cbVNFT->setCurrentIndex(-1);
            for(auto &it : listVNFT)
            {
                if(it.VNFT == nakl.VNFT)
                {
                    ui->cbVNFT->setCurrentIndex(i);
                    // countUse = repo.GetCountRegisterPlate(ui->leNumberDoc->text(), it.id);
                    countFromDoc = nakl.count;
                    break;
                }
                ++i;
            }
        }
        else
        {
            ui->cbVNFT->setCurrentIndex(-1);
        }
    }

    ui->lbName->setText(nakl.name);
    ui->lbPlan->setText(nakl.plan);
    UpdateUseCount();
}

