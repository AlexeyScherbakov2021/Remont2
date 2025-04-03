#include "cardprodwindow.h"
#include "treeitemsform.h"
#include "ui_treeitemsform.h"

#include <models/Items.h>
#include <models/setterout.h>

#include <QClipboard>
#include <QMessageBox>

TreeItemsForm::TreeItemsForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TreeItemsForm)
{
    ui->setupUi(this);

    ui->treeWidget->addAction("Карточка устройства", this, [this] () {

        auto item = ui->treeWidget->currentItem();
        int id = item->data(0, Qt::UserRole).toInt();
        IndexType type = (IndexType)item->data(0, Qt::UserRole + 1).toInt();
        if(id > 0 && type <= IndexType::Plate)
        {
            Items dev = repo.GetItem(id);
            CardProdWindow *win = new CardProdWindow(&dev, this);
            win->exec();
        }

    });
    ui->treeWidget->addAction("Скопировать номер", this, [this] () {
        auto item = ui->treeWidget->currentItem();
        int id = item->data(0, Qt::UserRole).toInt();
        Items dev = repo.GetItem(id);
        QClipboard *cpb = QApplication::clipboard();
        cpb->setText(dev.number, QClipboard::Clipboard);
    });

    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

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
void TreeItemsForm::AddItem(IEntity *dev, bool isRootVisible)
{
    QTreeWidgetItem *item = nullptr;

    Q_ASSERT(device == nullptr);

    device = dev;

    if(isRootVisible)
    {
        QString nameType, nameIcon;
        IndexType type = dev->GetInfo(nameType, nameIcon);
        QStringList name;

        item = new QTreeWidgetItem();

        if(type <= IndexType::Plate )
        {
            Items* devItem = (Items*)dev;
            name << devItem->GetDefaultName();
            repo.LoadChildItems(devItem->id, devItem->childItems);
            if(devItem->listStatus.size() > 0 )
            {
                switch(devItem->listStatus.last().idStatus)
                {
                case StatusItem::FAULTY_ON_OBJECT:
                    item->setForeground(0, QBrush(Qt::red));
                    name << " неисправен";
                    break;

                case StatusItem::FAULTY_CHILD:
                    item->setForeground(0, QBrush(Qt::darkGray));
                    name << " неиспр.комплектующие";
                    break;
                }
            }

        }
        else if(type == IndexType::SetterType)
        {
            SetterOut* setter = (SetterOut*)dev;
            name << setter->name << "Карта заказа" << setter->numberDoc;
            repo.LoadChildSetter(*setter);
        }

        item->setText(0, name.join(" "));
        item->setIcon(0, QIcon(nameIcon));
        item->setData(0, Qt::UserRole, dev->id);
        item->setData(0, Qt::UserRole + 1, type);
        item->setToolTip(0, nameType);
        ui->treeWidget->addTopLevelItem(item);
        item->setExpanded(true);
    }

    for(auto &it : dev->childItems)
    {
        repo.LoadChildItems(it.id, it.childItems);
        AddChildTree(item, &it);
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

    if(dev->listStatus.size() > 0 )
    {
        if(dev->listStatus.last().typeStatus == TypeStatus::REMONT_STATUS)
        {
            child->setForeground(0, QBrush(Qt::red));
            child->setText(0, dev->GetDefaultName() + " неисправен");
        }
        else
        {
            switch(dev->listStatus.last().idStatus)
            {
            // case StatusItem::FAULTY_ON_OBJECT:
            // case StatusItem::FAULTY_ON_OSO:
            // case StatusItem::REMONT:
            //     child->setForeground(0, QBrush(Qt::red));
            //     child->setText(0, dev->GetDefaultName() + " неисправен");
            //     break;

            // case StatusItem::EXCHANGE:
            //     child->setForeground(0, QBrush(Qt::lightGray));
            //     child->setText(0, dev->GetDefaultName() + " был заменен");
            //     break;

            case StatusItem::FAULTY_CHILD:
                child->setForeground(0, QBrush(Qt::darkGray));
                child->setText(0, dev->GetDefaultName() + " неиспр.комплектующие");
                break;

            case StatusItem::CORRECT_OSO:
                child->setForeground(0, QBrush(Qt::lightGray));
                child->setText(0, dev->GetDefaultName() + " отсутствует");
                break;

            default:
                child->setText(0, dev->GetDefaultName());
                break;
            }
        }



        // if(dev->listStatus.last().idStatus == StatusItem::FAULTY_ON_OBJECT)
        // {
        //     child->setForeground(0, QBrush(Qt::red));
        //     child->setText(0, dev->GetDefaultName() + " неисправен");
        // }
        // if(dev->listStatus.last().idStatus == StatusItem::EXCHANGE)
        // {
        //     child->setForeground(0, QBrush(Qt::lightGray));
        //     child->setText(0, dev->GetDefaultName() + " был заменен");
        // }
    }
    // else
    //     child->setText(0, dev->GetDefaultName());

    child->setData(0, Qt::UserRole, dev->id);
    child->setData(0, Qt::UserRole + 1, dev->type.indexType);
    child->setIcon(0, QIcon(nameIcon));
    child->setToolTip(0, nameType);
    if(root == nullptr)
        ui->treeWidget->addTopLevelItem(child);
    else
        root->addChild(child);

    for(auto &it : dev->childItems)
    {
        repo.LoadChildItems(it.id, it.childItems);
        AddChildTree(child, &it);
    }
}


//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
int TreeItemsForm::GetCurrentRootItemId(IndexType &type)
{
    auto selItem = ui->treeWidget->currentItem();
    if(selItem == nullptr)
        return -1;

    while(selItem->parent() != nullptr)
        selItem = selItem->parent();

    int id = selItem->data(0, Qt::UserRole).toInt();
    type = (IndexType)selItem->data(0, Qt::UserRole + 1).toInt();

    return id;
}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
int TreeItemsForm::DeleteSelectedItem(bool isConfirm)
{
    int id = 0;
    auto selItem = ui->treeWidget->currentItem();
    if(selItem != nullptr)
    {
        while(selItem->parent() != nullptr)
            selItem = selItem->parent();

        // if(isConfirm)
        // {
            if( !isConfirm || (/*isConfirm &&*/ QMessageBox::warning(this, "Предупреждение", QString("Удалить %1 %2 ?").arg(selItem->toolTip(0)).arg(selItem->text(0)),
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes))
            {
                id = selItem->data(0, Qt::UserRole).toInt();
                delete selItem;
            }
        // }
    }

    return id;
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void TreeItemsForm::SetSelectItem(int id, IndexType typeIndex)
{
    QTreeWidgetItem* root = ui->treeWidget->topLevelItem(0);
    SetSelectItemRec(id, typeIndex, root);
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
int TreeItemsForm::GetSelectedId()
{
    auto selItem = ui->treeWidget->currentItem();
    return selItem->data(0, Qt::UserRole).toInt();
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
QPair<int,IndexType> TreeItemsForm::GetSelectedItem()
{
    QPair<int,IndexType> pair;
    auto selItem = ui->treeWidget->currentItem();
    pair.first = selItem->data(0, Qt::UserRole).toInt();
    pair.second = (IndexType)selItem->data(0, Qt::UserRole + 1).toInt();
    return pair;
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void TreeItemsForm::ExecMenu(QMenu &menu, const QPoint &pos)
{
    // qDebug() << "ExecMenu";
    menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void TreeItemsForm::AddHistoryChild()
{
    QList<Items> listItems;
    repo.LoadHistoryChild(device->id, listItems);

    QTreeWidgetItem *root = ui->treeWidget->topLevelItem(0);

    for(auto &it : listItems)
    {
        QString nameType, nameIcon;
        it.GetInfo(nameType, nameIcon);

        QTreeWidgetItem *child = new QTreeWidgetItem();
        // child->setForeground(0, QBrush(Qt::lightGray));
        child->setForeground(0, QColor(140,100,100));
        child->setText(0, it.GetDefaultName() + " был заменен " + it.dateOff.toString("dd.MM.yyyy"));
        child->setData(0, Qt::UserRole, it.id);
        child->setData(0, Qt::UserRole + 1, it.type.indexType);
        child->setIcon(0, QIcon(nameIcon));
        child->setToolTip(0, nameType);
        root->addChild(child);
    }
}


//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
bool TreeItemsForm::SetSelectItemRec(int id, IndexType typeIndex, QTreeWidgetItem* item)
{
    int _id = item->data(0, Qt::UserRole).toInt();
    int _type = item->data(0, Qt::UserRole + 1).toInt();
    if(_id == id && typeIndex == _type)
    {
        QFont font;
        font.setBold(true);
        item->setFont(0, font);
        return true;
    }

    for(int index = 0; index < item->childCount(); ++index)
    {
        QTreeWidgetItem* child = item->child(index);
        if( SetSelectItemRec(id, typeIndex, child))
            return true;
    }

    return false;
}




