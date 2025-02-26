
#include <models/claim.h>
#include <models/setterout.h>
#include <models/shipment.h>
#include <models/remont.h>
#include <models/organization.h>
#include<models/Items.h>

#include <QSqlDriver>
#include <infrastructure/IStatus.h>
#include "repomssql.h"

class Product;

RepoMSSQL::RepoMSSQL(const QString& threadName) : instanceName(threadName)
{

    if(!QSqlDatabase::contains(threadName))
    {
        db = QSqlDatabase::addDatabase("QODBC", threadName);
        ConnectDb();
    }
}

RepoMSSQL::~RepoMSSQL()
{
    if(db.connectionName() != QSqlDatabase::defaultConnection && !db.connectionName().isEmpty())
    {
        // qDebug() << "BaseClose" << db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(instanceName);
    }
}

bool RepoMSSQL::ConnectDb()
{

#ifdef QT_DEBUG1
    qDebug() << "Отладочная версия.";
    // db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;Encrypt=yes;Trusted_Connection=yes");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;");
    db.setUserName("sa");
    db.setPassword("gonduras");

#else
    qDebug() << "Рабочая версия.";
    // db.setDatabaseName("DRIVER={SQL Server};SERVER=SFP\\FPSQLN;DATABASE=FinGoodsTest2;");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SFP\\FPSQLN;DATABASE=TrackProd;");
    db.setUserName("fpLoginName");
    db.setPassword("ctcnhjt,s");

#endif

    if(!db.open())
    {
        // qDebug() << "Ошибка соединения с базой данных." << db.lastError().text();
        // QMessageBox::critical(0, "Ошибка", "Ошибка соединения с базой данных.");
        std::terminate();
        return false;
    }
    // qDebug() << "Содединение с базой успешно.";
    return true;
}


bool RepoMSSQL::UpdateItem(Items &item)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("update Items set idParent=:idParent,idShip=:idShip,idSet=:idSet,idType=:idType,"
                  "number=:number,number2=:number2,numberDoc=:numberDoc,nameItem=:nameItem,dateCreate=:dateCreate,"
                  "dateOn=:dateOn,dateOff=:dateOff,garantMonth=:garantMonth,dateGarant=:dateGarant,isZip=:isZip "
                  "where id=:id");

    QVariant var = item.idParent > 0 ? item.idParent : QVariant();
    query.bindValue(":idParent", var);
    var = item.idShip > 0 ? item.idShip : QVariant();
    query.bindValue(":idShip", var);
    var = item.idSet > 0 ? item.idSet : QVariant();
    query.bindValue(":idSet", var);
    query.bindValue(":idType", item.idType);
    query.bindValue(":number", item.number);
    query.bindValue(":number2", item.number2);
    query.bindValue(":numberDoc", item.numberDoc);
    query.bindValue(":nameItem", item.name);
    query.bindValue(":dateCreate", item.dateCreate);
    query.bindValue(":dateOn", item.dateOn);
    query.bindValue(":dateOff", item.dateOff);
    query.bindValue(":garantMonth", item.garantMonth);
    query.bindValue(":dateGarant", item.dateGarant);
    query.bindValue(":isZip", item.isZip);
    query.bindValue(":id", item.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в UpdateItem";

    return res;
}

bool RepoMSSQL::DeleteItem(int id) const
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from Items where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в DeleteItem";

    return res;
}



Items RepoMSSQL::GetItem(int id) const
{
    Items item;

    QSqlQuery query(db);

    query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                  "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,it.indexType,it.VNFT,it.garantMonth "
                  "from Items i join ItemType it on it.id=i.idType "
                  "where i.id=:id");

    query.bindValue(":id", id);
    query.exec();
    if(query.next())
    {
        item.id = query.value(0).toInt();
        item.idParent = query.value(1).toInt();
        item.idShip = query.value(2).toInt();
        item.idSet = query.value(3).toInt();
        item.idType = query.value(4).toInt();
        item.number = query.value(5).toString();
        item.number2 = query.value(6).toString();
        item.numberDoc = query.value(7).toString();
        item.name = query.value(8).toString();
        item.dateCreate = query.value(9).toDateTime();
        item.dateOn = query.value(10).toDateTime();
        item.dateOff = query.value(11).toDateTime();
        item.garantMonth = query.value(12).toInt();
        item.dateGarant = query.value(13).toDateTime();
        item.isZip = query.value(14).toBool();
        item.type.typeName = query.value(15).toString();
        item.type.indexType = (IndexType)query.value(16).toInt();
        item.type.VNFT = query.value(17).toString();
        item.type.garantMonth = query.value(18).toInt();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        LoadStatus(item);
    }

    return item;
}



Items RepoMSSQL::GetItem2( QString number, const QVector<StatusItem>& listStatus, bool isBusy, bool isParent) const
{
    Items item;

    QStringList slStatus;
    QSqlQuery query(db);

    QStringList slWhere;
    QString sqlNumber = " number like :number";
    QString sqlBusy = " idShip is null and idSet is null";
    QString sqlParent = " idParent is null";
    QString sqlStatus = "ist.idStatus=:idStatus%1 ";

    QStringList sql = {"select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                       "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,it.indexType,it.VNFT,it.garantMonth "
                       "from Items i join ItemType it on it.id=i.idType "
                       "join (select idItem, max(DateStatus) dateStatus from ItemStatus group by idItem "
                       ") ms on ms.idItem=i.id "
                       "join ItemStatus ist on ist.idItem=ms.idItem and ist.dateStatus=ms.dateStatus "};

    if(!number.isEmpty())
        slWhere.push_back(sqlNumber);

    if(!isBusy)
        slWhere.push_back(sqlBusy);

    if(!isParent)
        slWhere.push_back(sqlParent);

    if(slWhere.size() > 0)
    {
        sql.push_back(" where ");
        sql.push_back(slWhere.join(" and "));
    }

    if(listStatus.size() > 0)
    {
        for(int i = 0; i < listStatus.size(); ++i)
            slStatus.push_back(sqlStatus.arg(i));
        sql.push_back(" and (" + slStatus.join(" or ") + ")");
    }

    QString sql2 = sql.join("");
    query.prepare(sql2);
    query.bindValue(":number", QString("%%1%").arg(number));

    for(int i = 0; i < listStatus.size(); ++i)
    {
        query.bindValue(QString(":idStatus%1").arg(i), listStatus[i]);
    }

    query.exec();
    if(query.next())
    {
        item.id = query.value(0).toInt();
        item.idParent = query.value(1).toInt();
        item.idShip = query.value(2).toInt();
        item.idSet = query.value(3).toInt();
        item.idType = query.value(4).toInt();
        item.number = query.value(5).toString();
        item.number2 = query.value(6).toString();
        item.numberDoc = query.value(7).toString();
        item.name = query.value(8).toString();
        item.dateCreate = query.value(9).toDateTime();
        item.dateOn = query.value(10).toDateTime();
        item.dateOff = query.value(11).toDateTime();
        item.garantMonth = query.value(12).toInt();
        item.dateGarant = query.value(13).toDateTime();
        item.isZip = query.value(14).toBool();
        item.type.typeName = query.value(15).toString();
        item.type.indexType = (IndexType)query.value(16).toInt();
        item.type.VNFT = query.value(17).toString();
        item.type.garantMonth = query.value(18).toInt();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        LoadStatus(item);
        item.SetLastStatus(item.currStatus, item.commentStatus);
    }

    return item;
}

void RepoMSSQL::LoadItemsType(QList<ItemType> &listType, IndexType indexType) const
{
    listType.clear();
    QSqlQuery query(db);
    query.prepare("select id,typeName,garantMonth,VNFT from ItemType where indexType=:indexType");
    query.bindValue(":indexType", indexType);

    query.exec();
    while(query.next())
    {
        ItemType mType;
        mType.id = query.value(0).toInt();
        mType.typeName = query.value(1).toString();
        mType.garantMonth = query.value(2).toInt();
        mType.VNFT = query.value(3).toString();
        listType.push_back(mType);
    }
}


//------------------------------------------------------------------------------------------------------
// Удаление отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteShipment(int id)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from Shipment where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Shipment";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка отгрузок
//------------------------------------------------------------------------------------------------------
int RepoMSSQL::LoadPart(int start, int count, const QString &number, QList<Shipment> &listItems, bool isShip) const
{
    int res = 0;
    QSqlQuery query(db);
    // QStringList slWhere;

    QString sqlNumber = " c_schet like :c_schet";

    QStringList sql = {"select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
                        "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,s.id,o.orgName,c_customer "
                       "from Shipment s "
                       "left join Organization o on o.id=s.idOrganization "
                       "where c_dateUPD is "};

    if(isShip)
        sql << "not null";
    else
        sql << "null";

    if(!number.isEmpty())
    {
        sql << " and " << sqlNumber;
    }

    sql << " order by c_dateUPD desc,c_schet offset :start rows fetch next :count rows only";

    QString sql2 = sql.join("");
    query.prepare(sql2);

    // query.bindValue(":isShip", isShip ? "not null" : "null");

    query.bindValue(":c_schet", QString("%%1%").arg(number));
    query.bindValue(":start", start);
    query.bindValue(":count", count);

    // qDebug() << sql2;

    query.exec();

    while(query.next())
    {
        Shipment ship;
        ship.number = query.value(0).toString();
        ship.objectInstall = query.value(1).toString();
        ship.dateRegister = query.value(2).toDateTime();
        ship.idOrganization = query.value(3).toInt();
        ship.questList = query.value(4).toString();
        ship.schet = query.value(5).toString();
        ship.cardOrder = query.value(6).toString();
        ship.numberUPD = query.value(7).toString();
        ship.buyer = query.value(8).toString();
        ship.dateUPD = query.value(9).toDateTime();
        ship.id = query.value(10).toInt();
        ship.org.orgName = query.value(11).toString();
        ship.customer = query.value(12).toString();
        listItems.push_back(ship);
        ++res;
    }

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка содержимиого отгрузки
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadChildShip(Shipment &ship)
{
    ship.listSetterOut.clear();

    QSqlQuery query(db);
    query.prepare("select id,idShipment,s_name,s_orderNum,dateCreate "
                  "from SetterOut where idShipment = :idShipment");

    query.bindValue(":idShipment", ship.id);
    query.exec();
    while(query.next())
    {
        SetterOut item;

        item.id = query.value(0).toInt();
        item.idShip = query.value(1).toInt();
        item.name = query.value(2).toString();
        item.numberDoc = query.value(3).toString();
        item.dateCreate = query.value(4).toDateTime();
        ship.listSetterOut.push_back(item);
    }

    query.clear();
    ship.childItems.clear();
    query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,"
                  "dateCreate,dateOn,dateOff,i.garantMonth,dateGarant,isZip,it.indexType,it.VNFT,it.typeName "
                  "from Items i join itemType it on it.id=i.idType "
                  "where idShip = :idShip");

    query.bindValue(":idShip", ship.id);

    query.exec();
    while(query.next())
    {
        Items prod;
        prod.id = query.value(0).toInt();
        prod.idParent = query.value(1).toInt();
        prod.idShip = query.value(2).toInt();
        prod.idSet = query.value(3).toInt();
        prod.idType = query.value(4).toInt();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.numberDoc = query.value(7).toString();
        prod.name = query.value(8).toString();
        prod.dateCreate = query.value(9).toDateTime();
        prod.dateOn = query.value(10).toDateTime();
        prod.dateOff = query.value(11).toDateTime();
        prod.garantMonth = query.value(12).toInt();
        prod.dateGarant = query.value(13).toDateTime();
        prod.isZip = query.value(14).toBool();
        prod.type.id = prod.id;
        prod.type.indexType = (IndexType)query.value(15).toInt();
        prod.type.VNFT = query.value(16).toString();
        prod.type.typeName = query.value(17).toString();
        LoadStatus(prod);
        ship.childItems.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Добавление, удаление состава отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::ItemsSyncShip(int idShip, TrackRecord<Items> *track)
{
    bool res = false;

    QSqlDatabase::database().transaction();

    QSqlQuery query(db);
    query.prepare("update Items set idShip=null where id=:id");
    for(auto &it: track->listDel)
    {
        query.bindValue(":id", it.id);
        res = query.exec();
    }

    query.clear();

    query.prepare("update Items set idShip=:idShip where id=:id");
    for(auto &it: track->listAdd)
    {
        query.bindValue(":id", it.id);
        query.bindValue(":idShip", idShip);
        res = query.exec();
    }

    res = QSqlDatabase::database().commit();

    if(!res)
        qDebug() << "Ошибка при изменении записи в ItemsSyncShip";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Добавление, удаление состава отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::SetsSyncShip(int idShip, TrackRecord<SetterOut> *track)
{
    bool res = false;

    QSqlDatabase::database().transaction();

    QSqlQuery query(db);
    query.prepare("update SetterOut set idShipment=null where id=:id");
    for(auto &it: track->listDel)
    {
        query.bindValue(":id", it.id);
        res = query.exec();
    }

    query.clear();

    query.prepare("update SetterOut set idShipment=:idShipment where id=:id");
    for(auto &it: track->listAdd)
    {
        query.bindValue(":id", it.id);
        query.bindValue(":idShipment", idShip);
        res = query.exec();
    }

    res = QSqlDatabase::database().commit();

    if(!res)
        qDebug() << "Ошибка при изменении записи в SetsSyncShip";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Удаление набора
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteSetter(int id)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from SetterOut where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в SetterOut";

    return res;

}


//------------------------------------------------------------------------------------------------------
// Удаление рекламации
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteClaim(int id)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from Claim where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Claim";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка списка рекламации
//------------------------------------------------------------------------------------------------------
int RepoMSSQL::LoadPart(int start, int count, const QString &number, QList<Claim> &listItems) const
{
    int res = 0;
    listItems.clear();
    QSqlQuery query(db);
    QStringList slWhere;

    QString sqlNumber = " Number like :Number";

    QStringList sql = {"select c.id,Number,DateClaim,TypeClaimId,idOrg,ObjectInstall,"
                       "o.orgName,ct.NameType,isClosed "
                       "from Claim c "
                       "join ClaimType ct on ct.id=c.TypeClaimId "
                       "left join Organization o on o.id=c.idOrg" };


    if(!number.isEmpty())
        slWhere.push_back(sqlNumber);

    if(slWhere.size() > 0)
    {
        sql.push_back(" where ");
        sql.push_back(slWhere.join(" and "));
    }

    sql.push_back(" order by DateClaim desc,Number offset :start rows fetch next :count rows only");
    QString sql2 = sql.join("");
    query.prepare(sql2);
    query.bindValue(":start", start);
    query.bindValue(":count", count);
    query.bindValue(":Number", QString("%%1%").arg(number));

    // qDebug() << sql2;

    query.exec();
    while(query.next())
    {
        Claim claim;
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateCreate = query.value(2).toDateTime();
        claim.idTypeClaim = query.value(3).toInt();
        claim.idOrg = query.value(4).toInt();
        claim.ObjectInstall = query.value(5).toString();
        claim.nameOrganization = query.value(6).toString();
        claim.TypeClaimString = query.value(7).toString();
        claim.isClosed = query.value(8).toBool();
        listItems.push_back(claim);
        ++res;
    }

    return res;
}



//------------------------------------------------------------------------------------------------------
// Загрузка списков продукции
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::LoadChildClaim(Claim &claim)
{
    bool res;
    claim.childItems.clear();
    QSqlQuery query(db);
    query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,"
                  "dateCreate,dateOn,dateOff,i.garantMonth,dateGarant,isZip,it.indexType,it.VNFT,it.typeName "
                  "from ClaimItems ci "
                  "join Items i on i.id=ci.idItem "
                  "join itemType it on it.id=i.idType "
                  "where ci.idClaim=:idClaim");

    query.bindValue(":idClaim", claim.id);

    res = query.exec();
    while(query.next())
    {
        Items prod;

        prod.id = query.value(0).toInt();
        prod.idParent = query.value(1).toInt();
        prod.idShip = query.value(2).toInt();
        prod.idSet = query.value(3).toInt();
        prod.idType = query.value(4).toInt();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.numberDoc = query.value(7).toString();
        prod.name = query.value(8).toString();
        prod.dateCreate = query.value(9).toDateTime();
        prod.dateOn = query.value(10).toDateTime();
        prod.dateOff = query.value(11).toDateTime();
        prod.garantMonth = query.value(12).toInt();
        prod.dateGarant = query.value(13).toDateTime();
        prod.isZip = query.value(14).toBool();
        prod.type.id = prod.id;
        prod.type.indexType = (IndexType)query.value(15).toInt();
        prod.type.VNFT = query.value(16).toString();
        prod.type.typeName = query.value(17).toString();
        LoadStatus(prod);
        claim.childItems.push_back(prod);
    }

    return res;
}


//------------------------------------------------------------------------------------------------------
// Поиск изделий по строке серийного номера
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(const QString &serialNumber, QList<Product> &listProduct, int status, bool isFree)
// {
//     listProduct.clear();
//     QSqlQuery query(db);

//     if(serialNumber.isEmpty())
//         return FindItems(listProduct, status, isFree);

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                       "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                       "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                       "from Product where g_number like :number order by g_name");
//         else
//         {
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product where g_number like :number and idShipment is null and idSetter is null order by g_name");
//         }
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                       "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                       "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                       "from Product p "
//                       "join "
//                       "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                       "from ProductStatus group by idProduct "
//                       "having max(idStatus)=:idStatus "
//                       ") ms on ms.idProduct=p.id where g_number like :number order by g_name"
//                       );
//         else
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product p "
//                           "join "
//                           "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ProductStatus group by idProduct "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idProduct=p.id where g_number like :number and idShipment is null and idSetter is null order by g_name"
//                           );

//         query.bindValue(":idStatus", status);
//     }
//     query.bindValue(":number", QString("%%1%").arg(serialNumber));

//     query.exec();
//     while(query.next())
//     {
//         Product prod;

//         prod.id = query.value(0).toInt();
//         prod.idShipment = query.value(1).toInt();
//         prod.idSetterOut = query.value(2).toInt();
//         prod.idType = query.value(3).toInt();
//         prod.name = query.value(4).toString();
//         prod.number = query.value(5).toString();
//         prod.number2 = query.value(6).toString();
//         prod.dateRegister = query.value(7).toDateTime();
//         prod.redaction1 = query.value(8).toString();
//         prod.redaction2 = query.value(9).toString();
//         prod.redactionPS = query.value(10).toString();
//         prod.questList = query.value(11).toString();
//         prod.isAvr = query.value(12).toBool();
//         prod.isAkb = query.value(13).toBool();
//         prod.isCooler = query.value(14).toBool();
//         prod.isSkm = query.value(15).toBool();
//         prod.numberBI = query.value(16).toString();
//         prod.numberUSIKP = query.value(17).toString();
//         prod.shunt = query.value(18).toString();
//         prod.isZip = query.value(19).toBool();
//         prod.garantMonth = query.value(20).toInt();
//         prod.EndGarant = query.value(21).toDateTime();
//         prod.dateOn = query.value(22).toDateTime();
//         LoadStatus(prod/*, this*/);
//         listProduct.push_back(prod);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Поиск изделий
// isFree - свободные изделия, не включенные в набор или отгрузку
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(QList<Product> &listProduct, int status, bool isFree)
// {
//     listProduct.clear();
//     QSqlQuery query(db);


//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                   "from Product order by g_name");
//         else
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product where idShipment is null and idSetter is null order by g_name");
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                   "from Product p "
//                   "join "
//                   "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                   "from ProductStatus group by idProduct "
//                   "having max(idStatus)=:idStatus "
//                   ") ms on ms.idProduct=p.id order by g_name"
//                   );
//         else
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product p "
//                           "join "
//                           "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ProductStatus group by idProduct "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idProduct=p.id where idShipment is null and idSetter is null order by g_name"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     query.exec();
//     while(query.next())
//     {
//         Product prod;

//         prod.id = query.value(0).toInt();
//         prod.idShipment = query.value(1).toInt();
//         prod.idSetterOut = query.value(2).toInt();
//         prod.idType = query.value(3).toInt();
//         prod.name = query.value(4).toString();
//         prod.number = query.value(5).toString();
//         prod.number2 = query.value(6).toString();
//         prod.dateRegister = query.value(7).toDateTime();
//         prod.redaction1 = query.value(8).toString();
//         prod.redaction2 = query.value(9).toString();
//         prod.redactionPS = query.value(10).toString();
//         prod.questList = query.value(11).toString();
//         prod.isAvr = query.value(12).toBool();
//         prod.isAkb = query.value(13).toBool();
//         prod.isCooler = query.value(14).toBool();
//         prod.isSkm = query.value(15).toBool();
//         prod.numberBI = query.value(16).toString();
//         prod.numberUSIKP = query.value(17).toString();
//         prod.shunt = query.value(18).toString();
//         prod.isZip = query.value(19).toBool();
//         prod.garantMonth = query.value(20).toInt();
//         prod.EndGarant = query.value(21).toDateTime();
//         prod.dateOn = query.value(22).toDateTime();
//         LoadStatus(prod/*, this*/);
//         listProduct.push_back(prod);
//     }
// }


//------------------------------------------------------------------------------------------------------
// Поиск модулей
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(QList<Modul> &listModul, int status, bool isFree)
// {
//     listModul.clear();
//     QSqlQuery query(db);

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                       "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                   "from Modules order by m_name");
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules where idShipment is null order by m_name");
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                     "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                     "from Modules m "
//                     "join "
//                     "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                     "from ModulStatus group by idModul "
//                     "having max(idStatus)=:idStatus "
//                     ") ms on ms.idModul=m.id order by m_name"
//                   );
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules m "
//                           "join "
//                           "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ModulStatus  group by idModul "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idModul=m.id  where idShipment is null order by m_name"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     query.exec();
//     while(query.next())
//     {
//         Modul mod;

//         mod.id = query.value(0).toInt();
//         mod.idShipment = query.value(1).toInt();
//         mod.idProduct = query.value(2).toInt();
//         mod.idType = query.value(3).toInt();
//         mod.name = query.value(4).toString();
//         mod.number = query.value(5).toString();
//         mod.number2 = query.value(6).toString();
//         mod.dateRegister = query.value(8).toDateTime();
//         mod.isZip = query.value(9).toBool();
//         mod.garantMonth = query.value(10).toInt();
//         mod.EndGarant = query.value(11).toDateTime();
//         mod.dateOn = query.value(12).toDateTime();
//         LoadStatus(mod/*, this*/);
//         listModul.push_back(mod);
//     }
// }

//------------------------------------------------------------------------------------------------------
// Поиск модулей по серийному номеру
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(const QString &serialNumber, QList<Modul> &listModul, int status, bool isFree)
// {
//     listModul.clear();
//     QSqlQuery query(db);

//     if(serialNumber.isEmpty())
//         return FindItems(listModul, status, isFree);

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                       "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                       "from Modules where m_number like :number order by m_name");
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules where m_number like :number and idShipment is null order by m_name");
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                       "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                       "from Modules m "
//                       "join "
//                       "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                       "from ModulStatus group by idModul "
//                       "having max(idStatus)=:idStatus "
//                       ") ms on ms.idModul=m.id where m_number like :number order by m_name"
//                       );
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules m "
//                           "join "
//                           "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ModulStatus group by idModul "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idModul=m.id where m_number like :number and idShipment is null order by m_name"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     query.bindValue(":number", QString("%%1%").arg(serialNumber));

//     query.exec();
//     while(query.next())
//     {
//         Modul mod;

//         mod.id = query.value(0).toInt();
//         mod.idShipment = query.value(1).toInt();
//         mod.idProduct = query.value(2).toInt();
//         mod.idType = query.value(3).toInt();
//         mod.name = query.value(4).toString();
//         mod.number = query.value(5).toString();
//         mod.number2 = query.value(6).toString();
//         mod.dateRegister = query.value(8).toDateTime();
//         mod.isZip = query.value(9).toBool();
//         mod.garantMonth = query.value(10).toInt();
//         mod.EndGarant = query.value(11).toDateTime();
//         mod.dateOn = query.value(12).toDateTime();
//         LoadStatus(mod);
//         listModul.push_back(mod);
//     }
// }



//------------------------------------------------------------------------------------------------------
// Загрузка списка статусов для изделия
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadStatus(Product &prod)
// {
//     prod.listStatus.clear();

//     QSqlQuery query(db);
//     query.prepare("select ms.id,idProduct,idStatus,DateStatus,Comment,sd.nameStatus,sd.typeStatus "
//                   "from ProductStatus ms "
//                   "join StatusDevice sd on sd.id=ms.idStatus "
//                   "where ms.idProduct=:id "
//                   "order by ms.DateStatus");

//     query.bindValue(":id", prod.id);

//     query.exec();
//     while(query.next())
//     {
//         Status stat;
//         stat.id = query.value(0).toInt();
//         stat.idDevice = query.value(1).toInt();
//         stat.idStatus = (Status::Stat)query.value(2).toInt();
//         stat.dateStatus = query.value(3).toDateTime();
//         stat.Comment = query.value(4).toString();
//         stat.nameStatus = query.value(5).toString();
//         stat.typeStatus = query.value(6).toInt();
//         prod.listStatus.push_back(stat);
//     }
// }

//------------------------------------------------------------------------------------------------------
// Загрузка списка статусов для модуля
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadStatus(Modul &mod)
// {
//     mod.listStatus.clear();

//     QSqlQuery query(db);
//     query.prepare("select ms.id,idModul,idStatus,DateStatus,Comment,sd.nameStatus,sd.typeStatus "
//                   "from ModulStatus ms "
//                   "join StatusDevice sd on sd.id=ms.idStatus "
//                   "where ms.idModul=:id "
//                   "order by ms.DateStatus");

//     query.bindValue(":id", mod.id);

//     query.exec();
//     while(query.next())
//     {
//         Status stat;
//         stat.id = query.value(0).toInt();
//         stat.idDevice = query.value(1).toInt();
//         stat.idStatus = (Status::Stat)query.value(2).toInt();
//         stat.dateStatus = query.value(3).toDateTime();
//         stat.Comment = query.value(4).toString();
//         stat.nameStatus = query.value(5).toString();
//         stat.typeStatus = query.value(6).toInt();
//         mod.listStatus.push_back(stat);
//     }
// }

// void RepoMSSQL::LoadStatus(Plate &plate)
// {
//     plate.listStatus.clear();

//     QSqlQuery query(db);
//     query.prepare("select ps.id,idPlate,idStatus,DateStatus,Comment,sd.nameStatus,sd.typeStatus "
//                   "from PlateStatus ps "
//                   "join StatusDevice sd on sd.id=ps.idStatus "
//                   "where ps.idPlate=:id "
//                   "order by ps.DateStatus");

//     query.bindValue(":id", plate.id);

//     query.exec();
//     while(query.next())
//     {
//         Status stat;
//         stat.id = query.value(0).toInt();
//         stat.idDevice = query.value(1).toInt();
//         stat.idStatus = (Status::Stat)query.value(2).toInt();
//         stat.dateStatus = query.value(3).toDateTime();
//         stat.Comment = query.value(4).toString();
//         stat.nameStatus = query.value(5).toString();
//         stat.typeStatus = query.value(6).toInt();
//         plate.listStatus.push_back(stat);
//     }

// }

int RepoMSSQL::GetTypeStatus(int idStatus)
{
    int typeStat = 0;
    QSqlQuery query(db);
    query.prepare("select typeStatus from StatusDevice where id=:id");
    query.bindValue(":id", idStatus);

    query.exec();
    if(query.next())
        typeStat = query.value(0).toInt();

    return typeStat;
}

const QString RepoMSSQL::GetNameStatus(int idStatus)
{
    QString nameStatus = 0;
    QSqlQuery query(db);
    query.prepare("select NameStatus from StatusDevice where id=:id");
    query.bindValue(":id", idStatus);

    query.exec();
    if(query.next())
        nameStatus = query.value(0).toString();

    return nameStatus;
}


//------------------------------------------------------------------------------------------------------
// Поиск списка плат по номеру
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(const QString &number, QList<Plate> &listPlate, int /*status*/, bool isFree)
// {
//     QSqlQuery query(db);
//     listPlate.clear();

//     // if(number.isEmpty())
//     //     return FindItems(listPlate, status);

//     if(isFree)
//         query.prepare("select id,CreateDate,Number,NumberFW,NumberDoc,idModul,idPlateType "
//                   "from Plate where Number like :Number and idModul is null");
//     else
//         query.prepare("select id,CreateDate,Number,NumberFW,NumberDoc,idModul,idPlateType "
//                       "from Plate where Number like :Number");


//     query.bindValue(":Number", QString("%%1%").arg(number));

//     query.exec();
//     while(query.next())
//     {
//         Plate plate;
//         plate.id = query.value(0).toInt();
//         plate.dateRegister = query.value(1).toDateTime();
//         plate.number = query.value(2).toString();
//         plate.number2 = query.value(3).toString();
//         plate.numberDoc = query.value(4).toString();
//         // plate.VNFT = query.value(5).toString();
//         plate.idParent = query.value(5).toInt();
//         plate.idType = query.value(6).toInt();
//         LoadStatus(plate);
//         listPlate.push_back(plate);
//     }
// }

// void RepoMSSQL::FindItems(const QString &/*number*/, QList<Shipment> &listShip, int status, bool isFree)
// {
//     return FindItems(listShip, status, isFree);
// }

//------------------------------------------------------------------------------------------------------
// Привязка платы к модулю
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::LinkPlate(int idPlate, int idModul)
// {
//     bool res;
//     QSqlQuery query(db);

//     query.prepare("update Plate set idModul=:idModul where id=:id");

//     query.bindValue(":idModul", idModul);
//     query.bindValue(":id", idPlate);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в RemontM";

//     return res;
// }


//------------------------------------------------------------------------------------------------------
// Поиск набора по id
//------------------------------------------------------------------------------------------------------
SetterOut RepoMSSQL::GetSetter(int id)
{
    QSqlQuery query(db);
    SetterOut setter;

    query.prepare("select idShipment,s_name,s_orderNum "
                  "from SetterOut where id = :id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        setter.id = id;
        setter.idShip = query.value(0).toInt();
        setter.name = query.value(1).toString();
        setter.number = query.value(2).toString();
    }

    return setter;
}


//------------------------------------------------------------------------------------------------------
// Добавление новой отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddItem(Shipment &ship)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into Shipment (c_number,c_objectInstall,c_dateOut,c_questList,c_schet,"
                  "c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,idOrganization) "
                  "output inserted.id values(:c_number,:c_objectInstall,:c_dateOut,:c_questList,:c_schet,"
                  ":c_cardOrder,:c_numberUPD,:c_buyer,:c_dateUPD,:idOrganization)");

    query.bindValue(":c_number", ship.number);
    query.bindValue(":c_objectInstall", ship.objectInstall);
    query.bindValue(":c_dateOut", ship.dateRegister);
    // query.bindValue(":c_customer", ship.customer);
    query.bindValue(":c_questList", ship.questList);
    query.bindValue(":c_schet", ship.schet);
    query.bindValue(":c_cardOrder", ship.cardOrder);
    query.bindValue(":c_numberUPD", ship.numberUPD);
    query.bindValue(":c_buyer", ship.buyer);
    query.bindValue(":c_dateUPD", ship.dateUPD);
    query.bindValue(":idOrganization", ship.idOrganization);
    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в Shipment";
    else
    {
        if(query.next())
            ship.id = query.value(0).toInt();
    }

    return res;

}

bool RepoMSSQL::AddItem(SetterOut &setter)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into SetterOut (idShipment,s_name,s_OrderNum,dateCreate) "
                  "output inserted.id values(:idShipment,:s_name,:s_OrderNum,:dateCreate)");

    if(setter.idShip != 0)
        query.bindValue(":idShipment", setter.idShip);
    query.bindValue(":s_name", setter.name);
    query.bindValue(":s_OrderNum", setter.numberDoc);
    query.bindValue(":dateCreate", setter.dateCreate);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в SetterOut";
    else
    {
        if(query.next())
            setter.id = query.value(0).toInt();
    }

    return res;
}

bool RepoMSSQL::AddItem(Claim &claim)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into Claim (Number,dateClaim,typeClaimId,idOrg,ObjectInstall) "
                  "output inserted.id values(:Number,:dateClaim,:TypeClaimId,:idOrg,:ObjectInstall)");

    query.bindValue(":Number", claim.number);
    query.bindValue(":dateClaim", claim.dateCreate);
    query.bindValue(":TypeClaimId", claim.idTypeClaim);
    query.bindValue(":ObjectInstall", claim.ObjectInstall);
    query.bindValue(":idOrg", claim.idOrg);

    res = query.exec();

    if(query.next())
    {
        claim.id = query.value(0).toInt();
    }

    if(!res)
        qDebug() << "Ошибка при добавлении записи в Claim";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Обновление отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::UpdateItem(Shipment &ship)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("update Shipment set c_number=:c_number,c_objectInstall=:c_objectInstall,c_dateOut=:c_dateOut,"
                  "c_questList=:c_questList,c_schet=:c_schet,c_cardOrder=:c_cardOrder,"
                  "c_numberUPD=:c_numberUPD,c_buyer=:c_buyer,c_dateUPD=:c_dateUPD,idOrganization=:idOrganization where id=:id");


    // qDebug() << ship.dateRegister.date().year();

    query.bindValue(":c_number", ship.number);
    query.bindValue(":c_objectInstall", ship.objectInstall);
    // if(!ship.dateRegister.toString("dd.MM.yyyy").isEmpty())
    if(ship.dateRegister.date().year() > 1900)
        query.bindValue(":c_dateOut", ship.dateRegister);
    else
        query.bindValue(":c_dateOut", QVariant());

    query.bindValue(":c_questList", ship.questList);
    query.bindValue(":c_schet", ship.schet);
    query.bindValue(":c_cardOrder", ship.cardOrder);
    query.bindValue(":c_numberUPD", ship.numberUPD);
    query.bindValue(":c_buyer", ship.buyer);
    // if(!ship.dateUPD.toString("dd.MM.yyyy").isEmpty())
    if(ship.dateUPD.date().year() > 1900)
        query.bindValue(":c_dateUPD", ship.dateUPD);
    else
        query.bindValue(":c_dateUPD", QVariant());

    if(ship.idOrganization > 0)
        query.bindValue(":idOrganization", ship.idOrganization);
    query.bindValue(":id", ship.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в Shipment";

    return res;
}


//------------------------------------------------------------------------------------------------------
// Обновление набора
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::UpdateItem(SetterOut &setter)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("update SetterOut set idShipment=:idShipment,s_name=:s_name,s_orderNum=:s_orderNum,"
                  "dateCreate=:dateCreate where id=:id");

    if(setter.idShip > 0)
        query.bindValue(":idShipment", setter.idShip);
    query.bindValue(":s_name", setter.name);
    query.bindValue(":s_orderNum", setter.numberDoc);
    query.bindValue(":dateCreate", setter.dateCreate);
    query.bindValue(":id", setter.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в SetterOut";

    return res;
}



//------------------------------------------------------------------------------------------------------
// Обновление рекламации
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::UpdateItem(Claim &claim)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("update Claim set Number=:Number,DateClaim=:DateClaim,TypeClaimId=:TypeClaimId,"
                  "ObjectInstall=:ObjectInstall,idOrg=:idOrg,isClosed=:isClosed "
                  "where id=:id");

    query.bindValue(":id", claim.id);
    query.bindValue(":Number", claim.number);
    query.bindValue(":DateClaim", claim.dateCreate);
    query.bindValue(":TypeClaimId", claim.idTypeClaim);
    query.bindValue(":isClosed", claim.isClosed);
    if(claim.idOrg > 0)
        query.bindValue(":idOrg", claim.idOrg);
    query.bindValue(":ObjectInstall", claim.ObjectInstall);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в Claim";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Поиск отгрузки по id
//------------------------------------------------------------------------------------------------------
Shipment RepoMSSQL::GetShipment(int id)
{
    QSqlQuery query(db);
    Shipment ship;

    query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,c_customer "
                  "from Shipment where id = :id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        ship.id = id;
        ship.number = query.value(0).toString();
        ship.objectInstall = query.value(1).toString();
        ship.dateRegister = query.value(2).toDateTime();
        ship.idOrganization = query.value(3).toInt();
        ship.questList = query.value(4).toString();
        ship.schet = query.value(5).toString();
        ship.cardOrder = query.value(6).toString();
        ship.numberUPD = query.value(7).toString();
        ship.buyer = query.value(8).toString();
        ship.dateUPD = query.value(9).toDateTime();
        ship.customer = query.value(10).toString();
    }
    return ship;

}

// Modul RepoMSSQL::GetModul(int /*id*/)
// {
//     Modul mod;
//     return mod;
// }

//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadShipment(QList<Shipment> &listShip, bool /*isFinish*/)
// {
//     listShip.clear();
//     QSqlQuery query(db);

//     query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
//                   "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id "
//                   "from Shipment where c_dateOut is null");

//     query.exec();
//     while(query.next())
//     {
//         Shipment ship;
//         ship.number = query.value(0).toString();
//         ship.objectInstall = query.value(1).toString();
//         ship.dateRegister = query.value(2).toDateTime();
//         ship.idOrganization = query.value(3).toInt();
//         ship.questList = query.value(4).toString();
//         ship.schet = query.value(5).toString();
//         ship.cardOrder = query.value(6).toString();
//         ship.numberUPD = query.value(7).toString();
//         ship.buyer = query.value(8).toString();
//         ship.dateUPD = query.value(9).toDateTime();
//         ship.id = query.value(10).toInt();
//         // ship.customer = query.value(11).toString();
//         listShip.push_back(ship);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Загрузка изделий для набора
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadChildSetter(SetterOut &setter)
{
    setter.childItems.clear();
    QSqlQuery query(db);
    query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,"
                  "dateCreate,dateOn,dateOff,i.garantMonth,dateGarant,isZip,it.indexType,it.VNFT,it.typeName "
                  "from Items i join itemType it on it.id=i.idType "
                  "where idSet = :idSet");

    query.bindValue(":idSet", setter.id);

    query.exec();
    while(query.next())
    {
        Items prod;

        prod.id = query.value(0).toInt();
        prod.idParent = query.value(1).toInt();
        prod.idShip = query.value(2).toInt();
        prod.idSet = query.value(3).toInt();
        prod.idType = query.value(4).toInt();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.numberDoc = query.value(7).toString();
        prod.name = query.value(8).toString();
        prod.dateCreate = query.value(9).toDateTime();
        prod.dateOn = query.value(10).toDateTime();
        prod.dateOff = query.value(11).toDateTime();
        prod.garantMonth = query.value(12).toInt();
        prod.dateGarant = query.value(13).toDateTime();
        prod.isZip = query.value(14).toBool();
        prod.type.id = prod.id;
        prod.type.indexType = (IndexType)query.value(15).toInt();
        prod.type.VNFT = query.value(16).toString();
        prod.type.typeName = query.value(17).toString();
        LoadStatus(prod);
        setter.childItems.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Добавление, удаление состава набора
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::ItemsSyncSet(int idSet, TrackRecord<Items> *track)
{
    bool res = false;

    QSqlDatabase::database().transaction();

    QSqlQuery query(db);
    query.prepare("update Items set idSet=null where id=:id");
    for(auto &it: track->listDel)
    {
        query.bindValue(":id", it.id);
        res = query.exec();
    }

    query.clear();

    query.prepare("update Items set idSet=:idSet where id=:id");
    for(auto &it: track->listAdd)
    {
        query.bindValue(":id", it.id);
        query.bindValue(":idSet", idSet);
        res = query.exec();
    }

    res = QSqlDatabase::database().commit();

    if(!res)
        qDebug() << "Ошибка при изменении записи в ItemsSyncSet";

    return res;
}


//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(QList<Shipment> &listShip, int /*isFinish*/, bool isFree)
// {
//     listShip.clear();
//     QSqlQuery query(db);

//     if(isFree)
//         query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
//                   "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,c_customer "
//                   "from Shipment where c_dateOut is null");
//     else
//         query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
//                       "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,c_customer "
//                       "from Shipment where c_dateOut is not null");

//     query.exec();
//     while(query.next())
//     {
//         Shipment ship;
//         ship.number = query.value(0).toString();
//         ship.objectInstall = query.value(1).toString();
//         ship.dateRegister = query.value(2).toDateTime();
//         ship.idOrganization = query.value(3).toInt();
//         ship.questList = query.value(4).toString();
//         ship.schet = query.value(5).toString();
//         ship.cardOrder = query.value(6).toString();
//         ship.numberUPD = query.value(7).toString();
//         ship.buyer = query.value(8).toString();
//         ship.dateUPD = query.value(9).toDateTime();
//         ship.id = query.value(10).toInt();
//         ship.customer = query.value(11).toString();
//         listShip.push_back(ship);
//     }
// }

// void RepoMSSQL::FindItems(IndexType iType, QList<Items> &listItems, int status, bool isFree)
// {
//     listItems.clear();
//     QSqlQuery query(db);

//     if(status == StatusItem::NONE)
//     {
//         if(!isFree)
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem");
//         else
//         {
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i "
//                           "join ItemType it on it.id=i.idType and it.indexType=:indexType "
//                           "where idShip is null and idSet is null "
//                           "order by nameItem");
//         }

//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id "
//                           "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
//                           );
//         else
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip"
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id where idShip is null and idSet is null "
//                           "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     // QElapsedTimer t;
//     // t.start();
//     // qDebug() << t.elapsed() << "Start";


//     query.bindValue(":indexType", iType);
//     query.exec();
//     while(query.next())
//     {
//         Items item;

//         item.id = query.value(0).toInt();
//         item.idParent = query.value(1).toInt();
//         item.idShip = query.value(2).toInt();
//         item.idSet = query.value(3).toInt();
//         item.idType = query.value(4).toInt();
//         item.number = query.value(5).toString();
//         item.number2 = query.value(6).toString();
//         item.numberDoc = query.value(7).toString();
//         item.name = query.value(8).toString();
//         item.dateCreate = query.value(9).toDateTime();
//         item.dateOn = query.value(10).toDateTime();
//         item.dateOff = query.value(11).toDateTime();
//         item.garantMonth = query.value(12).toInt();
//         item.dateGarant = query.value(13).toDateTime();
//         item.isZip = query.value(14).toBool();
//         // item.LoadStatus(item);
//         listItems.push_back(item);
//     }

//     // qDebug() << t.elapsed() << "End";

// }

bool RepoMSSQL::LoadChildItems(int idParent, QList<Items> &listItems) const
{
    bool res;
    listItems.clear();
    QSqlQuery query(db);
    query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                    "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,sd.NameStatus,it.indexType,it.VNFT,it.garantMonth,"
                    "ist.dateStatus,ist.idStatus,ist.comment "
                    "from Items i join ItemType it on it.id=i.idType "
                    "join (select idItem, max(DateStatus) dateStatus from ItemStatus group by idItem "
                    ") ms on ms.idItem=i.id "
                    "join ItemStatus ist on ist.idItem=ms.idItem and ist.dateStatus=ms.dateStatus "
                    "join StatusDevice sd on sd.id=ist.idStatus "
                    "where i.idParent=:idParent order by i.nameItem");

    query.bindValue(":idParent", idParent);

    res = query.exec();
    while(query.next())
    {
        Items item;

        item.id = query.value(0).toInt();
        item.idParent = query.value(1).toInt();
        item.idShip = query.value(2).toInt();
        item.idSet = query.value(3).toInt();
        item.idType = query.value(4).toInt();
        item.number = query.value(5).toString();
        item.number2 = query.value(6).toString();
        item.numberDoc = query.value(7).toString();
        item.name = query.value(8).toString();
        item.dateCreate = query.value(9).toDateTime();
        item.dateOn = query.value(10).toDateTime();
        item.dateOff = query.value(11).toDateTime();
        item.garantMonth = query.value(12).toInt();
        item.dateGarant = query.value(13).toDateTime();
        item.isZip = query.value(14).toBool();
        item.VNFT = query.value(15).toString();
        item.currStatus = query.value(16).toString();

        item.type.indexType = (IndexType)query.value(17).toInt();
        item.type.typeName = query.value(15).toString();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        Status status;
        status.idItem = item.id;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (StatusItem)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        LoadChildItems(item.id, item.childItems);
        listItems.push_back(item);
    }

    return res;
}


int RepoMSSQL::LoadPart(int start, int count, IndexType iType,
                           const QString &number, QList<Items> &listItems,
                           const QVector<StatusItem>& listStatus, bool isBusy, bool isParent) const
{
    int res = 0;
    QStringList slStatus;
    QSqlQuery query(db);
    QStringList slWhere;


    QString sqlNumber = " number like :number";
    QString sqlBusy = " idShip is null and idSet is null";
    QString sqlParent = " idParent is null";
    QString sqlStatus = "ist.idStatus=:idStatus%1 ";

    QStringList sql = {"select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                       "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,sd.NameStatus,it.indexType,it.VNFT,it.garantMonth,"
                       "ist.dateStatus,ist.idStatus,ist.comment "
                       "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
                       "join (select idItem, max(DateStatus) dateStatus from ItemStatus group by idItem "
                       ") ms on ms.idItem=i.id "
                       "join ItemStatus ist on ist.idItem=ms.idItem and ist.dateStatus=ms.dateStatus "
                       "join StatusDevice sd on sd.id=ist.idStatus"};


    if(!number.isEmpty())
        slWhere.push_back(sqlNumber);

    if(!isBusy)
        slWhere.push_back(sqlBusy);

    if(!isParent)
        slWhere.push_back(sqlParent);

    if(slWhere.size() > 0)
    {
        sql.push_back(" where ");
        sql.push_back(slWhere.join(" and "));
    }

    if(listStatus.size() > 0)
    {
        for(int i = 0; i < listStatus.size(); ++i)
            slStatus.push_back(sqlStatus.arg(i));

        sql.push_back(" and (" + slStatus.join(" or ") + ")");
    }

    sql.push_back(" order by number");
    if(count > 0)
        sql.push_back(" offset :start rows fetch next :count rows only");

    QString sql2 = sql.join("");
    query.prepare(sql2);
    query.bindValue(":indexType", iType);
    query.bindValue(":start", start);
    query.bindValue(":count", count);
    query.bindValue(":number", QString("%%1%").arg(number));

    for(int i = 0; i < listStatus.size(); ++i)
    {
        query.bindValue(QString(":idStatus%1").arg(i), listStatus[i]);
    }

    // qDebug() << sql2;

    query.exec();
    while(query.next())
    {
        Items item;

        item.id = query.value(0).toInt();
        item.idParent = query.value(1).toInt();
        item.idShip = query.value(2).toInt();
        item.idSet = query.value(3).toInt();
        item.idType = query.value(4).toInt();
        item.number = query.value(5).toString();
        item.number2 = query.value(6).toString();
        item.numberDoc = query.value(7).toString();
        item.name = query.value(8).toString();
        item.dateCreate = query.value(9).toDateTime();
        item.dateOn = query.value(10).toDateTime();
        item.dateOff = query.value(11).toDateTime();
        item.garantMonth = query.value(12).toInt();
        item.dateGarant = query.value(13).toDateTime();
        item.isZip = query.value(14).toBool();
        item.type.typeName = query.value(15).toString();
        item.currStatus = query.value(16).toString();
        item.type.indexType = (IndexType)query.value(17).toInt();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.dateGarant = query.value(20).toDateTime();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;

        Status status;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (StatusItem)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.idItem = item.id;
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        listItems.push_back(item);
        ++res;
    }

    return res;
}

int RepoMSSQL::LoadPartAll(int start, int count, const QString &number, QList<Items> &listItems, QVector<StatusItem> &listStatus, bool isBusy, bool isParent) const
{
    int res = 0;
    QStringList slStatus;
    QSqlQuery query(db);

    QStringList slWhere;
    QString sqlNumber = " number like :number";
    QString sqlBusy = " idShip is null and idSet is null";
    QString sqlParent = " idParent is null";
    QString sqlStatus = "ist.idStatus=:idStatus%1 ";

    QStringList sql = {"select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                       "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,sd.NameStatus,it.indexType,it.VNFT,it.garantMonth,"
                       "ist.dateStatus,ist.idStatus,ist.comment "
                       "from Items i join ItemType it on it.id=i.idType "
                       "join (select idItem, max(DateStatus) dateStatus from ItemStatus group by idItem "
                       ") ms on ms.idItem=i.id "
                       "join ItemStatus ist on ist.idItem=ms.idItem and ist.dateStatus=ms.dateStatus "
                       "join StatusDevice sd on sd.id=ist.idStatus"};


    if(!number.isEmpty())
        slWhere.push_back(sqlNumber);

    if(!isBusy)
        slWhere.push_back(sqlBusy);

    if(!isParent)
        slWhere.push_back(sqlParent);

    if(slWhere.size() > 0)
    {
        sql.push_back(" where ");
        sql.push_back(slWhere.join(" and "));
    }

    if(listStatus.size() > 0)
    {
        for(int i = 0; i < listStatus.size(); ++i)
            slStatus.push_back(sqlStatus.arg(i));

        sql.push_back(" and (" + slStatus.join(" or ") + ")");
    }

    sql.push_back(" order by number");
    if(count > 0)
        sql.push_back(" offset :start rows fetch next :count rows only");
    QString sql2 = sql.join("");
    query.prepare(sql2);
    query.bindValue(":start", start);
    query.bindValue(":count", count);
    query.bindValue(":number", QString("%%1%").arg(number));

    for(int i = 0; i < listStatus.size(); ++i)
    {
        query.bindValue(QString(":idStatus%1").arg(i), listStatus[i]);
    }

    query.exec();
    while(query.next())
    {
        Items item;

        item.id = query.value(0).toInt();
        item.idParent = query.value(1).toInt();
        item.idShip = query.value(2).toInt();
        item.idSet = query.value(3).toInt();
        item.idType = query.value(4).toInt();
        item.number = query.value(5).toString();
        item.number2 = query.value(6).toString();
        item.numberDoc = query.value(7).toString();
        item.name = query.value(8).toString();
        item.dateCreate = query.value(9).toDateTime();
        item.dateOn = query.value(10).toDateTime();
        item.dateOff = query.value(11).toDateTime();
        item.garantMonth = query.value(12).toInt();
        item.dateGarant = query.value(13).toDateTime();
        item.isZip = query.value(14).toBool();
        item.type.typeName = query.value(15).toString();
        item.currStatus = query.value(16).toString();
        item.type.indexType = (IndexType)query.value(17).toInt();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.VNFT = item.type.VNFT;
        item.type.id = item.idType;

        Status status;
        status.idItem = item.id;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (StatusItem)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        listItems.push_back(item);
        ++res;
    }

    return res;

}


bool RepoMSSQL::AddItem(Items &item) const
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into Items (idType,number,number2,numberDoc,nameItem,dateCreate,garantMonth,isZip) "
                  "output inserted.id values(:idType,:number,:number2,:numberDoc,:nameItem,:dateCreate,:garantMonth,:isZip)");

    query.bindValue(":idType", item.idType);
    query.bindValue(":number", item.number);
    query.bindValue(":number2", item.number2);
    query.bindValue(":numberDoc", item.numberDoc);
    query.bindValue(":nameItem", item.name);
    query.bindValue(":dateCreate", item.dateCreate);
    query.bindValue(":garantMonth", item.garantMonth);
    query.bindValue(":isZip", item.isZip);

    res = query.exec();
    if(!res)
    {
        qDebug() << "Ошибка при добавлении записи в Items";
        qDebug() << db.lastError().text();
    }
    else
    {
        if(query.next())
            item.id = query.value(0).toInt();
    }

    return res;

}



// void RepoMSSQL::FindItems(IndexType iType, const QString &number, QList<Items> &listItems, int status, bool isFree)
// {
//     listItems.clear();
//     QSqlQuery query(db);

//     if(number.isEmpty())
//         return FindItems(iType, listItems, status, isFree);

//     if(status == StatusItem::NONE)
//     {
//         if(!isFree)
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
//                           "where number like :number order by nameItem");
//         else
//         {
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
//                           "where number like :number and idShip is null and idSet is null order by nameItem");
//         }
//     }
//     else
//     {
//         // запрос с учетом статуса
//         if(!isFree)
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip "
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id where number like :number "
//                           "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
//                           );
//         else
//             query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,i.garantMonth,dateGarant,isZip"
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id join ItemType it on it.id=i.idType and it.indexType=:indexType "
//                           "where number like :number and idShip is null and idSet is null order by nameItem"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     query.bindValue(":indexType", iType);
//     query.bindValue(":number", QString("%%1%").arg(number));

//     query.exec();
//     while(query.next())
//     {
//         Items item;

//         item.id = query.value(0).toInt();
//         item.idParent = query.value(1).toInt();
//         item.idShip = query.value(2).toInt();
//         item.idSet = query.value(3).toInt();
//         item.idType = query.value(4).toInt();
//         item.number = query.value(5).toString();
//         item.number2 = query.value(6).toString();
//         item.numberDoc = query.value(7).toString();
//         item.name = query.value(8).toString();
//         item.dateCreate = query.value(9).toDateTime();
//         item.dateOn = query.value(10).toDateTime();
//         item.dateOff = query.value(11).toDateTime();
//         item.garantMonth = query.value(12).toInt();
//         item.dateGarant = query.value(13).toDateTime();
//         item.isZip = query.value(14).toBool();
//         // item.LoadStatus(item);
//         // LoadStatus(prod);
//         listItems.push_back(item);
//     }

// }

void RepoMSSQL::LoadStatus(Items& item) const
{
    item.listStatus.clear();

    QSqlQuery query(db);
    query.prepare("select ist.id,idItem,idStatus,DateStatus,Comment,sd.nameStatus,sd.typeStatus "
                  "from ItemStatus ist "
                  "join StatusDevice sd on sd.id=ist.idStatus "
                  "where ist.idItem=:id "
                  "order by ist.DateStatus");

    query.bindValue(":id", item.id);

    query.exec();
    while(query.next())
    {
        Status stat;
        stat.id = query.value(0).toInt();
        stat.idItem = query.value(1).toInt();
        stat.idStatus = (StatusItem)query.value(2).toInt();
        stat.dateStatus = query.value(3).toDateTime();
        stat.Comment = query.value(4).toString();
        stat.nameStatus = query.value(5).toString();
        stat.typeStatus = query.value(6).toInt();
        item.listStatus.push_back(stat);
        item.SetLastStatus(item.currStatus, item.commentStatus);
    }

}

bool RepoMSSQL::AddStatus(Items &item, Status &status) const
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into ItemStatus (idItem,idStatus,DateStatus,Comment) "
                  "output inserted.id values(:idItem,:idStatus,:DateStatus,:Comment)");

    query.bindValue(":idItem", item.id);
    query.bindValue(":idStatus", status.idStatus);
    query.bindValue(":DateStatus", status.dateStatus);
    query.bindValue(":Comment", status.Comment);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в AddStatus";
    else
    {
        if(query.next())
            status.id = query.value(0).toInt();
    }

    return res;
}

bool RepoMSSQL::DelLastStatus(Items &item) const
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from ItemStatus where id = (select Top(1) id FROM ItemStatus where idItem=:idItem "
                  "order by DateStatus desc)");

    query.bindValue(":idItem", item.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при удалении записи в DelLastStatus";

    return res;
}

void RepoMSSQL::LoadTypeItem(IndexType indexType, QVector<ItemType> &listType) const
{
    listType.clear();
    QSqlQuery query(db);
    query.prepare("select id,typeName,garantMonth,VNFT from ItemType where indexType=:indexType");
    query.bindValue(":indexType", indexType);

    query.exec();
    while(query.next())
    {
        ItemType mType;
        mType.id = query.value(0).toInt();
        mType.typeName = query.value(1).toString();
        mType.garantMonth = query.value(2).toInt();
        mType.VNFT = query.value(3).toString();
        mType.indexType = indexType;
        listType.push_back(mType);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка наборов
//------------------------------------------------------------------------------------------------------
int RepoMSSQL::LoadPart(int start, int count, const QString &number, QList<SetterOut> &listItems, bool isFree) const
{
    int res = 0;
    listItems.clear();
    QSqlQuery query(db);
    QStringList slWhere;

    QString sqlNumber = " s_orderNum like :s_orderNum";
    QString sqlBusy = " idShipment is null";

    QStringList sql = {"select id,idShipment,s_name,s_orderNum,dateCreate from SetterOut " };

    if(!number.isEmpty())
        slWhere.push_back(sqlNumber);

    if(isFree)
        slWhere.push_back(sqlBusy);

    if(slWhere.size() > 0)
    {
        sql.push_back(" where ");
        sql.push_back(slWhere.join(" and "));
    }

    sql.push_back(" order by dateCreate desc,s_orderNum  offset :start rows fetch next :count rows only");
    QString sql2 = sql.join("");
    query.prepare(sql2);
    query.bindValue(":start", start);
    query.bindValue(":count", count);
    query.bindValue(":s_orderNum", QString("%%1%").arg(number));

    // qDebug() << sql2;

    query.exec();
    while(query.next())
    {
        SetterOut item;

        item.id = query.value(0).toInt();
        item.idShip = query.value(1).toInt();
        item.name = query.value(2).toString();
        item.numberDoc = query.value(3).toString();
        item.dateCreate = query.value(4).toDateTime();
        listItems.push_back(item);
        ++res;
    }

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка организаций
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadOrganization(QMap<int, QString> &listOrg)
{
    QSqlQuery query(db);
    listOrg.clear();

    query.prepare("select id,OrgName,INN,KPP from Organization where INN is not null or KPP is not null");
    query.exec();
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString INN = query.value(2).toString();
        QString KPP = query.value(3).toString();
        if(!INN.isEmpty())
        {
            name += " (ИНН " + INN;
            name += " КПП " + KPP + ")";
        }
        listOrg.insert(id, name);
    }
}

void RepoMSSQL::LoadOrganizationAsync(QMap<int, QString> &listOrg, QPromise<void> &promise)
{
    QSqlQuery query(db);
    listOrg.clear();

    query.prepare("select id,OrgName,INN,KPP from Organization where INN is not null or KPP is not null");
    query.exec();
    while(query.next() && !promise.isCanceled())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString INN = query.value(2).toString();
        QString KPP = query.value(3).toString();
        if(!INN.isEmpty())
        {
            name += " (ИНН " + INN;
            name += " КПП " + KPP + ")";
        }
        listOrg.insert(id, name);
    }
}

void RepoMSSQL::LoadOrganization(QList<Organization> &listOrg)
{
    QSqlQuery query(db);
    listOrg.clear();

    query.prepare("select id,OrgName,INN,KPP from Organization where INN is not null or KPP is not null order by OrgName");
    query.exec();
    while(query.next())
    {
        Organization org;
        org.id = query.value(0).toInt();
        org.orgName = query.value(1).toString();
        org.INN = query.value(2).toString();
        org.KPP = query.value(3).toString();
        listOrg.push_back(org);
    }
}

void RepoMSSQL::LoadOrganizationAsync(QList<Organization> &listOrg, QPromise<void> &promise)
{
    QSqlQuery query(db);
    listOrg.clear();

    query.prepare("select id,OrgName,INN,KPP from Organization where INN is not null or KPP is not null order by OrgName");
    query.exec();
    while(query.next() && !promise.isCanceled())
    {
        Organization org;
        org.id = query.value(0).toInt();
        org.orgName = query.value(1).toString();
        org.INN = query.value(2).toString();
        org.KPP = query.value(3).toString();
        listOrg.push_back(org);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка наборов в отгрузке
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadShipSetter(QList<SetterOut> &listSetter, int idShip)
{
    listSetter.clear();
    QSqlQuery query(db);

    query.prepare("select id,s_name,s_orderNum "
                  "from SetterOut where idShipment=:idShipment");

    query.bindValue(":idShipment", idShip);

    query.exec();
    while(query.next())
    {
        SetterOut setter;
        // setter.id = query.value(0).toInt();
        // setter.idShipment = idShip;
        // setter.name = query.value(1).toString();
        // setter.orderNumber = query.value(2).toString();
        listSetter.push_back(setter);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка организации
//------------------------------------------------------------------------------------------------------
Organization RepoMSSQL::GetOrganization(int id)
{
    Organization org;
    QSqlQuery query(db);

    query.prepare("select id,OrgName,INN,KPP from Organization where id=:id");
    query.bindValue(":id", id);
    query.exec();
    if(query.next())
    {
        org.id = query.value(0).toInt();
        org.orgName = query.value(1).toString();
        org.INN = query.value(2).toString();
        org.KPP = query.value(3).toString();
    }
    return org;
}



bool RepoMSSQL::LoadClaim(const QString number, QList<Claim> &listClaim)
{
    bool res;
    listClaim.clear();
    QSqlQuery query(db);
    QString sql = "select c.id,c.Number,dateClaim,typeClaimId,c.idOrg,ObjectInstall,o.orgName,isClosed "
                  "from Claim c "
                  "join ClaimType ct on ct.id=c.TypeClaimId "
                  "left join Organization o on o.id=c.idOrg";

    if(!number.isEmpty())
        sql += " where Number like '%:number%'";

    query.prepare(sql);
    query.bindValue(":number", number);

    res = query.exec();
    while(query.next())
    {
        Claim claim;
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateCreate = query.value(2).toDateTime();
        claim.idTypeClaim = query.value(3).toInt();
        claim.idOrg = query.value(4).toInt();
        claim.ObjectInstall = query.value(5).toString();
        claim.nameOrganization = query.value(6).toString();
        claim.isClosed = query.value(7).toBool();
        listClaim.push_back(claim);
    }

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка типов рекламаций
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadClaimType(QMap<int, QString> &listTypeClaim)
{
    listTypeClaim.clear();
    QSqlQuery query(db);
    query.prepare("select id,NameType from ClaimType");

    query.exec();
    while(query.next())
    {
        listTypeClaim.insert(query.value(0).toInt(), query.value(1).toString());
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка модулей из рекламации
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadClaimModules(int idClaim, QList<Modul> &listModul)
// {
//     listModul.clear();
//     QSqlQuery query(db);

//     query.prepare("select m.id,m.idShipment,m.idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,"
//                   "m_dateCreate,m_zip,m_garantMonth,m_endGarant "
//                   "from ClaimModule cm "
//                   "join Modules m on m.id=cm.idModul where cm.idClaim=:idClaim"
//                   );

//     query.bindValue(":idClaim", idClaim);

//     query.exec();
//     while(query.next())
//     {
//         Modul mod;

//         mod.id = query.value(0).toInt();
//         mod.idShipment = query.value(1).toInt();
//         mod.idProduct = query.value(2).toInt();
//         mod.idType = query.value(3).toInt();
//         mod.name = query.value(4).toString();
//         mod.number = query.value(5).toString();
//         mod.number2 = query.value(6).toString();
//         mod.dateRegister = query.value(8).toDateTime();
//         mod.isZip = query.value(9).toBool();
//         mod.garantMonth = query.value(10).toInt();
//         mod.EndGarant = query.value(11).toDateTime();
//         LoadStatus(mod);
//         listModul.push_back(mod);
//     }

// }

// void RepoMSSQL::LoadClaimProducts(int idClaim, QList<Product> &listProduct)
// {
//     listProduct.clear();
//     QSqlQuery query(db);
//     query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
//                   "from ClaimProduct cp "
//                   "join Product p on p.id=cp.idProduct "
//                   "where cp.idClaim=:idClaim");

//     query.bindValue(":idClaim", idClaim);
//     query.exec();
//     while(query.next())
//     {
//         Product prod;

//         prod.id = query.value(0).toInt();
//         prod.idShipment = query.value(1).toInt();
//         prod.idSetterOut = query.value(2).toInt();
//         prod.idType = query.value(3).toInt();
//         prod.name = query.value(4).toString();
//         prod.number = query.value(5).toString();
//         prod.number2 = query.value(6).toString();
//         prod.dateRegister = query.value(7).toDateTime();
//         prod.redaction1 = query.value(8).toString();
//         prod.redaction2 = query.value(9).toString();
//         prod.redactionPS = query.value(10).toString();
//         prod.questList = query.value(11).toString();
//         prod.isAvr = query.value(12).toBool();
//         prod.isAkb = query.value(13).toBool();
//         prod.isCooler = query.value(14).toBool();
//         prod.isSkm = query.value(15).toBool();
//         prod.numberBI = query.value(16).toString();
//         prod.numberUSIKP = query.value(17).toString();
//         prod.shunt = query.value(18).toString();
//         prod.isZip = query.value(19).toBool();
//         prod.garantMonth = query.value(20).toInt();
//         prod.EndGarant = query.value(21).toDateTime();
//         LoadStatus(prod);
//         listProduct.push_back(prod);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Добавление модуля в рекламацию
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::AddModulToClaim(int idModul, int idClaim)
// {
//     bool res;
//     QSqlQuery query(db);

//     query.prepare("insert into ClaimModule (idClaim,idModul) values(:idClaim,:idModul)");

//     query.bindValue(":idClaim", idClaim);
//     query.bindValue(":idModul", idModul);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в AddModulToClaim";

//     return res;

// }

// bool RepoMSSQL::DelModulFromClaim(int idModul, int idClaim)
// {
//     bool res;
//     QSqlQuery query(db);

//     query.prepare("delete from ClaimModule where idClaim=:idClaim and idModul=:idModul");

//     query.bindValue(":idClaim", idClaim);
//     query.bindValue(":idModul", idModul);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при удалении записи в DelModulFromClaim";

//     return res;

// }

//------------------------------------------------------------------------------------------------------
// Добавление изделия в рекламацию
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddItemToClaim(int idItem, int idClaim)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into ClaimItems (idClaim,idItem) values(:idClaim,:idItem)");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idItem", idItem);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в AddItemToClaim";

    return res;

}

bool RepoMSSQL::DelItemFromClaim(int idItem, int idClaim)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("delete from ClaimItems where idClaim=:idClaim and idItem=:idItem");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idItem", idItem);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в DelItemFromClaim";

    return res;

}

Claim RepoMSSQL::GetClaimForItem(int idItem)
{
    QSqlQuery query(db);
    Claim claim;

    query.prepare("select c.id,Number,dateClaim,typeClaimId,idOrg,ObjectInstall,o.OrgName,isClosed "
                  "from ClaimItems ci "
                  "join Claim c on c.id=ci.idClaim and c.isClosed=0 "
                  "join Organization o on o.id=c.idOrg "
                  "where idItem=:idItem");

    query.bindValue(":idItem", idItem);

    query.exec();
    if(query.next())
    {
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateCreate = query.value(2).toDateTime();
        claim.idTypeClaim = query.value(3).toInt();
        claim.idOrg = query.value(4).toInt();
        claim.ObjectInstall = query.value(5).toString();
        claim.nameOrganization = query.value(6).toString();
        claim.isClosed = query.value(7).toBool();
    }

    // if(!res)
    //     qDebug() << "Ошибка в GetClaimForItem";

    return claim;

}

// bool RepoMSSQL::LoadClaimForProduct(int ProdId, Claim &claim)
// {
//     bool res = false;
//     QSqlQuery query(db);
//     query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
//                   "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
//                   "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult "
//                   "from ClaimProduct cp join Claim c on c.id=cp.idClaim where cp.idProduct=:idProd");

//     query.bindValue(":idProd", ProdId);

//     query.exec();
//     while(query.next())
//     {
//         claim.id = query.value(0).toInt();
//         claim.number = query.value(1).toString();
//         claim.dateRegister = query.value(2).toDateTime();
//         claim.FromWho = query.value(3).toString();
//         claim.idTypeClaim = query.value(4).toInt();
//         claim.idOrg = query.value(5).toInt();
//         claim.ObjectInstall = query.value(6).toString();
//         claim.Descript = query.value(7).toString();
//         claim.TypeComplectId = query.value(8).toInt();
//         claim.VNFT = query.value(9).toString();
//         claim.Quantity = query.value(10).toInt();
//         claim.idTypeClaim = query.value(11).toInt();
//         claim.NumberModul = query.value(12).toString();
//         claim.NumberNewModul = query.value(13).toString();
//         claim.NumberDevice = query.value(14).toString();
//         claim.DateOut = query.value(15).toDateTime();
//         claim.IsGuarantee = query.value(16).toBool();
//         claim.Reason = query.value(17).toString();
//         claim.DateRepair = query.value(18).toDateTime();
//         claim.DoRepair = query.value(19).toString();
//         claim.FileAnswer = query.value(20).toString();
//         claim.TextResult = query.value(21).toString();
//         res = true;
//     }
//     return res;
// }


Claim RepoMSSQL::GetClaim(int id)
{
    QSqlQuery query(db);
    Claim claim;

    query.prepare("select c.id,Number,dateClaim,typeClaimId,idOrg,ObjectInstall,o.orgName,isClosed "
                  "from Claim c "
                  "join Organization o on o.id=c.idOrg "
                  "where c.id=:id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateCreate = query.value(2).toDateTime();
        claim.idTypeClaim = query.value(3).toInt();
        claim.idOrg = query.value(4).toInt();
        claim.ObjectInstall = query.value(5).toString();
        claim.nameOrganization = query.value(6).toString();
        claim.isClosed = query.value(7).toBool();

    }
    return claim;
}

//------------------------------------------------------------------------------------------------------
// Загрузка причин ремонта
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadRemontReason(QMap<int, QString> &listReason)
{
    listReason.clear();
    QSqlQuery query(db);
    query.prepare("select id,name from RemontReason");

    query.exec();
    while(query.next())
    {
        listReason.insert(query.value(0).toInt(), query.value(1).toString());
    }
}


//------------------------------------------------------------------------------------------------------
// Загрузка причины ремонта
//------------------------------------------------------------------------------------------------------
QString RepoMSSQL::GetRemontReason(int id)
{
    QString s;
    QSqlQuery query(db);
    query.prepare("select name from RemontReason where id=:id");
    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        s = query.value(0).toString();
    }

    return s;
}


//------------------------------------------------------------------------------------------------------
// Добавление ремонта
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddRemont(Remont &remont)
{
    bool res;
    QSqlQuery query(db);

    query.prepare("insert into Remont (idItem,idClaim,dateStart) "
              "output inserted.id values(:idItem,:idClaim,:dateStart)");
    query.bindValue(":idItem", remont.idItem);

    query.bindValue(":idClaim", remont.idClaim);
    query.bindValue(":dateStart", remont.startDate);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в Remont";
    else
    {
        if(query.next())
            remont.id = query.value(0).toInt();
    }

    return res;

}

//------------------------------------------------------------------------------------------------------
// Обновление ремонта
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::UpdateRemont(Remont &remont)
{
    bool res;
    QSqlQuery query(db);
        query.prepare("update Remont set idReason=:idReason,Action=:Action,Defect=:Defect,Remark=:Remark,endDate=:endDate "
                  "where id=:id");

    query.bindValue(":id", remont.id);
    query.bindValue(":idReason", remont.idReason);
    query.bindValue(":Action", remont.action);
    query.bindValue(":Defect", remont.defect);
    query.bindValue(":Remark", remont.remark);
    query.bindValue(":endDate", remont.endDate);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при обновлении записи в Remont";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Загрузка ремонтов для id
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadRemont(QList<Remont> &list, int idItem)
{
    list.clear();
    QSqlQuery query(db);

    query.prepare("select id,idItem,idClaim,idReason,dateStart,Action,Defect,Remark,endDate "
                  "from Remont where idItem=:idItem");
    query.bindValue(":idItem", idItem);

    query.exec();
    while(query.next())
    {
        Remont rem;
        rem.id = query.value(0).toInt();
        rem.idItem = query.value(1).toInt();
        rem.idClaim = query.value(2).toInt();
        rem.idReason = query.value(3).toInt();
        rem.startDate = query.value(4).toDateTime();
        rem.action = query.value(5).toString();
        rem.defect = query.value(6).toString();
        rem.remark = query.value(7).toString();
        rem.endDate = query.value(8).toDateTime();
        list.push_back(rem);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка ремонта для id
//------------------------------------------------------------------------------------------------------
Remont RepoMSSQL::GetRemontForItem(int idItem)
{
    QSqlQuery query(db);
    Remont rem;

    query.prepare("select id,idItem,idClaim,idReason,dateStart,Action,Defect,Remark,endDate "
                      "from Remont where endDate is null and idItem=:idItem");
    query.bindValue(":idItem", idItem);

    query.exec();
    if(query.next())
    {
        rem.id = query.value(0).toInt();
        rem.idItem = query.value(1).toInt();
        rem.idClaim = query.value(2).toInt();
        rem.idReason = query.value(3).toInt();
        rem.startDate = query.value(4).toDateTime();
        rem.action = query.value(5).toString();
        rem.defect = query.value(6).toString();
        rem.remark = query.value(7).toString();
        rem.endDate = query.value(8).toDateTime();
    }
    return rem;
}


//------------------------------------------------------------------------------------------------------
// Получение количества зарагестрированных плат по накладной
//------------------------------------------------------------------------------------------------------
int RepoMSSQL::GetCountRegisterPlate(QString numDoc, int idType)
{
    QSqlQuery query(db);
    int res = 0;

    query.prepare("select count(*) from Items where NumberDoc=:NumberDoc and idType=:idType");
    query.bindValue(":NumberDoc", numDoc);
    query.bindValue(":idType", idType);
    query.exec();
    if(query.next())
    {
        res = query.value(0).toInt();
    }
    return res;
}


int32_t RepoMSSQL::GetNextNumber(uint year)
{
    int32_t res = -1;
    QSqlQuery query(db);
    query.exec(QString("select next value for GenSerial%1").arg(year));
    if(query.next())
        res = query.value(0).toInt();
    return res;
}

void RepoMSSQL::CreateGenerator(uint year)
{
    QSqlQuery query(db);
    query.exec(QString("CREATE SEQUENCE GenSerial%1 as int START WITH 1 INCREMENT BY 1").arg(year));

}

void RepoMSSQL::RestartSerialNumber(uint year)
{
    QSqlQuery query(db);
    query.exec(QString("ALTER SEQUENCE GenSerial%1 restart").arg(year));
}

int RepoMSSQL::GetCurrentNumber(uint year)
{
    qDebug() << year;

    int res = -1;
    QSqlQuery query(db);
    QString sql = QString("select current_value from sys.sequences where name = 'GenSerial%1'").arg(year);
    query.exec(sql);
    if(query.next())
    {
        QByteArray ba = query.value(0).toByteArray();
        QDataStream ds(ba);
        ds.setByteOrder(QDataStream::LittleEndian);
        ds >> res;
        qDebug() << ba << res;
    }
    return res;

}

