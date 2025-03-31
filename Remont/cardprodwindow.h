#ifndef CARDPRODWINDOW_H
#define CARDPRODWINDOW_H

#include "repomssql.h"
#include <QDialog>
// #include <models/modul.h>
// #include <models/product.h>
#include <models/setterout.h>
#include <models/shipment.h>

namespace Ui {
class CardProdWindow;
}

class CardProdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CardProdWindow(Items *device, QWidget *parent = nullptr);
    ~CardProdWindow();

private slots:
    void on_pbToShip_clicked();

private:
    Ui::CardProdWindow *ui;
    RepoMSSQL repo;
    QString number;
    Shipment ship;

    // RemontEntity *remontEntity;

    // void loadShipmentToForm(const Items *prod);
    void loadInclude(const Items *prod);
    // void AddRowRemont(RemontM &rem);
    void LoadHistoryToForm(QList<Status> &listStatus);
    void LoadRemontToForm(int idItem);
    void LoadShipping();
};

#endif // CARDPRODWINDOW_H
