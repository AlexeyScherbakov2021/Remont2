#ifndef SHIPWINDOW_H
#define SHIPWINDOW_H

#include <QDialog>
#include <QTreeWidgetItem>
// #include <models/product.h>
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
    void on_pbDelete_clicked();
    void on_pbFinish_clicked();
    // void on_ShipWindow_finished(int result);
    void slotIsEditing();
    void on_tbAddSetter_clicked();
    void on_pbSave_clicked();

private:
    Ui::ShipWindow *ui;
    bool isEditing = false;
    RepoMSSQL repo;
    Shipment *ship;
    QHash<int, int> listID;
    QMap<int, QString> listOrg;
    TrackRecord<Items> trackItem;
    TrackRecord<SetterOut> trackSet;

};

#endif // SHIPWINDOW_H
