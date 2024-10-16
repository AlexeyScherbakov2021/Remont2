#ifndef CARDPRODWINDOW_H
#define CARDPRODWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <models/modul.h>
#include <models/product.h>
#include <models/setterout.h>
#include <models/shipment.h>

namespace Ui {
class CardProdWindow;
}

class CardProdWindow : public QDialog
{
    Q_OBJECT

public:
    // explicit CardProdWindow(Modul &modul, QWidget *parent = nullptr);
    // explicit CardProdWindow(Product &prod, QWidget *parent = nullptr);
    CardProdWindow(IDevice *device, QWidget *parent = nullptr);
    ~CardProdWindow();

private:
    explicit CardProdWindow(const QString &name, QWidget *parent = nullptr);
    Ui::CardProdWindow *ui;
    RepoMSSQL repo;
    QString number;

    // RemontEntity *remontEntity;

    void loadShipmentToForm(const Product *prod);
    void loadInclude(const Product *prod);
    // void AddRowRemont(RemontM &rem);
    void LoadHistoryToForm(QList<Status> &listStatus);
    void LoadRemontToForm(QList<Remont> &listRemmont);
};

#endif // CARDPRODWINDOW_H
