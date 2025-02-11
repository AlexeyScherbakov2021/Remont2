#ifndef CREATEPRODWINDOW_H
#define CREATEPRODWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QTreeWidget>

#include <models/ItemsType.h>
// #include <models/modultype.h>
// #include <models/modultype.h>
// #include <models/plate.h>
// #include <models/prodtype.h>
// #include <models/prodtype.h>

namespace Ui {
class CreateProductWindow;
}

class CreateProductWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateProductWindow(QWidget *parent = nullptr);
    ~CreateProductWindow();

private slots:
    void on_tbDeleteProduct_clicked();
    void on_pbRegProduct_clicked();
    void on_cbProduct_currentIndexChanged(int index);
    void slotReadScan(QString s);
    void on_tbDocP_clicked();

private:
    Ui::CreateProductWindow *ui;
    RepoMSSQL repo;
    int countFromDoc = 0;
    int countUse = 0;

    QVector<ItemType> listTypeProduct;
    QMetaObject::Connection conn;
    void UpdateUseCount();
    void addLineContent(Items &prod);


    QList<Items> listAddingPlate;
};

#endif // CREATEPRODWINDOW_H
