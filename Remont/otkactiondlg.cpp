#include "otkactiondlg.h"
#include "ui_otkactiondlg.h"

OtkActionDlg::OtkActionDlg(Items* item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OtkActionDlg), dev(item)
{
    ui->setupUi(this);

    QString nameType;
    QString nameIcon;
    item->GetInfo(nameType, nameIcon);

    ui->gbDevice->setTitle(nameType);
    ui->gbDevice->setWindowIcon(QIcon(nameIcon));
    ui->lbNumber->setText(item->number);
    ui->lbVNFT->setText(item->type.VNFT);
    ui->lbTypeName->setText(item->type.typeName);
    ui->lbDateCreate->setText(item->dateCreate.toString("dd.MM.yyyy"));
    ui->lbGarant->setText(QString::number(item->garantMonth));
    ui->lbStatus->setText(item->currStatus);

    ui->wTreeItem->AddItem(dev);

    // GetTreeChild(item);

    // QTreeWidgetItem *root = new QTreeWidgetItem();
    // root->setText(0, item->GetDefaultName());
    // root->setToolTip(0, item->GetDefaultName());
    // QIcon icon(nameIcon);
    // root->setIcon(0, icon);

    // ui->treeWidget->addTopLevelItem(root);
    // root->setExpanded(true);
    // AddTreeWidget(item, root);

}

OtkActionDlg::~OtkActionDlg()
{
    delete ui;
}


//------------------------------------------------------------------------------
// построение дерева потомков для устройства
//------------------------------------------------------------------------------
// void OtkActionDlg::GetTreeChild(Items *item)
// {
//     repo.LoadChildItems(item->id, item->childItems);
//     for(auto &it : item->childItems)
//     {
//         repo.LoadChildItems(it.id, it.childItems);
//         GetTreeChild(&it);
//     }
// }

// void OtkActionDlg::AddTreeWidget(Items *dev, QTreeWidgetItem *item)
// {
//     QString nameType;
//     QString nameIcon;
//     for(auto &it : dev->childItems)
//     {
//         it.GetInfo(nameType, nameIcon);
//         QTreeWidgetItem *child = new QTreeWidgetItem();
//         child->setText(0, it.GetDefaultName());
//         child->setToolTip(0, it.GetDefaultName());
//         QIcon icon(nameIcon);
//         child->setIcon(0, icon);
//         item->addChild(child);
//         AddTreeWidget(&it, child);
//         child->setExpanded(true);
//     }
// }

void OtkActionDlg::on_pbOk_clicked()
{
    StatusItem stat = ui->rbAcceptControl->isChecked() ? StatusItem::CORRECT : StatusItem::FAULTY;
    if(dev->AddStatus(*dev, stat, ui->leComment->text()))
        accept();
}

