#ifndef CARDPRODWINDOW_H
#define CARDPRODWINDOW_H

#include "repomssql.h"

#include <QDialog>

#include <models/modul.h>
#include <models/product.h>
// #include <models/remontentity.h>
#include <models/setterout.h>
#include <models/shipment.h>


namespace Ui {
class CardProdWindow;
}

class CardProdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CardProdWindow(Modul &modul, QWidget *parent = nullptr);
    explicit CardProdWindow(Product &prod, QWidget *parent = nullptr);
    ~CardProdWindow();

private slots:
    void on_pbAddRemont_clicked();
    void on_pbEditStatus_clicked();

private:
    explicit CardProdWindow(const QString &name, QWidget *parent = nullptr);
    Ui::CardProdWindow *ui;
    RepoMSSQL repo;
    // Modul modul;
    QString number;

    // RemontEntity *remontEntity;

    void loadShipmentToForm(const Product *prod);
    void loadInclude(const Product *prod);
    // void AddRowRemont(RemontM &rem);
    void LoadHistoryToForm(QList<Status> listStatus);
};

#endif // CARDPRODWINDOW_H
