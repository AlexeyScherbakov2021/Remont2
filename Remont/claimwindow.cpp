#include "claimdetail.h"
#include "claimwindow.h"
#include "ui_claimwindow.h"
#include <QMessageBox>

ClaimWindow::ClaimWindow(QWidget *parent/*, bool isSelected*/)
    : QDialog(parent)
    , ui(new Ui::ClaimWindow)//, isSelected(isSelected)
{
    ui->setupUi(this);

    claims.Load();
    // repo.LoadClaim(listClaim);
    for(auto const &it : claims.listItems)
        AddLineScreen(&it);

    ui->twClaim->resizeColumnsToContents();
    ui->twClaim->resizeRowsToContents();

    // ui->pbSelect->setVisible(isSelected);
}

ClaimWindow::~ClaimWindow()
{
    delete ui;
}


//----------------------------------------------------------------------------------------
// Кнопка Добавить рекламацию
//----------------------------------------------------------------------------------------
void ClaimWindow::on_pbAdd_clicked()
{
    Claim claim;

    ClaimDetail *win = new ClaimDetail(&claim, this);
    if(win->exec() == QDialog::Accepted)
    {
        int row = AddLineScreen(&claim);
        ui->twClaim->resizeColumnsToContents();
        ui->twClaim->resizeRowToContents(row);
        // if(repo.AddItem(claim))
        if(claim.id > 0)
            claims.listItems.push_back(claim);
    }
}


//----------------------------------------------------------------------------------------
// Добавить рекламацию строку в таблицу
//----------------------------------------------------------------------------------------
int ClaimWindow::AddLineScreen(const Claim *claim)
{
    int row = ui->twClaim->rowCount();
    ui->twClaim->insertRow(row);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(claim->number);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twClaim->setItem(row, 0, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->dateRegister.toString("dd.MM.yyyy"));
    ui->twClaim->setItem(row, 1, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->FromWho);
    ui->twClaim->setItem(row, 2, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->TypeClaimString);
    ui->twClaim->setItem(row, 3, item);

    // item = new QTableWidgetItem();
    // item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    // item->setText(claim->idOrg);
    // ui->twClaim->setItem(row, 4, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->ObjectInstall);
    ui->twClaim->setItem(row, 5, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->Descript);
    ui->twClaim->setItem(row, 6, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->TypeComplectString);
    ui->twClaim->setItem(row, 7, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->VNFT);
    ui->twClaim->setItem(row, 8, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->TypeDeviceString);
    ui->twClaim->setItem(row, 9, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->NumberModul);
    ui->twClaim->setItem(row, 10, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->NumberNewModul);
    ui->twClaim->setItem(row, 11, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->NumberDevice);
    ui->twClaim->setItem(row, 12, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(claim->DateOut.toString("dd.MM.yyyy"));
    ui->twClaim->setItem(row, 13, item);

    return row;
}

//----------------------------------------------------------------------------------------
// Кнопка Удалить рекламацию
//----------------------------------------------------------------------------------------
void ClaimWindow::on_pbDelete_clicked()
{
    int row = ui->twClaim->currentRow();
    if(row < 0 )
        return;

    if(QMessageBox::warning(this, "Предупреждение",
                             QString("Удалить рекламацию № %1 ?").arg(claims.listItems.at(row).number), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    if(repo.DeleteClaim(claims.listItems[row].id))
    {
        claims.listItems.removeAt(row);
        ui->twClaim->removeRow(row);
    }
}


//----------------------------------------------------------------------------------------
// Кнопка Изменить рекламацию
//----------------------------------------------------------------------------------------
void ClaimWindow::on_pbEdit_clicked()
{
    int row = ui->twClaim->currentRow();
    if(row < 0 )
        return;

    Claim *claim = &claims.listItems[row];

    ClaimDetail *win = new ClaimDetail(claim, this);
    if(win->exec() == QDialog::Accepted)
    {
        if(repo.UpdateItem(*claim))
        {
            QTableWidgetItem *item = ui->twClaim->item(row, 0);
            item->setText(claim->number);
            item = ui->twClaim->item(row, 1);
            item->setText(claim->dateRegister.toString("dd.MM.yyyy"));
            item = ui->twClaim->item(row, 2);
            item->setText(claim->FromWho);
            item = ui->twClaim->item(row, 3);
            item->setText(claim->TypeClaimString);
            item = ui->twClaim->item(row, 4);
            // item->setText(claim->idOrg);
            item = ui->twClaim->item(row, 5);
            item->setText(claim->ObjectInstall);
            item = ui->twClaim->item(row, 6);
            item->setText(claim->Descript);
            item = ui->twClaim->item(row, 7);
            item->setText(claim->TypeComplectString);
            item = ui->twClaim->item(row, 8);
            item->setText(claim->VNFT);
            item = ui->twClaim->item(row, 9);
            item->setText(claim->TypeDeviceString);
            item = ui->twClaim->item(row, 10);
            item->setText(claim->NumberModul);
            item = ui->twClaim->item(row, 11);
            item->setText(claim->NumberNewModul);
            item = ui->twClaim->item(row, 12);
            item->setText(claim->NumberDevice);
            item = ui->twClaim->item(row, 13);
            item->setText(claim->DateOut.toString("dd.MM.yyyy"));
        }

    }
}


//----------------------------------------------------------------------------------------
// Кнопка Изменить рекламацию двойной клик
//----------------------------------------------------------------------------------------
void ClaimWindow::on_twClaim_cellDoubleClicked(int /*row*/, int /*column*/)
{
    // if(isSelected)
    //     on_pbSelect_clicked();
    // else
        on_pbEdit_clicked();
}


//----------------------------------------------------------------------------------------
// Кнопка Выбрать рекламацию
//----------------------------------------------------------------------------------------
// void ClaimWindow::on_pbSelect_clicked()
// {
//     int row = ui->twClaim->currentRow();
//     if(row < 0 )
//         return;

//     selectedClaim = claims.listItems[row];

//     accept();
// }

