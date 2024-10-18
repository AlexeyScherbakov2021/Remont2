#ifndef SHIPWINDOW_H
#define SHIPWINDOW_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <models/product.h>
#include <models/shipment.h>

namespace Ui {
class ShipWindow;
}

class ShipWindow : public QDialog
{
    Q_OBJECT
    enum TypeItemTree {SET, PRODUCT, MODUL};


public:
    explicit ShipWindow(Shipment *ship, QWidget *parent = nullptr);
    ~ShipWindow();

private slots:
    void on_tbNumProd_clicked();
    void on_tbNumModul_clicked();
    void on_tbAddSetterProd_clicked();
    // void on_pbAddSet_clicked();
    void on_pbDelete_clicked();
    void on_pbFinish_clicked();
    void on_ShipWindow_finished(int result);
    void on_pbClose_clicked();
    void slotIsEditing();
    void on_tbAddSetter_clicked();

    void on_tbAddSetProd_clicked();

private:
    Ui::ShipWindow *ui;
    bool isEditing = false;
    RepoMSSQL repo;
    Shipment *ship;
    QHash<int, int> listID;
    QMap<int, QString> listOrg;

    QTreeWidgetItem* AddItemProd(Product &prod, QTreeWidgetItem *parent = nullptr);
    QTreeWidgetItem* AddItemTree(QString text, int id, TypeItemTree type, QTreeWidgetItem *parent = nullptr);
    void SetStatusProduct(QList<Product> &listProduct);
    void SetStatusModules(QList<Modul> &listModules);
};

#endif // SHIPWINDOW_H
