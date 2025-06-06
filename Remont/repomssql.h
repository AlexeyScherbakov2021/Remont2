#ifndef REPOMSSQL_H
#define REPOMSSQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFutureInterface>
#include <QPromise>
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
class Users;

struct optionSearch {
    QString numberDoc;
    QString number2;
    QString name;
    QString descript;
    QString cardOrder;
    QString schet;
    QString dogovor;
    QString objInstall;
    QString numRelease;
};


class RepoMSSQL
{
private:
    QSqlDatabase db;
    QString instanceName;

public:
    RepoMSSQL(const QString& threadName = QSqlDatabase::defaultConnection);
    ~RepoMSSQL();
    bool ConnectDb();

    // Items
    bool LoadChildItems(int idParent, QList<Items> &listItems) const;
    int LoadPart(int start, int count, IndexType iType, const QString &number,
                    QList<Items> &listItems, const QVector<StatusItem>& listStatus, bool isBusy = false, LoadPartType hasParent = LoadPartType::NO_HAS_PARENT) const;

    int LoadPartAll(int start, int count, const QString &number,
                    QList<Items> &listItems, QVector<StatusItem>& listStatus, bool isBusy = false, LoadPartType hasParent = LoadPartType::NO_HAS_PARENT) const;

    bool loadSearch(optionSearch &opt, QList<Items> &listItems);

    bool AddItem(Items &item) const;
    bool UpdateItem(Items &item);
    bool DeleteItem(int id) const;
    Items GetItem(int id) const;
    Items GetItem2(QString number, const QVector<StatusItem>& listStatus, bool isBusy = false, bool isParent = false) const;
    void LoadItemsType(QList<ItemType> &listType, IndexType indexType) const;
    void LoadNewItemsType(QVector<ItemType> &listType, IndexType indexType) const;
    void LoadStatus(Items& item) const;
    bool AddStatus(Items &item, Status &status) const;
    bool DelLastStatus(Items &item, StatusItem status) const;
    bool LoadHistoryChild(int idParent, QList<Items> &listItems) const;
    // void LoadTypeItem(IndexType indexType, QVector<ItemType> &listType) const;       //=============

    // SetterOut
    int LoadPart(int start, int count, const QString &number, QList<SetterOut> &listItems, bool isBusy = false) const;
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
    int LoadPart(int start, int count, const QString &number, QList<Shipment> &listItems, bool isShip = false) const;
    void LoadChildShip(Shipment &ship);
    bool ItemsSyncShip(int idShip, TrackRecord<Items> *track);
    bool SetsSyncShip(int idShip, TrackRecord<SetterOut> *track);
    Shipment GetShipment(int id);

    //Claim
    // bool LoadClaim(const QString number, QList<Claim> &listClaim);
    void LoadClaimType(QMap<int, QString> &listTypeClaim);
    Claim GetClaim(int id);
    bool AddItem(Claim &claim);
    bool UpdateItem(Claim &claim);
    bool DeleteClaim(int id);
    int LoadPart(int start, int count, const QString &number, QList<Claim> &listItems) const;
    bool LoadChildClaim(Claim &claim);
    bool AddItemToClaim(int idItem, int idClaim);
    bool SetEndWorkClaim(int idItem, int idClaim);
    bool DelItemFromClaim(int idItem, int idClaim);
    Claim GetClaimForItem(int idItem);

    // Remont
    void LoadRemontReason(QMap<int, QString> &listReason);
    QString GetRemontReason(int id);
    bool AddRemont(Remont &remont);
    bool UpdateRemont(Remont &remont);
    void LoadRemont(QList<Remont> &list, int idItem);
    Remont GetRemontForItem(int id, int idClaim);
    Remont GetCurrentRemontForItem(int id);
    void LoadRemontPrevReason(QMap<QString, int> &listReason);
    bool DeleteRemont(int id);


    // Organization
    Organization GetOrganization(int id);

    void LoadOrganization(QList<Organization> &listOrg);
    void LoadOrganizationAsync(QList<Organization> &listOrg, QPromise<void> &promise);

    int GetTypeStatus(int idStatus);
    const QString GetNameStatus(int id);

    void LoadShipSetter(QList<SetterOut> &listSetter, int idShip);

    int GetCountRegisterPlate(QString numDoc, int idType);

    int32_t GetNextNumber(uint year);
    void CreateGenerator(uint year);
    void RestartSerialNumber(uint year);
    int GetCurrentNumber(uint year);

    // User Role
    bool LoadRolesUser(int idUser, QSet<RolesType> &roles);
    void LoadRoles(QMap<RolesType, QString> &roles);
    void LoadUsers(QList<Users> &listUsers);
    bool SaveUserRoles(Users &user);
    Users LoadUser(int idUser);

    bool loadAllSatus(QMap<QString, int> &listStatus);
};

#endif
