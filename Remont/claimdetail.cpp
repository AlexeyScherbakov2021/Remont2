#include "claimdetail.h"
#include "selectdevicewindow.h"
#include "ui_claimdetail.h"

ClaimDetail::ClaimDetail(Claim *cl,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClaimDetail), claim(cl)
{
    ui->setupUi(this);

    repo.LoadClaimType(listTypeClaim);
    for(auto it = listTypeClaim.cbegin(); it != listTypeClaim.cend(); ++it)
        ui->cbTypeClaim->addItem(*it, it.key());

    repo.LoadClaimModules(claim->id, claim->listModul);
    repo.LoadClaimProducts(claim->id, claim->listProduct);

    ClaimToScreen(claim);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

}

ClaimDetail::~ClaimDetail()
{
    delete ui;
}


//-----------------------------------------------------------------------------------------
// Кнопка ОК
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_pbOK_clicked()
{
    claim->number = ui->leNumber->text();
    claim->dateRegister = ui->deDateClaim->dateTime();
    claim->FromWho = ui->leFromWho->text();
    claim->ObjectInstall = ui->leObjectInst->text();
    claim->Descript = ui->leDescript->text();
    // claim->idOrg = ui->leOrganiz->text();
    claim->Reason = ui->leReason->text();
    claim->DateRepair = ui->deDateRepair->dateTime();
    claim->DoRepair = ui->leDoRepair->text();
    claim->FileAnswer = ui->leFileAnswer->text();
    claim->TextResult = ui->leTextResult->text();
    claim->IsGuarantee = ui->cbGuarantee->isChecked();

    claim->idTypeClaim = ui->cbTypeClaim->currentData(Qt::UserRole).toInt();

    if(claim->id == 0)
        repo.AddItem(*claim);

    for(auto &it : listAddModul)
    {
        if(repo.AddModulToClaim(it.id, claim->id))
            it.AddStatus(it, Status::FAULTY_ON_OBJECT);
    }
    for(auto &it : listDelModul)
    {
        if(repo.DelModulFromClaim(it.id, claim->id))
            it.DeleteLastStatus(it);

    }

    for(auto &it : listAddProduct)
    {
        if(repo.AddProductToClaim(it.id, claim->id))
            it.AddStatus(it, Status::FAULTY_ON_OBJECT);
    }

    for(auto &it : listDelProduct)
    {
        if(repo.DelProductToClaim(it.id, claim->id))
            it.DeleteLastStatus(it);

    }

    accept();
}

//-----------------------------------------------------------------------------------------
// Отображение данных на экран
//-----------------------------------------------------------------------------------------
void ClaimDetail::ClaimToScreen(Claim *claim)
{
    ui->leNumber->setText(claim->number);
    ui->deDateClaim->setDateTime(claim->dateRegister);
    ui->leFromWho->setText(claim->FromWho);
    // ui->leOrganiz->setText(claim->idOrg);
    ui->leObjectInst->setText(claim->ObjectInstall);
    ui->leDescript->setText(claim->Descript);
    ui->leReason->setText(claim->Reason);
    ui->deDateRepair->setDateTime(claim->DateRepair);
    ui->leDoRepair->setText(claim->DoRepair);
    ui->leFileAnswer->setText(claim->FileAnswer);
    ui->leTextResult->setText(claim->TextResult);
    ui->cbGuarantee->setChecked(claim->IsGuarantee);

    ui->cbTypeClaim->setCurrentText(listTypeClaim[claim->idTypeClaim]);

    for(auto &it : claim->listModul)
        AddModulToTableScreen(it);

    for(auto &it : claim->listProduct)
        AddProductToTableScreen(it);
}


//-----------------------------------------------------------------------------------------
// Добавление модуля в таблицу экрана
//-----------------------------------------------------------------------------------------
void ClaimDetail::AddModulToTableScreen(const Modul &modul)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::UserRole, modul.id);
    item->setData(Qt::UserRole + 1, 1);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText("Модуль");
    ui->tableWidget->setItem(row, 0, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(modul.number);
    ui->tableWidget->setItem(row, 1, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(modul.name);
    ui->tableWidget->setItem(row, 2, item);

}

//-----------------------------------------------------------------------------------------
// Добавление изделия в таблицу экрана
//-----------------------------------------------------------------------------------------
void ClaimDetail::AddProductToTableScreen(const Product &prod)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::UserRole, prod.id);
    item->setData(Qt::UserRole + 1, 0);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText("Изделие");
    ui->tableWidget->setItem(row, 0, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(prod.number);
    ui->tableWidget->setItem(row, 1, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setText(prod.name);
    ui->tableWidget->setItem(row, 2, item);
}


//-----------------------------------------------------------------------------------------
// Кнопка добавления устройства
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_tbAddDevice_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this);

    QVector<Status::Stat> listStatus = {Status::WORK, Status::SHIPPED};
    IDevice *dev = win->SelectDevice(false, "", listStatus);
    if(dev != nullptr)
    // if(win->exec() == QDialog::Accepted)
    {
        if(dev->typeDevice == ev::PRODUCT)
        {
            Product* prod = static_cast<Product*>(dev);
            AddProductToTableScreen(*prod);
            listAddProduct.insert(prod->id, *prod);
        }
        else if(dev->typeDevice == ev::MODUL)
        {
            Modul *modul = static_cast<Modul*>(dev);
            AddModulToTableScreen(*modul);
            listAddModul.insert(modul->id, *modul);
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
    }
}


//-----------------------------------------------------------------------------------------
// Кнопка удаления устройства
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_tbDeleteDevice_clicked()
{

    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();
    int type = item->data(Qt::UserRole + 1).toInt();

    if(type == 0)
    {
        if(listAddProduct.contains(id))
            listAddProduct.remove(id);
        else
        {
            auto prod_iter = std::find_if(claim->listProduct.cbegin(), claim->listProduct.cend(), [id] (Product p) { return p.id == id;});
            if(prod_iter != claim->listProduct.cend())
                listDelProduct.insert(id, *prod_iter);
        }
    }

    else
    {
        if(listAddModul.contains((id)))
            listAddModul.remove(id);
        else
        {
            auto mod_iter = std::find_if(claim->listModul.cbegin(), claim->listModul.cend(), [id] (Modul m) { return m.id == id;});
            if(mod_iter != claim->listModul.cend())
                listDelModul.insert(id, *mod_iter);
        }
    }

    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

