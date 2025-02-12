#ifndef REPOMSSQL_H
#define REPOMSSQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <models/ItemsType.h>
#include <models/enumvariable.h>

// class Product;
// class Modul;
// class Plate;
class SetterOut;
class Shipment;
class Status;
class Claim;
// class ProductType;
// class ModulType;
class Remont;
// class PlateType;
class Organization;
class Items;
// class ItemType;

class RepoMSSQL
{
private:
    QSqlDatabase db;
    // void FindItems(QList<Product> &listProduct, int status = 0, bool isFree = false);
    // void FindItems(QList<Modul> &listModul, int status = 0, bool isFree = false);
    // void FindItems(QList<Plate> &listPlate, int status = 0);
    void FindItems(QList<Shipment> &listShip, int status = 0, bool isFree = false);

    void FindItems(ItemType::IndexType iType, QList<Items> &listItems, int status = 0, bool isFree = false);

public:
    // size_t LoadPart(size_t start, size_t count, ItemType::IndexType iType, const QString &number,
    //             QList<Items> &listItems, int status = 0, bool isBusy = false, bool isParent = false);

    bool LoadChildItems(int idParent, QList<Items> &listItems) const;


    int LoadPart(size_t start, size_t count, ItemType::IndexType iType, const QString &number,
                    QList<Items> &listItems, QVector<int>& listStatus, bool isBusy = false, bool isParent = false) const;

    int LoadPartAll(size_t start, size_t count, const QString &number,
                    QList<Items> &listItems, QVector<int>& listStatus, bool isBusy = false, bool isParent = false) const;

    bool AddItem(Items &item) const;
    void FindItems(ItemType::IndexType iType, const QString &number, QList<Items> &listItems, int status = 0, bool isFree = false);
    bool UpdateItem(Items &item);
    bool DeleteItem(int id) const;
    Items GetItem(int id) const;
    // Items GetItem(QString number, int status = 0, bool isBusy = false) const;
    Items GetItem2(QString number, QVector<int>& listStatus, bool isBusy = false, bool isParent = false) const;

    void LoadItemsType(QList<ItemType> &listType, ItemType::IndexType indexType) const;

    void LoadStatus(Items& item) const;
    bool AddStatus(Items &item, Status &status) const;
    bool DelLastStatus(Items &item) const;

    void LoadTypeItem(ItemType::IndexType indexType, QVector<ItemType> &listType) const;       //=============


public:
    RepoMSSQL();
    bool ConnectDb();

    bool AddItem(Shipment &ship);
    bool AddItem(SetterOut &setter);
    bool AddItem(Claim &claim);

    bool UpdateItem(Shipment &ship);
    bool UpdateItem(SetterOut &setter);
    bool UpdateItem(Claim &claim);

    bool DeleteShipment(int id);
    bool DeleteSetter(int id);
    bool DeleteClaim(int id);

    Shipment GetShipment(int id);
    SetterOut GetSetter(int id);

    void FindItems(const QString &number, QList<Shipment> &listShip, int status = 0, bool isFree = false);

    // void LoadProductType(QMap<int, QString> &listTypeProduct);
    // void LoadProductType(QMap<int, ProductType> &listTypeProduct);

    // void LoadModuleType(QMap<int, QString> &listTypeModule);
    // void LoadModuleType(QMap<int, ModulType> &listTypeModule);

    int GetTypeStatus(int idStatus);
    const QString GetNameStatus(int id);

    bool LinkPlate(int idPlate, int idModul);
    // void LoadChildProduct(Product &prod);
    void LoadChildSetter(SetterOut &setter);
    void LoadShipment(QList<Shipment> &listShip, bool isFinish);
    void LoadShipSetter(QList<SetterOut> &listSetter, int idShip);
    // void LoadShipModule(QList<Modul> &listModul, int idShip);
    // void LoadShipProduct(QList<Product> &listProduct, int idShip);


    void LoadOrganization(QMap<int, QString> &listOrg);
    void LoadOrganization(QList<Organization> &listOrg);

    void LoadClaim(QList<Claim> &listClaim);
    void LoadClaimType(QMap<int, QString> &listTypeClaim);
    // void LoadClaimModules(int idClaim, QList<Modul> &listModul);
    // void LoadClaimProducts(int idClaim, QList<Product> &listProduct);
    // bool AddModulToClaim(int idModul, int idClaim);
    // bool DelModulFromClaim(int idModul, int idClaim);
    // bool AddProductToClaim(int idProd, int idClaim);
    // bool DelProductToClaim(int idProd, int idClaim);
    // bool LoadClaimForProduct(int ProdId, Claim &claim);
    // bool LoadClaimForModul(int ModulId, Claim &claim);
    Claim GetClaim(int id);

    void LoadRemontReason(QMap<int, QString> &listReason);
    QString GetRemontReason(int id);
    bool AddRemont(Remont &remont, ev::DeviceKind kindDevice);
    bool UpdateRemont(Remont &remont, ev::DeviceKind kindDevice);
    void LoadRemont(QList<Remont> &list, int idParent, ev::DeviceKind kindDevice);
    Remont GetCurrentRemont(int id, ev::DeviceKind kindDevice);

    // void LoadTypePlate(QVector<PlateType> &listType);       //=============
    int GetCountRegisterPlate(QString numDoc, int idType);
    // int GetCountRegisterModul(QString numDoc, int idType);
    // int GetCountRegisterProduct(QString numDoc, int idType);

    int32_t GetNextNumber(uint year);
    void CreateGenerator(uint year);
    void RestartSerialNumber(uint year);
    int GetCurrentNumber(uint year);
};

#endif
