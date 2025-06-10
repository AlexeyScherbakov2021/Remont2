#include "cardprodwindow.h"
#include "searchdevdlg.h"
#include "ui_searchdevdlg.h"

#include <QClipboard>

SearchDevDlg::SearchDevDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchDevDlg)
{
    ui->setupUi(this);
    model = new DeviceModel(IndexType::All, this);

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 30);       // icon
    ui->tableView->setColumnWidth(1, 100);      // Номер
    ui->tableView->setColumnWidth(2, 200);      // тип
    ui->tableView->setColumnWidth(3, 200);      // внфт
    ui->tableView->setColumnWidth(4, 150);      // наименование
    ui->tableView->setColumnWidth(5, 80);       // дата регистрации
    ui->tableView->setColumnWidth(6, 80);       // статус

    ui->tableView->addAction("Карточка устройства", this, [this] () {

        QModelIndex index = ui->tableView->currentIndex();
        // index = proxy->mapToSource(index);
        Items* dev = model->GetItem(&index);
        if(dev->id > 0 && dev->type.indexType <= IndexType::Plate)
        {
            CardProdWindow *win = new CardProdWindow(dev, this);
            win->exec();
        }

    });
    ui->tableView->addAction("Скопировать номер", this, [this] () {
        QModelIndex index = ui->tableView->currentIndex();
        // index = proxy->mapToSource(index);
        Items* dev = model->GetItem(&index);
        QClipboard *cpb = QApplication::clipboard();
        cpb->setText(dev->number, QClipboard::Clipboard);
    });

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);


}

//-------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------
SearchDevDlg::~SearchDevDlg()
{
    delete ui;
}


//-------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------
void SearchDevDlg::on_pbSelect_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();

    if(index == QModelIndex())
        return;

    Items* dev = model->GetItem(&index);
    if(dev->id > 0 && dev->type.indexType <= IndexType::Plate)
    {
        CardProdWindow *win = new CardProdWindow(dev, this);
        win->exec();
    }
}

//-------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------
void SearchDevDlg::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pbSelect_clicked();
}


//-------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------
void SearchDevDlg::on_pbSearch_clicked()
{
    optionSearch opt;
    opt.cardOrder = ui->cbCardOrder->isChecked() ? ui->leCardOrder->text() : "";
    opt.descript = ui->cbDescript->isChecked() ? ui->leDescript->text() : "";
    opt.dogovor = ui->cbDog->isChecked() ? ui->leDogovor->text() : "";
    opt.name = ui->cbName->isChecked() ? ui->leName->text() : "";
    opt.number2 = ui->cbNumber2->isChecked() ? ui->leNumber2->text() : "";
    opt.numberDoc = ui->cbNumDoc->isChecked() ? ui->leNumDoc->text() : "";
    opt.numRelease = ui->cbRelease->isChecked() ? ui->leNumRelease->text() : "";
    opt.objInstall = ui->cbObjInst->isChecked() ? ui->leObjInstall->text() : "";
    opt.schet = ui->cbSchet->isChecked() ? ui->leSchet->text() : "";
    QApplication::setOverrideCursor(Qt::WaitCursor);
    model->prepareSearch(opt);
}



