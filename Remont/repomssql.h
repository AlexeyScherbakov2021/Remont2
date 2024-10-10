#ifndef REPOMSSQL_H
#define REPOMSSQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Product;
class Modul;
class Plate;
class SetterOut;
class Shipment;
class Status;
class Claim;
class ProductType;
class ModulType;

class RepoMSSQL
{
private:
    QSqlDatabase db;
    void FindItems(QList<Product> &listProduct, int status = 0, bool isFree = true);
    void FindItems(QList<Modul> &listModul, int status = 0, bool isFree = true);
    // void FindItems(QList<Plate> &listPlate, int status = 0);
    void FindItems(QList<Shipment> &listShip, int status = 0);

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

    bool DeleteProduct(int id);
    bool DeleteModul(int id);
    bool DeletePlate(int id);
    bool DeleteShipment(int id);
    bool DeleteSetter(int id);
    bool DeleteClaim(int id);

    Product GetProduct(int id);
    Shipment GetShipment(int id);
    Modul GetModul(int id);
    SetterOut GetSetter(int id);

    void FindItems(const QString &number, QList<Product> &listProduct, int status = 0, bool isFree = true);
    void FindItems(const QString &number, QList<Modul> &listModul, int status = 0, bool isFree = true);
    void FindItems(const QString &number, QList<Plate> &listPlate, int status = 0, bool isFree = true);
    void FindItems(const QString &number, QList<Shipment> &listShip, int status = 0);

    // void LoadProductType(QMap<int, QString> &listTypeProduct);
    void LoadProductType(QMap<int, ProductType> &listTypeProduct);

    // void LoadModuleType(QMap<int, QString> &listTypeModule);
    void LoadModuleType(QMap<int, ModulType> &listTypeModule);

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
    bool DelLastStatus(Modul &modul);
    bool DelLastStatus(Product &product);

    void LoadOrganization(QMap<int, QString> &listOrg);

    void LoadClaim(QList<Claim> &listClaim);
    void LoadClaimType(QMap<int, QString> &listTypeClaim);
    void LoadClaimModules(int idClaim, QList<Modul> &listModul);
    void LoadClaimProducts(int idClaim, QList<Product> &listProduct);
    bool AddModulToClaim(int idModul, int idClaim);
    bool DelModulFromClaim(int idModul, int idClaim);
    bool AddProductToClaim(int idProd, int idClaim);
    bool DelProductToClaim(int idProd, int idClaim);
    bool LoadClaimForProduct(int ProdId, Claim &claim);
    bool LoadClaimForModul(int ModulId, Claim &claim);

    void LoadRemontReason(QMap<int, QString> &listReason);
};



#endif // REPOMSSQL_H
