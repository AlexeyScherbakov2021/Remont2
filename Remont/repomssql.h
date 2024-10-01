#ifndef REPOMSSQL_H
#define REPOMSSQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

// #include <models/product.h>
// #include <models/setterout.h>
// #include <models/shipment.h>
// #include <models/modul.h>

class Product;
class Modul;
class Plate;
class SetterOut;
class Shipment;

class RepoMSSQL
{
private:
    QSqlDatabase db;

public:
    RepoMSSQL();
    bool ConnectDb();

    bool AddItem(Product &prod);
    bool AddItem(Modul &mod);
    bool AddItem(Plate &plate);
    bool AddItem(Shipment &ship);
    bool AddItem(SetterOut &setter);

    bool UpdateItem(Product &prod);
    bool UpdateItem(Modul &mod);
    bool UpdateItem(Plate &plate);
    bool UpdateItem(Shipment &ship);
    bool UpdateItem(SetterOut &setter);

    bool DeleteItem(Product &prod);
    bool DeleteItem(Modul &mod);
    bool DeleteItem(Plate &plate);
    bool DeleteItem(Shipment &ship);
    bool DeleteItem(SetterOut &setter);

    Product GetProduct(int id);
    Shipment GetShipment(int id);
    Modul GetModul(int id);
    SetterOut GetSetter(int id);

    void FindItems(const QString &number, QList<Product> &listProduct, int status = 0);
    void FindItems(const QString &number, QList<Modul> &listModul, int status = 0);
    void FindItems(const QString &number, QList<Plate> &listlate, int status = 0);

    void FindItems(QList<Product> &listProduct, int status = 0);
    void FindItems(QList<Modul> &listModul, int status = 0);
    void FindItems(QList<Plate> &listlate, int status = 0);

    void LoadProductType(QMap<int, QString> &listTypeProduct);

    void LoadModuleType(QMap<int, QString> &listTypeModule);

    void LoadStatus(Product &prod);
    void LoadStatus(Modul &modul);

    bool LinkPlate(int idPlate, int idModul);
    void LoadChildProduct(Product &prod);
    void LoadChildSetter(SetterOut &setter);
    void LoadShipment(QList<Shipment> &listShip, bool isFinish);

};


#endif // REPOMSSQL_H
