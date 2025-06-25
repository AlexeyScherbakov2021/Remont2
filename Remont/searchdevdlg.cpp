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

    connect(ui->leCardOrder, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leNumber2, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leNumDoc, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leName, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leDescript, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leSchet, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leDogovor, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leObjInstall, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->leNumRelease, &QLineEdit::textChanged, this, &SearchDevDlg::UpdateButtonEnabled);

    connect(ui->cbCardOrder, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbNumber2, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbNumDoc, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbName, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbDescript, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbSchet, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbDog, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbObjInst, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);
    connect(ui->cbRelease, &QCheckBox::toggled, this, &SearchDevDlg::UpdateButtonEnabled);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SearchDevDlg::UpdateButtonEnabled);

    UpdateButtonEnabled();

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
        UpdateButtonEnabled();
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
void SearchDevDlg::UpdateButtonEnabled()
{
    bool enable;
    if(ui->tableView->selectionModel() != nullptr)
    {
        enable = ui->tableView->selectionModel()->currentIndex() != QModelIndex();
        ui->pbSelect->setEnabled(enable);
    }

    enable = (!ui->leCardOrder->text().isEmpty() && ui->cbCardOrder->isChecked())
             || (!ui->leDescript->text().isEmpty() && ui->cbDescript->isChecked())
             || (!ui->leNumber2->text().isEmpty() && ui->cbNumber2->isChecked())
             || (!ui->leNumDoc->text().isEmpty() && ui->cbNumDoc->isChecked())
             || (!ui->leName->text().isEmpty() && ui->cbName->isChecked())
             || (!ui->leSchet->text().isEmpty() && ui->cbSchet->isChecked())
             || (!ui->leDogovor->text().isEmpty() && ui->cbDog->isChecked())
             || (!ui->leObjInstall->text().isEmpty() && ui->cbObjInst->isChecked())
             || (!ui->leNumRelease->text().isEmpty() && ui->cbRelease->isChecked());
    ui->pbSearch->setEnabled(enable);
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
    UpdateButtonEnabled();
}



