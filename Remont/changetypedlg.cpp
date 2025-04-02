#include "changetypedlg.h"
#include "ui_changetypedlg.h"

ChangeTypeDlg::ChangeTypeDlg(Items *_item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeTypeDlg), dev(_item)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    QString sql = QString("select id,VNFT,typeName from ItemType where indexType=%1 and VNFT is not null order by VNFT").arg(dev->type.indexType);
    model->setQuery(sql);
    model->setHeaderData(1, Qt::Horizontal, "ВНФТ");
    model->setHeaderData(2, Qt::Horizontal, "Наименование");
    model->setHeaderData(1, Qt::Horizontal, "ВНФТ");
    model->setHeaderData(2, Qt::Horizontal, "Наименование");
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    ui->tblView->setModel(proxy);
    ui->tblView->hideColumn(0);
    ui->tblView->setColumnWidth(1, 200);

    connect(ui->tblView, &QTableView::doubleClicked, this, &ChangeTypeDlg::slotSelect);
    connect(ui->pbSelect, &QPushButton::clicked, this, &ChangeTypeDlg::slotSelect);
}

ChangeTypeDlg::~ChangeTypeDlg()
{
    delete ui;
}

void ChangeTypeDlg::slotSelect()
{
    QModelIndex index = ui->tblView->currentIndex();
    index = proxy->index(index.row(), 0);
    // index = proxy->mapToSource(index);
    dev->idType = proxy->data(index).toInt();

    dev->type.id = dev->idType;

    index = proxy->index(index.row(), 1);
    dev->type.VNFT = proxy->data(index).toString();
    index = proxy->index(index.row(), 2);
    dev->type.typeName = proxy->data(index).toString();
    accept();

}

void ChangeTypeDlg::on_tbSearch_clicked()
{
    // proxy->setFilterFixedString(ui->leSearch->text());
    // proxy->setFilterWildcard(ui->leSearch->text());
    proxy->setFilterRegularExpression(ui->leSearch->text());
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(2);
    qDebug() << ui->leSearch->text();
}

