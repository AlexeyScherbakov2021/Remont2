#include "treeitemsform.h"
#include "ui_treeitemsform.h"

#include <models/Items.h>
#include <models/setterout.h>

#include <QMessageBox>

TreeItemsForm::TreeItemsForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TreeItemsForm)
{
    ui->setupUi(this);
}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
TreeItemsForm::~TreeItemsForm()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
void TreeItemsForm::AddItem(IEntity *dev)
{
    QString nameType, nameIcon;
    IndexType type = dev->GetInfo(nameType, nameIcon);
    QStringList name;

    if(type <= IndexType::Plate )
    {
        Items* devItem = (Items*)dev;
        name << devItem->GetDefaultName();
        repo.LoadChildItems(devItem->id, devItem->childItems);
    }
    else if(type == IndexType::SetterType)
    {
        SetterOut* setter = (SetterOut*)dev;
        name << setter->name << "Карта заказа" << setter->numberDoc;
        repo.LoadChildSetter(*setter);
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name.join(" "));
    item->setIcon(0, QIcon(nameIcon));
    item->setData(0, Qt::UserRole, dev->id);
    item->setData(0, Qt::UserRole + 1, type);
    item->setToolTip(0, nameType);
    ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    for(auto &it : dev->childItems)
    {
        repo.LoadChildItems(it.id, it.childItems);
        AddChildTree(item, &it);
    }
}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
int TreeItemsForm::GetCurrentRootItem(IndexType &type)
{
    auto selItem = ui->treeWidget->currentItem();
    while(selItem->parent() != nullptr)
        selItem = selItem->parent();

    int id = selItem->data(0, Qt::UserRole).toInt();
    type = (IndexType)selItem->data(0, Qt::UserRole + 1).toInt();

    return id;
}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
bool TreeItemsForm::DeleteSelectedItem(bool isConfirm)
{
    auto selItem = ui->treeWidget->currentItem();
    if(selItem != nullptr)
    {
        while(selItem->parent() != nullptr)
            selItem = selItem->parent();

        if(isConfirm)
        {
            if(QMessageBox::warning(this, "Предупреждение", QString("Удалить %1 %2 ?").arg(selItem->toolTip(0)).arg(selItem->text(0)),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
                return false;
        }

        delete selItem;
        return true;
    }
}


//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
void TreeItemsForm::AddChildTree(QTreeWidgetItem *root, Items* dev)
{
    QString nameType, nameIcon;
    dev->GetInfo(nameType, nameIcon);

    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0, dev->GetDefaultName());
    child->setIcon(0, QIcon(nameIcon));
    child->setToolTip(0, nameType);
    root->addChild(child);

    for(auto &it : dev->childItems)
    {
        repo.LoadChildItems(it.id, it.childItems);
        AddChildTree(child, &it);
    }
}


