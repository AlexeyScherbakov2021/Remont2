#ifndef ITEMVNFTWINDOW_H
#define ITEMVNFTWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <qtablewidget.h>
#include <models/ItemsType.h>

namespace Ui {
class ItemVNFTWindow;
}

class ItemVNFTWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ItemVNFTWindow(IndexType t, QWidget *parent = nullptr);
    ~ItemVNFTWindow();

private slots:
    // void on_twVNFT_itemChanged(QTableWidgetItem *item);
    void on_tbAdd_clicked();
    void on_tbDelete_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();

private:
    QStringList nameWindow {"изделий","модулей","плат"};
    Ui::ItemVNFTWindow *ui;
    RepoMSSQL repo;
    QSqlTableModel model2;
    IndexType workType;
};

#endif // ITEMVNFTWINDOW_H
