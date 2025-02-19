#ifndef REPOMSSQL_H
#define REPOMSSQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <models/ItemsType.h>
#include <models/enumvariable.h>
// #include <infrastructure/IStatus.h>
#include <infrastructure/trackrecord.h>
#include <infrastructure/general.h>

class SetterOut;
class Shipment;
class Status;
class Claim;
class Remont;
class Organization;
class Items;

class RepoMSSQL
{
private:
    QSqlDatabase db;
    void FindItems(QList<Shipment> &listShip, int status = 0, bool isFree = false);
    void FindItems(IndexType iType, QList<Items> &listItems, int status = 0, bool isFree = false);

public:

    // Items
    bool LoadChildItems(int idParent, QList<Items> &listItems) const;
    int LoadPart(size_t start, size_t count, IndexType iType, const QString &number,
                    QList<Items> &listItems, const QVector<StatusItem>& listStatus, bool isBusy = false, bool isParent = false) const;

    int LoadPartAll(size_t start, size_t count, const QString &number,
                    QList<Items> &listItems, QVector<StatusItem>& listStatus, bool isBusy = false, bool isParent = false) const;

    bool AddItem(Items &item) const;
    void FindItems(IndexType iType, const QString &number, QList<Items> &listItems, int status = 0, bool isFree = false);
    bool UpdateItem(Items &item);
    bool DeleteItem(int id) const;
    Items GetItem(int id) const;
    Items GetItem2(QString number, QVector<StatusItem>& listStatus, bool isBusy = false, bool isParent = false) const;
    void LoadItemsType(QList<ItemType> &listType, IndexType indexType) const;

    void LoadStatus(Items& item) const;
    bool AddStatus(Items &item, Status &status) const;
    bool DelLastStatus(Items &item) const;

    void LoadTypeItem(IndexType indexType, QVector<ItemType> &listType) const;       //=============


    // SetterOut
    int LoadPart(size_t start, size_t count, const QString &number, QList<SetterOut> &listItems, bool isBusy = false) const;
    bool AddItem(SetterOut &setter);
    bool UpdateItem(SetterOut &setter);
    bool DeleteSetter(int id);
    void LoadChildSetter(SetterOut &setter);
    SetterOut GetSetter(int id);
    bool ItemsSyncSet(int idSet, TrackRecord<Items> *track);

    // Shipment
    bool AddItem(Shipment &ship);
    bool UpdateItem(Shipment &ship);
    bool DeleteShipment(int id);
    int LoadPart(size_t start, size_t count, const QString &number, QList<Shipment> &listItems, bool isShip = false) const;
    void LoadChildShip(Shipment &ship);
    bool ItemsSyncShip(int idShip, TrackRecord<Items> *track);
    bool SetsSyncShip(int idShip, TrackRecord<SetterOut> *track);


    void FindItems(const QString &number, QList<Shipment> &listShip, int status = 0, bool isFree = false);
    void LoadShipment(QList<Shipment> &listShip, bool isFinish);


public:
    RepoMSSQL();
    bool ConnectDb();

    bool AddItem(Claim &claim);
    bool UpdateItem(Claim &claim);
    bool DeleteClaim(int id);
    Shipment GetShipment(int id);

    int GetTypeStatus(int idStatus);
    const QString GetNameStatus(int id);

    bool LinkPlate(int idPlate, int idModul);
    void LoadShipSetter(QList<SetterOut> &listSetter, int idShip);


    // Organization
    Organization GetOrganization(int id);
    void LoadOrganization(QMap<int, QString> &listOrg);
    void LoadOrganization(QList<Organization> &listOrg);

    bool LoadClaim(const QString number, QList<Claim> &listClaim);
    void LoadClaimType(QMap<int, QString> &listTypeClaim);
    Claim GetClaim(int id);

    void LoadRemontReason(QMap<int, QString> &listReason);
    QString GetRemontReason(int id);
    bool AddRemont(Remont &remont, ev::DeviceKind kindDevice);
    bool UpdateRemont(Remont &remont, ev::DeviceKind kindDevice);
    void LoadRemont(QList<Remont> &list, int idParent, ev::DeviceKind kindDevice);
    Remont GetCurrentRemont(int id, ev::DeviceKind kindDevice);

    int GetCountRegisterPlate(QString numDoc, int idType);

    int32_t GetNextNumber(uint year);
    void CreateGenerator(uint year);
    void RestartSerialNumber(uint year);
    int GetCurrentNumber(uint year);
};

#endif
