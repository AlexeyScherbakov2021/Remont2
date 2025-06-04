#ifndef TREEITEMSFORM_H
#define TREEITEMSFORM_H

#include "repomssql.h"

#include <QMenu>
#include <QTreeWidgetItem>
#include <QWidget>

#include <infrastructure/general.h>
#include <models/entity.h>

namespace Ui {
class TreeItemsForm;
}

class TreeItemsForm : public QWidget
{
    Q_OBJECT

public:
    explicit TreeItemsForm(QWidget *parent = nullptr);
    ~TreeItemsForm();

    void AddItem(IEntity* item, bool isRootVisible = true);
    int GetCurrentRootItemId(IndexType& type);
    int DeleteSelectedItem(bool isConfirm = true);
    void SetSelectItem(int id, IndexType typeIndex);
    int GetSelectedId();
    QPair<int,IndexType> GetSelectedItem();
    void ExecMenu(QMenu &menu, const QPoint &pos);
    void AddHistoryChild();
    void Clear();
    void setExpand(bool isExpand = true);

private:
    Ui::TreeItemsForm *ui;
    RepoMSSQL repo;
    IEntity *device = nullptr;

    void AddChildTree(QTreeWidgetItem *item, Items* dev);
    bool SetSelectItemRec(int id, IndexType typeIndex, QTreeWidgetItem* item);
};

#endif // TREEITEMSFORM_H
