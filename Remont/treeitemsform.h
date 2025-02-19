#ifndef TREEITEMSFORM_H
#define TREEITEMSFORM_H

#include "repomssql.h"

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


    void AddItem(IEntity* item);
    int GetCurrentRootItem(IndexType& type);
    bool DeleteSelectedItem(bool isConfirm = true);

private:
    Ui::TreeItemsForm *ui;
    RepoMSSQL repo;

    void AddChildTree(QTreeWidgetItem *item, Items* dev);
};

#endif // TREEITEMSFORM_H
