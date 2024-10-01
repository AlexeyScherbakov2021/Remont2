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
class Status;
class Claim;

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
    bool AddItem(Claim &claim);

    bool UpdateItem(Product &prod);
    bool UpdateItem(Modul &mod);
    bool UpdateItem(Plate &plate);
    bool UpdateItem(Shipment &ship);
    bool UpdateItem(SetterOut &setter);
    bool UpdateItem(Claim &claim);

    bool DeleteItem(Product &prod);
    bool DeleteItem(Modul &mod);
    bool DeleteItem(Plate &plate);
    bool DeleteItem(Shipment &ship);
    bool DeleteItem(SetterOut &setter);
    bool DeleteItem(Claim &claim);

    Product GetProduct(int id);
    Shipment GetShipment(int id);
    Modul GetModul(int id);
    SetterOut GetSetter(int id);

    void FindItems(const QString &number, QList<Product> &listProduct, int status = 0);
    void FindItems(const QString &number, QList<Modul> &listModul, int status = 0);
    void FindItems(const QString &number, QList<Plate> &listPlate, int status = 0);

    void FindItems(QList<Product> &listProduct, int status = 0);
    void FindItems(QList<Modul> &listModul, int status = 0);
    void FindItems(QList<Plate> &listPlate, int status = 0);
    void FindItems(QList<Shipment> &listShip, int status = 0);

    void LoadProductType(QMap<int, QString> &listTypeProduct);

    void LoadModuleType(QMap<int, QString> &listTypeModule);

    void LoadStatus(Product &prod);
    void LoadStatus(Modul &modul);

    bool LinkPlate(int idPlate, int idModul);
    void LoadChildProduct(Product &prod);
    void LoadChildSetter(SetterOut &setter);
    void LoadShipment(QList<Shipment> &listShip, bool isFinish);
    void LoadShipSetter(QList<SetterOut> &listSetter, int idShip);
    void LoadShipModule(QList<Modul> &listModul, int idShip);
    void LoadShipProduct(QList<Product> &listProduct, int idShip);

    bool AddStatus(Modul &modul, Status &status);
    bool AddStatus(Product &product, Status &status);

    void LoadOrganization(QMap<int, QString> &listOrg);

    void LoadClaim(QList<Claim> &listClaim);
    void LoadClaimType(QMap<int, QString> &listTypeClaim);
    void LoadClaimModules(int idClaim, QList<Modul> &listModul);
    void LoadClaimProducts(int idClaim, QList<Product> &listProduct);
    bool AddModulToClaim(int idModul, int idClaim);
    bool AddProductToClaim(int idProd, int idClaim);
};



#endif // REPOMSSQL_H
