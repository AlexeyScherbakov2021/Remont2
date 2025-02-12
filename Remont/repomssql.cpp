
#include <models/claim.h>
// #include <models/modul.h>
// #include <models/product.h>
#include <models/setterout.h>
#include <models/shipment.h>
// #include<models/plate.h>
// #include <models/prodtype.h>
// #include <models/modultype.h>
#include <models/remont.h>

// #include <QElapsedTimer>
// #include <models/platetype.h>
#include <models/organization.h>
#include<models/Items.h>
#include <QMessageBox>
#include <QSqlRecord>
#include "repomssql.h"
// #include <infrastructure/IStatus.h>

class Product;

RepoMSSQL::RepoMSSQL()
{

    if(!QSqlDatabase::contains(db.defaultConnection))
    {
        db = QSqlDatabase::addDatabase("QODBC");
        ConnectDb();
    }
}

// RepoMSSQL::~RepoMSSQL()
// {
//     db.close();
// }

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
        QMessageBox::critical(0, "Ошибка", "Ошибка соединения с базой данных.");
        std::terminate();
        return false;
    }


    // qDebug() << "Содединение с базой успешно.";
    return true;
}

// bool RepoMSSQL::AddItem(Product &prod)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into Product (g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth) "
//                   "output inserted.id values(:g_ProductTypeId,:g_name,:g_number,:g_numberBox,:g_dateRegister,"
//                   ":g_redaction1,:g_redaction2,:g_redactionPS,:g_questList,:g_avr,:g_akb,:g_cooler,:g_skm,:g_numberBI,"
//                   ":g_numberUSIKP,:g_shunt,:g_zip,:g_garantMonth)");

//     query.bindValue(":g_ProductTypeId", prod.idType);
//     query.bindValue(":g_name", prod.name);
//     query.bindValue(":g_number", prod.number);
//     query.bindValue(":g_numberBox", prod.number2);
//     query.bindValue(":g_dateRegister", prod.dateRegister);
//     query.bindValue(":g_redaction1", prod.redaction1);
//     query.bindValue(":g_redaction2", prod.redaction2);
//     query.bindValue(":g_redactionPS", prod.redactionPS);
//     query.bindValue(":g_questList", prod.questList);
//     query.bindValue(":g_avr", prod.isAvr);
//     query.bindValue(":g_akb", prod.isAkb);
//     query.bindValue(":g_cooler", prod.isCooler);
//     query.bindValue(":g_skm", prod.isSkm);
//     query.bindValue(":g_numberBI", prod.numberBI);
//     query.bindValue(":g_numberUSIKP", prod.numberUSIKP);
//     query.bindValue(":g_shunt", prod.shunt);
//     query.bindValue(":g_zip", prod.isZip);
//     query.bindValue(":g_garantMonth", prod.garantMonth);

//     res = query.exec();
//     if(!res)
//     {
//         qDebug() << "Ошибка при добавлении записи в Product";
//         qDebug() << db.lastError().text();
//     }
//     else
//     {
//         if(query.next())
//             prod.id = query.value(0).toInt();
//     }

//     return res;
// }

// bool RepoMSSQL::AddItem(Modul &modul)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into Modules (m_modTypeId,m_name,m_number,m_numberFW,m_dateCreate,m_zip,m_garantMonth) "
//                   "output inserted.id values(:m_modTypeId,:m_name,:m_number,:m_numberFW,:m_dateCreate,:m_zip,:m_garantMonth)");

//     query.bindValue(":m_modTypeId", modul.idType);
//     query.bindValue(":m_name", modul.name);
//     query.bindValue(":m_number", modul.number);
//     query.bindValue(":m_numberFW", modul.number2);
//     query.bindValue(":m_dateCreate", modul.dateRegister);
//     query.bindValue(":m_zip", modul.isZip);
//     query.bindValue(":m_garantMonth", modul.garantMonth);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в Modules";
//     else
//     {
//         if(query.next())
//             modul.id = query.value(0).toInt();
//     }
//     return res;
// }

// bool RepoMSSQL::AddItem(Plate &plate)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into Plate (CreateDate,Number,NumberFW,NumberDoc,idPlateType) "
//                   "output inserted.id values(:CreateDate,:Number,:NumberFW,:NumberDoc,:idPlateType)");

//     query.bindValue(":CreateDate", plate.dateRegister);
//     query.bindValue(":Number", plate.number);
//     query.bindValue(":NumberFW", plate.number2);
//     query.bindValue(":NumberDoc", plate.numberDoc);
//     // query.bindValue(":VNFT", plate.VNFT);
//     query.bindValue(":idPlateType", plate.idType);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в Plate";
//     else
//     {
//         if(query.next())
//             plate.id = query.value(0).toInt();
//     }

//     return res;
// }

// bool RepoMSSQL::UpdateItem(Product &prod)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("update Product set idShipment=:idShipment,idSetter=:idSetter,g_ProductTypeId=:g_ProductTypeId"
//                   ",g_name=:g_name,g_number=:g_number,g_numberBox=:g_numberBox,g_dateRegister=:g_dateRegister,"
//                   "g_questList=:g_questList,g_avr=:g_avr,g_akb=:g_akb,g_cooler=:g_cooler,g_skm=:g_skm,"
//                   "g_numberBI=:g_numberBI,g_numberUSIKP=:g_numberUSIKP,g_shunt=:g_shunt,g_zip=:g_zip,"
//                   "g_garantMonth=:g_garantMonth,g_endGarant=:g_endGarant,g_dateOn=:g_dateOn where id=:id");

//     QVariant var = prod.idShipment > 0 ? prod.idShipment : QVariant();
//     query.bindValue(":idShipment", var);

//     var = prod.idSetterOut > 0 ? prod.idSetterOut : QVariant();
//     query.bindValue(":idSetter", var);

//     query.bindValue(":g_ProductTypeId", prod.idType);
//     query.bindValue(":g_name", prod.name);
//     query.bindValue(":g_number", prod.number);
//     query.bindValue(":g_numberBox", prod.number2);
//     query.bindValue(":g_dateRegister", prod.dateRegister);
//     query.bindValue(":g_questList", prod.questList);
//     query.bindValue(":g_avr", prod.isAvr);
//     query.bindValue(":g_akb", prod.isAkb);
//     query.bindValue(":g_cooler", prod.isCooler);
//     query.bindValue(":g_skm", prod.isSkm);
//     query.bindValue(":g_numberBI", prod.numberBI);
//     query.bindValue(":g_numberUSIKP", prod.numberUSIKP);
//     query.bindValue(":g_shunt", prod.shunt);
//     query.bindValue(":g_zip", prod.isZip);
//     query.bindValue(":g_garantMonth", prod.garantMonth);
//     query.bindValue(":g_endGarant", prod.EndGarant);
//     query.bindValue(":g_dateOn", prod.dateOn);

//     query.bindValue(":id", prod.id);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при изменении записи в Product";

//     return res;
// }

// bool RepoMSSQL::UpdateItem(Modul &/*mod*/)
// {
//     return false;
// }

bool RepoMSSQL::UpdateItem(Items &item)
{
    bool res;
    QSqlQuery query;

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
    QSqlQuery query;

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

    QSqlQuery query;

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
        item.type.indexType = (ItemType::IndexType)query.value(16).toInt();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        LoadStatus(item);
    }


    // Items item;
    // QSqlQuery query;

    // query.prepare("id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
    //               "dateOff,garantMonth,dateGarant,isZip "
    //               "from Items where id=:id");

    // query.bindValue(":id", id);
    // query.exec();
    // if(query.next())
    // {
    //     item.id = query.value(0).toInt();
    //     item.idParent = query.value(1).toInt();
    //     item.idShip = query.value(2).toInt();
    //     item.idSet = query.value(3).toInt();
    //     item.idType = query.value(4).toInt();
    //     item.number = query.value(5).toString();
    //     item.number2 = query.value(6).toString();
    //     item.numberDoc = query.value(7).toString();
    //     item.name = query.value(8).toString();
    //     item.dateCreate = query.value(9).toDateTime();
    //     item.dateOn = query.value(10).toDateTime();
    //     item.dateOff = query.value(11).toDateTime();
    //     item.garantMonth = query.value(12).toInt();
    //     item.dateGarant = query.value(13).toDateTime();
    //     item.isZip = query.value(14).toBool();
    //     item.LoadStatus(item);
    // }
    return item;
}

// Items RepoMSSQL::GetItem( QString number, int status, bool isFree) const
// {
//     Items item;
//     QSqlQuery query;

//     if(number.isEmpty())
//         return item;

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,garantMonth,dateGarant,isZip "
//                           "from Items where number like :number and  order by nameItem");
//         else
//         {
//             query.prepare("select id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,garantMonth,dateGarant,isZip "
//                           "from Items where number like :number and idShip is null and idSet is null order by nameItem");
//         }
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,garantMonth,dateGarant,isZip "
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id where number like :number order by nameItem"
//                           );
//         else
//             query.prepare("select id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                           "dateOff,garantMonth,dateGarant,isZip"
//                           "from Items i "
//                           "join "
//                           "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ItemStatus group by idItem "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idItem=i.id where number like :number and idShip is null and idSet is null order by nameItem"
//                           );

//         query.bindValue(":idStatus", status);
//     }
//     query.bindValue(":number", QString("%%1%").arg(number));

//     query.exec();
//     if(query.next())
//     {
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
//         item.LoadStatus(item);
//     }
//     return item;
// }


Items RepoMSSQL::GetItem2( QString number, QVector<int>& listStatus, bool isBusy, bool isParent) const
{
    Items item;

    QStringList slStatus;
    QSqlQuery query;

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

    sql.push_back(" where ");
    sql.push_back(slWhere.join(" and "));

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
        item.currStatus = query.value(16).toString();

        item.type.indexType = (ItemType::IndexType)query.value(17).toInt();
        item.type.typeName = query.value(15).toString();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        // item.commentStatus = query.value(20).toString();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        Status status;
        status.idItem = item.id;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (Status::Stat)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

    }

    return item;
}

void RepoMSSQL::LoadItemsType(QList<ItemType> &listType, ItemType::IndexType indexType) const
{
    listType.clear();
    QSqlQuery query;
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



// bool RepoMSSQL::UpdateItem(Plate &plate)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("update Plate set CreateDate=:CreateDate,Number=:Number,NumberFW=:NumberFW,NumberDoc=:NumberDoc,"
//                   "idPlateType=:idPlateType "
//                   "where id=:id");

//     QVariant var = plate.idType > 0 ? plate.idType : QVariant();
//     query.bindValue(":idPlateType", var);
//     query.bindValue(":CreateDate", plate.dateRegister);
//     query.bindValue(":Number", plate.number);
//     query.bindValue(":NumberFW", plate.number2);
//     query.bindValue(":NumberDoc", plate.numberDoc);
//     // query.bindValue(":VNFT", plate.VNFT);
//     query.bindValue(":id", plate.id);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при изменении записи в Plate";

//     return res;
// }

//------------------------------------------------------------------------------------------------------
// Удаление изделия
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::DeleteProduct(int id)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from Product where id=:id");
//     query.bindValue(":id", id);
//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при удалении записи в Product";

//     return res;
// }

//------------------------------------------------------------------------------------------------------
// Удаление модуля
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::DeleteModul(int id)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from Modules where id=:id");
//     query.bindValue(":id", id);
//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при удалении записи в Modules";

//     return res;

// }



//------------------------------------------------------------------------------------------------------
// Удаление платы
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::DeletePlate(int id)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from Plate where id=:id");
//     query.bindValue(":id", id);
//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в RemontM";

//     return res;

// }


//------------------------------------------------------------------------------------------------------
// Удаление отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteShipment(int id)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from Shipment where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Shipment";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Удаление набора
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteSetter(int id)
{
    bool res;
    QSqlQuery query;

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
    QSqlQuery query;

    query.prepare("delete from Claim where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Claim";

    return res;
}



//------------------------------------------------------------------------------------------------------
// Поиск изделий по строке серийного номера
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(const QString &serialNumber, QList<Product> &listProduct, int status, bool isFree)
// {
//     listProduct.clear();
//     QSqlQuery query;

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
//     QSqlQuery query;


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
//     QSqlQuery query;

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
//     QSqlQuery query;

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

//     QSqlQuery query;
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

//     QSqlQuery query;
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

//     QSqlQuery query;
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
    QSqlQuery query;
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
    QSqlQuery query;
    query.prepare("select NameStatus from StatusDevice where id=:id");
    query.bindValue(":id", idStatus);

    query.exec();
    if(query.next())
        nameStatus = query.value(0).toString();

    return nameStatus;
}

//------------------------------------------------------------------------------------------------------
// Загрузка типов модулей
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadModuleType(QMap<int, QString> &listTypeModule)
// {
//     listTypeModule.clear();
//     QSqlQuery query;
//     query.prepare("select id,mt_name from ModuleType");

//     query.exec();
//     while(query.next())
//     {
//         listTypeModule.insert(query.value(0).toInt(), query.value(1).toString());
//     }
// }

// void RepoMSSQL::LoadModuleType(QMap<int,ModulType> &listTypeModule)
// {
//     listTypeModule.clear();
//     QSqlQuery query;
//     query.prepare("select id,mt_name,mt_number,mt_garantMonth from ModuleType");

//     query.exec();
//     while(query.next())
//     {
//         ModulType mType;
//         mType.id = query.value(0).toInt();
//         mType.name = query.value(1).toString();
//         mType.number = query.value(2).toString();
//         mType.garantMonth = query.value(3).toInt();
//         listTypeModule.insert(query.value(0).toInt(), mType);
//     }

// }

// void RepoMSSQL::LoadModuleType(QList<ModulType> &listTypeModule)
// {
//     listTypeModule.clear();
//     QSqlQuery query;
//     query.prepare("select id,mt_name,mt_number,mt_garantMonth,mt_VNFT from ModuleType");

//     query.exec();
//     while(query.next())
//     {
//         ModulType mType;
//         mType.id = query.value(0).toInt();
//         mType.name = query.value(1).toString();
//         mType.number = query.value(2).toString();
//         mType.garantMonth = query.value(3).toInt();
//         mType.VNFT = query.value(4).toString();
//         listTypeModule.push_back(mType);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Загрузка типов изделий
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadProductType(QMap<int, QString> &listTypeProduct)
// {
//     listTypeProduct.clear();
//     QSqlQuery query;
//     query.prepare("select id,gt_name from ProductType");

//     query.exec();
//     while(query.next())
//     {
//         listTypeProduct.insert(query.value(0).toInt(), query.value(1).toString());
//     }
// }

// void RepoMSSQL::LoadProductType(QMap<int, ProductType> &listTypeProduct)
// {
//     listTypeProduct.clear();
//     QSqlQuery query;
//     query.prepare("select id,gt_name,gt_number,gt_garantMonth from ProductType");

//     query.exec();
//     while(query.next())
//     {
//         ProductType pType;
//         pType.id = query.value(0).toInt();
//         pType.name = query.value(1).toString();
//         pType.number = query.value(2).toString();
//         pType.garantMonth = query.value(3).toInt();
//         listTypeProduct.insert(query.value(0).toInt(), pType);
//     }

// }

// void RepoMSSQL::LoadProductType(QList<ProductType> &listTypeProduct)
// {
//     listTypeProduct.clear();
//     QSqlQuery query;
//     query.prepare("select id,gt_name,gt_number,gt_garantMonth,gt_VNFT from ProductType");

//     query.exec();
//     while(query.next())
//     {
//         ProductType pType;
//         pType.id = query.value(0).toInt();
//         pType.name = query.value(1).toString();
//         pType.number = query.value(2).toString();
//         pType.garantMonth = query.value(3).toInt();
//         pType.VNFT = query.value(4).toString();
//         listTypeProduct.push_back(pType);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Поиск списка плат по номеру
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::FindItems(const QString &number, QList<Plate> &listPlate, int /*status*/, bool isFree)
// {
//     QSqlQuery query;
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

void RepoMSSQL::FindItems(const QString &/*number*/, QList<Shipment> &listShip, int status, bool isFree)
{
    return FindItems(listShip, status, isFree);
}

//------------------------------------------------------------------------------------------------------
// Привязка платы к модулю
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::LinkPlate(int idPlate, int idModul)
{
    bool res;
    QSqlQuery query;

    query.prepare("update Plate set idModul=:idModul where id=:id");

    query.bindValue(":idModul", idModul);
    query.bindValue(":id", idPlate);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в RemontM";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Загрузка модулей для изделия
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadChildProduct(Product &prod)
// {
//     prod.listModules.clear();
//     QSqlQuery query;
//     query.prepare("select id,idShipment,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip,"
//                   "m_garantMonth,m_endGarant,m_dateOn "
//                   "from Modules where idProduct = :idProduct");

//     query.bindValue(":idProduct", prod.id);

//     query.exec();
//     while(query.next())
//     {
//         Modul mod;
//         mod.id = query.value(0).toInt();
//         mod.idShipment = query.value(1).toInt();
//         mod.idProduct = prod.id;
//         mod.idType = query.value(2).toInt();
//         mod.name = query.value(3).toString();
//         mod.number = query.value(4).toString();
//         mod.number2 = query.value(5).toString();
//         mod.dateUtil = query.value(6).toDateTime();
//         mod.dateRegister = query.value(7).toDateTime();
//         mod.isZip = query.value(8).toBool();
//         mod.garantMonth = query.value(9).toInt();
//         mod.EndGarant = query.value(10).toDateTime();
//         mod.dateOn = query.value(11).toDateTime();
//         prod.listModules.push_back(mod);
//     }
// }

//------------------------------------------------------------------------------------------------------
// Поиск набора по id
//------------------------------------------------------------------------------------------------------
SetterOut RepoMSSQL::GetSetter(int id)
{
    QSqlQuery query;
    SetterOut setter;

    query.prepare("select idShipment,s_name,s_orderNum "
                  "from SetterOut where id = :id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        setter.id = id;
        setter.idShipment = query.value(0).toInt();
        setter.name = query.value(1).toString();
        setter.orderNumber = query.value(2).toString();
    }

    return setter;
}

//------------------------------------------------------------------------------------------------------
// получение изделия по идентификатору
//------------------------------------------------------------------------------------------------------
// Product RepoMSSQL::GetProduct(int id)
// {
//     Product prod;
//     QSqlQuery query;

//     query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                   "from Product where id=:id");

//     query.bindValue(":id", id);
//     query.exec();
//     if(query.next())
//     {
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
//         LoadStatus(prod);
//     }
//     return prod;
// }

//------------------------------------------------------------------------------------------------------
// получение изделия по номеру
//------------------------------------------------------------------------------------------------------
// Product RepoMSSQL::GetProduct(QString number, int status, bool isFree)
// {
//     Product prod;
//     QSqlQuery query;

//     if(number.isEmpty())
//         return prod;

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product where g_number=:g_number");
//         else
//         {
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product where g_number=:g_number and idShipment is null and idSetter is null");
//         }
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product p "
//                           "join "
//                           "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ProductStatus group by idProduct "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idProduct=p.id where g_number=:g_number"
//                           );
//         else
//             query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                           "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                           "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                           "from Product p "
//                           "join "
//                           "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ProductStatus group by idProduct "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idProduct=p.id where g_number=:g_number and idShipment is null and idSetter is null"
//                           );

//         query.bindValue(":idStatus", status);
//     }
//     query.bindValue(":g_number", number);

//     query.exec();
//     if(query.next())
//     {
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
//         LoadStatus(prod);
//     }
//     return prod;
// }

//------------------------------------------------------------------------------------------------------
// Добавление новой отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddItem(Shipment &ship)
{
    bool res;
    QSqlQuery query;

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
    QSqlQuery query;

    query.prepare("insert into SetterOut (idShipment,s_name,s_OrderNum) "
                  "output inserted.id values(:idShipment,:s_name,:s_OrderNum)");

    query.bindValue(":idShipment", setter.idShipment);
    query.bindValue(":s_name", setter.name);
    query.bindValue(":s_OrderNum", setter.orderNumber);

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
    QSqlQuery query;

    query.prepare("insert into Claim (Number,DateClaim,FromWho,TypeClaimId,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult) "
                  "output inserted.id values(:Number,:DateClaim,:FromWho,:TypeClaimId,:ObjectInstall,"
                  ":Descript,:TypeComplectId,:VNFT,:Quantity,:TypeDeviceId,:NumberModul,:NumberNewModul,"
                  ":NumberDevice,:DateOut,:Guarantee,:Reason,:DateRepair,:DoRepair,:FileAnswer,:TextResult)");

    query.bindValue(":Number", claim.number);
    query.bindValue(":DateClaim", claim.dateRegister);
    query.bindValue(":FromWho", claim.FromWho);
    query.bindValue(":TypeClaimId", claim.idTypeClaim);
    // query.bindValue(":idOrg", claim.idOrg);
    query.bindValue(":ObjectInstall", claim.ObjectInstall);
    query.bindValue(":Descript", claim.Descript);
    query.bindValue(":TypeComplectId", claim.TypeComplectId);
    query.bindValue(":VNFT", claim.VNFT);
    query.bindValue(":Quantity", claim.Quantity);
    query.bindValue(":TypeDeviceId", claim.TypeDeviceId);
    query.bindValue(":NumberModul", claim.NumberModul);
    query.bindValue(":NumberNewModul", claim.NumberNewModul);
    query.bindValue(":NumberDevice", claim.NumberDevice);
    query.bindValue(":DateOut", claim.DateOut);
    query.bindValue(":Guarantee", claim.IsGuarantee);
    query.bindValue(":Reason", claim.Reason);
    query.bindValue(":DateRepair", claim.DateRepair);
    query.bindValue(":DoRepair", claim.DoRepair);
    query.bindValue(":FileAnswer", claim.FileAnswer);
    query.bindValue(":TextResult", claim.TextResult);

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
    QSqlQuery query;

    query.prepare("update Shipment set c_number=:c_number,c_objectInstall=:c_objectInstall,c_dateOut=:c_dateOut,"
                  "c_questList=:c_questList,c_schet=:c_schet,c_cardOrder=:c_cardOrder,"
                  "c_numberUPD=:c_numberUPD,c_buyer=:c_buyer,c_dateUPD=:c_dateUPD,idOrganization=:idOrganization where id=:id");

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
    query.bindValue(":id", ship.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в Shipment";

    return res;
}

bool RepoMSSQL::UpdateItem(SetterOut &/*setter*/)
{
    return false;
}

bool RepoMSSQL::UpdateItem(Claim &claim)
{
    bool res;
    QSqlQuery query;

    query.prepare("update Claim set Number=:Number,DateClaim=:DateClaim,FromWho=:FromWho,TypeClaimId=:TypeClaimId,"
                  "ObjectInstall=:ObjectInstall,Descript=:Descript,"
                  "TypeComplectId=:TypeComplectId,VNFT=:VNFT,Quantity=:Quantity,TypeDeviceId=:TypeDeviceId,"
                  "NumberModul=:NumberModul,NumberNewModul=:NumberNewModul,NumberDevice=:NumberDevice,"
                  "DateOut=:DateOut,Guarantee=:Guarantee,Reason=:Reason,DateRepair=:DateRepair,DoRepair=:DoRepair,"
                  "FileAnswer=:FileAnswer,TextResult=:TextResult,idOrg=:idOrg "
                  "where id=:id");

    query.bindValue(":id", claim.id);
    query.bindValue(":Number", claim.number);
    query.bindValue(":DateClaim", claim.dateRegister);
    query.bindValue(":FromWho", claim.FromWho);
    query.bindValue(":TypeClaimId", claim.idTypeClaim);
    if(claim.idOrg > 0)
        query.bindValue(":idOrg", claim.idOrg);
    query.bindValue(":ObjectInstall", claim.ObjectInstall);
    query.bindValue(":Descript", claim.Descript);
    query.bindValue(":TypeComplectId", claim.TypeComplectId);
    query.bindValue(":VNFT", claim.VNFT);
    query.bindValue(":Quantity", claim.Quantity);
    query.bindValue(":TypeDeviceId", claim.TypeDeviceId);
    query.bindValue(":NumberModul", claim.NumberModul);
    query.bindValue(":NumberNewModul", claim.NumberNewModul);
    query.bindValue(":NumberDevice", claim.NumberDevice);
    query.bindValue(":DateOut", claim.DateOut);
    query.bindValue(":Guarantee", claim.IsGuarantee);
    query.bindValue(":Reason", claim.Reason);
    query.bindValue(":DateRepair", claim.DateRepair);
    query.bindValue(":DoRepair", claim.DoRepair);
    query.bindValue(":FileAnswer", claim.FileAnswer);
    query.bindValue(":TextResult", claim.TextResult);
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
    QSqlQuery query;
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
// получение модуля по номеру
//------------------------------------------------------------------------------------------------------
// Modul RepoMSSQL::GetModul(QString number, int status, bool isFree)
// {
//     Modul mod;
//     QSqlQuery query;

//     if(number.isEmpty())
//         return mod;

//     if(status == Status::NONE)
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules where m_number=:number");
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules where m_number=:number and idShipment is null and idProduct is null");
//     }
//     else
//     {
//         if(!isFree)
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules m "
//                           "join "
//                           "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ModulStatus group by idModul "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idModul=m.id where m_number=:number"
//                           );
//         else
//             query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
//                           "m_zip,m_garantMonth,m_endGarant,m_dateOn "
//                           "from Modules m "
//                           "join "
//                           "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                           "from ModulStatus group by idModul "
//                           "having max(idStatus)=:idStatus "
//                           ") ms on ms.idModul=m.id where m_number=:number and idShipment is null and idProduct is null"
//                           );

//         query.bindValue(":idStatus", status);
//     }

//     query.bindValue(":number", number);

//     query.exec();
//     while(query.next())
//     {
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
//     }

//     return mod;
// }


//------------------------------------------------------------------------------------------------------
// получение платы по номеру
//------------------------------------------------------------------------------------------------------
// Plate RepoMSSQL::GetPlate(QString number)
// {
//     QSqlQuery query;
//     Plate plate;

//     if(!number.isEmpty())
//     {
//         query.prepare("select id,CreateDate,Number,NumberFW,NumberDoc,VNFT "
//                       "from Plate where Number=:Number and idModul is null");

//         query.bindValue(":Number", number);

//         query.exec();
//         if(query.next())
//         {
//             plate.id = query.value(0).toInt();
//             plate.dateRegister = query.value(1).toDateTime();
//             plate.number = query.value(2).toString();
//             plate.number2 = query.value(3).toString();
//             plate.numberDoc = query.value(4).toString();
//             plate.VNFT = query.value(5).toString();
//         }
//     }
//     return plate;
// }

//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadShipment(QList<Shipment> &listShip, bool /*isFinish*/)
{
    listShip.clear();
    QSqlQuery query;

    query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id "
                  "from Shipment where c_dateOut is null");

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
        // ship.customer = query.value(11).toString();
        listShip.push_back(ship);
    }

}

//------------------------------------------------------------------------------------------------------
// Загрузка изделий для набора
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadChildSetter(SetterOut &setter)
{
    setter.listItems.clear();
    QSqlQuery query;
    query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
                  "from Product where idSetter = :idSetter");

    query.bindValue(":idSetter", setter.id);

    query.exec();
    while(query.next())
    {
        Items prod;

        prod.id = query.value(0).toInt();
        prod.idShip = query.value(1).toInt();
        prod.idSet = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateCreate = query.value(7).toDateTime();
        // prod.redaction1 = query.value(8).toString();
        // prod.redaction2 = query.value(9).toString();
        // prod.redactionPS = query.value(10).toString();
        // prod.questList = query.value(11).toString();
        // prod.isAvr = query.value(12).toBool();
        // prod.isAkb = query.value(13).toBool();
        // prod.isCooler = query.value(14).toBool();
        // prod.isSkm = query.value(15).toBool();
        // prod.numberBI = query.value(16).toString();
        // prod.numberUSIKP = query.value(17).toString();
        // prod.shunt = query.value(18).toString();
        // prod.isZip = query.value(19).toBool();
        // prod.garantMonth = query.value(20).toInt();
        // prod.EndGarant = query.value(21).toDateTime();
        // prod.dateOn = query.value(22).toDateTime();
        LoadStatus(prod);
        setter.listItems.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Добавление статуса модулю
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::AddStatus(Modul &modul, Status &status)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into ModulStatus (idModul,idStatus,DateStatus,Comment) "
//                   "output inserted.id values(:idModul,:idStatus,:DateStatus,:Comment)");

//     query.bindValue(":idModul", modul.id);
//     query.bindValue(":idStatus", status.idStatus);
//     query.bindValue(":DateStatus", status.dateStatus);
//     query.bindValue(":Comment", status.Comment);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в ModulStatus";
//     else
//     {
//         if(query.next())
//             status.id = query.value(0).toInt();
//     }

//     return res;
// }

//------------------------------------------------------------------------------------------------------
// Добавление статуса изделию
//------------------------------------------------------------------------------------------------------
// bool RepoMSSQL::AddStatus(Product &product, Status &status)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into ProductStatus (idProduct,idStatus,DateStatus,Comment) "
//                   "output inserted.id values(:idProduct,:idStatus,:DateStatus,:Comment)");

//     query.bindValue(":idProduct", product.id);
//     query.bindValue(":idStatus", status.idStatus);
//     query.bindValue(":DateStatus", status.dateStatus);
//     query.bindValue(":Comment", status.Comment);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в ProductStatus";
//     else
//     {
//         if(query.next())
//             status.id = query.value(0).toInt();
//     }

//     return res;

// }

// bool RepoMSSQL::AddStatus(Plate &plate, Status &status)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into PlateStatus (idPlate,idStatus,DateStatus,Comment) "
//                   "output inserted.id values(:idPlate,:idStatus,:DateStatus,:Comment)");

//     query.bindValue(":idPlate", plate.id);
//     query.bindValue(":idStatus", status.idStatus);
//     query.bindValue(":DateStatus", status.dateStatus);
//     query.bindValue(":Comment", status.Comment);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в PlateStatus";
//     else
//     {
//         if(query.next())
//             status.id = query.value(0).toInt();
//     }
//     return res;
// }



// bool RepoMSSQL::DelLastStatus(Modul &modul)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from ModulStatus where id = (select Top(1) id FROM ModulStatus where idModul=:idModul "
//                   "order by DateStatus desc)");

//     query.bindValue(":idModul", modul.id);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при удалении записи в ModulStatus";

//     return res;

// }

// bool RepoMSSQL::DelLastStatus(Product &product)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from ProductStatus where id = (select Top(1) id FROM ProductStatus where idProduct=:idProduct "
//                   "order by DateStatus desc)");

//     query.bindValue(":idProduct", product.id);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при удалении записи в ProductStatus";

//     return res;

// }

// bool RepoMSSQL::DelLastStatus(Plate &plate)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from PlateStatus where id = (select Top(1) id FROM PlateStatus where idPlate=:idPlate "
//                   "order by DateStatus desc)");

//     query.bindValue(":idPlate", plate.id);

//     res = query.exec();
//     if(!res)
//         qDebug() << "Ошибка при удалении записи в PlateStatus";

//     return res;
// }

//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Shipment> &listShip, int /*isFinish*/, bool isFree)
{
    listShip.clear();
    QSqlQuery query;

    if(isFree)
        query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,c_customer "
                  "from Shipment where c_dateOut is null");
    else
        query.prepare("select c_number,c_objectInstall,c_dateOut,idOrganization,c_questList,"
                      "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,c_customer "
                      "from Shipment where c_dateOut is not null");

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
        ship.customer = query.value(11).toString();
        listShip.push_back(ship);
    }
}

void RepoMSSQL::FindItems(ItemType::IndexType iType, QList<Items> &listItems, int status, bool isFree)
{
    listItems.clear();
    QSqlQuery query;

    if(status == Status::NONE)
    {
        if(!isFree)
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem");
        else
        {
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i "
                          "join ItemType it on it.id=i.idType and it.indexType=:indexType "
                          "where idShip is null and idSet is null "
                          "order by nameItem");
        }

    }
    else
    {
        if(!isFree)
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i "
                          "join "
                          "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ItemStatus group by idItem "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idItem=i.id "
                          "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
                          );
        else
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip"
                          "from Items i "
                          "join "
                          "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ItemStatus group by idItem "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idItem=i.id where idShip is null and idSet is null "
                          "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
                          );

        query.bindValue(":idStatus", status);
    }

    // QElapsedTimer t;
    // t.start();
    // qDebug() << t.elapsed() << "Start";


    query.bindValue(":indexType", iType);
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
        // item.LoadStatus(item);
        listItems.push_back(item);
    }

    // qDebug() << t.elapsed() << "End";

}

bool RepoMSSQL::LoadChildItems(int idParent, QList<Items> &listItems) const
{
    bool res;
    listItems.clear();
    QSqlQuery query;
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

        item.type.indexType = (ItemType::IndexType)query.value(17).toInt();
        item.type.typeName = query.value(15).toString();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;
        Status status;
        status.idItem = item.id;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (Status::Stat)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        LoadChildItems(item.id, item.childItems);
        listItems.push_back(item);
    }

    return res;
}


int RepoMSSQL::LoadPart(size_t start, size_t count, ItemType::IndexType iType,
                           const QString &number, QList<Items> &listItems,
                           QVector<int>& listStatus, bool isBusy, bool isParent) const
{
    int res = 0;
    QStringList slStatus;
    QSqlQuery query;
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

    sql.push_back(" where ");

    sql.push_back(slWhere.join(" and "));

    if(listStatus.size() > 0)
    {
        for(int i = 0; i < listStatus.size(); ++i)
            slStatus.push_back(sqlStatus.arg(i));

        sql.push_back(" and (" + slStatus.join(" or ") + ")");
    }

    sql.push_back(" order by nameItem offset :start rows fetch next :count rows only");
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
        item.type.indexType = (ItemType::IndexType)query.value(17).toInt();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.dateGarant = query.value(20).toDateTime();
        item.type.id = item.idType;
        item.VNFT = item.type.VNFT;

        Status status;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (Status::Stat)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.idItem = item.id;
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        listItems.push_back(item);
        ++res;
    }

    return res;
}

int RepoMSSQL::LoadPartAll(size_t start, size_t count, const QString &number, QList<Items> &listItems, QVector<int> &listStatus, bool isBusy, bool isParent) const
{
    int res = 0;
    QStringList slStatus;
    QSqlQuery query;

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

    sql.push_back(" where ");
    sql.push_back(slWhere.join(" and "));

    if(listStatus.size() > 0)
    {
        for(int i = 0; i < listStatus.size(); ++i)
            slStatus.push_back(sqlStatus.arg(i));

        sql.push_back(" and (" + slStatus.join(" or ") + ")");
    }

    sql.push_back(" order by nameItem offset :start rows fetch next :count rows only");
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
        item.type.indexType = (ItemType::IndexType)query.value(17).toInt();
        item.type.VNFT = query.value(18).toString();
        item.type.garantMonth = query.value(19).toInt();
        item.VNFT = item.type.VNFT;
        item.type.id = item.idType;

        Status status;
        status.idItem = item.id;
        status.dateStatus = query.value(20).toDateTime();
        status.idStatus = (Status::Stat)query.value(21).toInt();
        status.Comment = query.value(22).toString();
        status.nameStatus = item.currStatus;
        item.listStatus.push_back(status);

        listItems.push_back(item);
        ++res;
    }

    return res;

}




// size_t RepoMSSQL::LoadPart(size_t start, size_t count, ItemType::IndexType iType,
//                          const QString &number, QList<Items> &listItems,
//                          int status, bool isBusy, bool isParent)
// {
//     size_t res = 0;
//     QSqlQuery query;

//     QString sqlNumber = " number like :number";
//     QString sqlBusy = " idShip is null and idSet is null";
//     QString sqlParent = " idParent is null";

//     QStringList sql = {"select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
//                        "dateOff,i.garantMonth,dateGarant,isZip,it.typeName,sd.NameStatus "
//                        "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
//                        "join (select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
//                        "from ItemStatus group by idItem ",
//                        "having max(idStatus)=:idStatus",
//                        ") ms on ms.idItem=i.id join StatusDevice sd on sd.id=ms.idStatus"};

//     if(status == Status::NONE)
//         sql.removeAt(1);
//         // sql.push_back(sqlStatus);

//     QStringList slWhere;

//     if(!number.isEmpty())
//         slWhere.push_back(sqlNumber);

//     if(!isBusy)
//         slWhere.push_back(sqlBusy);

//     if(!isParent)
//         slWhere.push_back(sqlParent);

//     sql.push_back(" where ");
//     sql.push_back(slWhere.join(" and "));

//     sql.push_back(" order by nameItem offset :start rows fetch next :count rows only");
//     QString sql2 = sql.join("");
//     query.prepare(sql2);
//     query.bindValue(":indexType", iType);
//     query.bindValue(":start", start);
//     query.bindValue(":count", count);
//     // if(!number.isEmpty())
//         query.bindValue(":number", QString("%%1%").arg(number));

//     // if(status != Status::NONE)
//         query.bindValue(":idStatus", status);

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
//         item.VNFT = query.value(15).toString();
//         item.currStatus = query.value(16).toString();
//         // item.LoadStatus(item);
//         listItems.push_back(item);
//         ++res;
//     }

//     return res;
// }


bool RepoMSSQL::AddItem(Items &item) const
{
    bool res;
    QSqlQuery query;

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



void RepoMSSQL::FindItems(ItemType::IndexType iType, const QString &number, QList<Items> &listItems, int status, bool isFree)
{
    listItems.clear();
    QSqlQuery query;

    if(number.isEmpty())
        return FindItems(iType, listItems, status, isFree);

    if(status == Status::NONE)
    {
        if(!isFree)
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
                          "where number like :number order by nameItem");
        else
        {
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i join ItemType it on it.id=i.idType and it.indexType=:indexType "
                          "where number like :number and idShip is null and idSet is null order by nameItem");
        }
    }
    else
    {
        // запрос с учетом статуса
        if(!isFree)
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip "
                          "from Items i "
                          "join "
                          "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ItemStatus group by idItem "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idItem=i.id where number like :number "
                          "join ItemType it on it.id=i.idType and it.indexType=:indexType order by nameItem"
                          );
        else
            query.prepare("select i.id,idParent,idShip,idSet,idType,number,number2,numberDoc,nameItem,dateCreate,dateOn,"
                          "dateOff,i.garantMonth,dateGarant,isZip"
                          "from Items i "
                          "join "
                          "(select idItem, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ItemStatus group by idItem "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idItem=i.id join ItemType it on it.id=i.idType and it.indexType=:indexType "
                          "where number like :number and idShip is null and idSet is null order by nameItem"
                          );

        query.bindValue(":idStatus", status);
    }

    query.bindValue(":indexType", iType);
    query.bindValue(":number", QString("%%1%").arg(number));

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
        // item.LoadStatus(item);
        // LoadStatus(prod);
        listItems.push_back(item);
    }

}

void RepoMSSQL::LoadStatus(Items& item) const
{
    item.listStatus.clear();

    QSqlQuery query;
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
        stat.idStatus = (Status::Stat)query.value(2).toInt();
        stat.dateStatus = query.value(3).toDateTime();
        stat.Comment = query.value(4).toString();
        stat.nameStatus = query.value(5).toString();
        stat.typeStatus = query.value(6).toInt();
        item.listStatus.push_back(stat);
    }

}

bool RepoMSSQL::AddStatus(Items &item, Status &status) const
{
    bool res;
    QSqlQuery query;

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
    QSqlQuery query;

    query.prepare("delete from ItemStatus where id = (select Top(1) id FROM ItemStatus where idItem=:idItem "
                  "order by DateStatus desc)");

    query.bindValue(":idItem", item.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при удалении записи в DelLastStatus";

    return res;
}

void RepoMSSQL::LoadTypeItem(ItemType::IndexType indexType, QVector<ItemType> &listType) const
{
    listType.clear();
    QSqlQuery query;
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
// Загрузка организаций
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadOrganization(QMap<int, QString> &listOrg)
{
    QSqlQuery query;
    listOrg.clear();

    query.prepare("select id,OrgName,INN,KPP from Organization where INN is not null or KPP is not null order by OrgName");
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

void RepoMSSQL::LoadOrganization(QList<Organization> &listOrg)
{
    QSqlQuery query;
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

//------------------------------------------------------------------------------------------------------
// Загрузка наборов в отгрузке
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadShipSetter(QList<SetterOut> &listSetter, int idShip)
{
    listSetter.clear();
    QSqlQuery query;

    query.prepare("select id,s_name,s_orderNum "
                  "from SetterOut where idShipment=:idShipment");

    query.bindValue(":idShipment", idShip);

    query.exec();
    while(query.next())
    {
        SetterOut setter;
        setter.id = query.value(0).toInt();
        setter.idShipment = idShip;
        setter.name = query.value(1).toString();
        setter.orderNumber = query.value(2).toString();
        listSetter.push_back(setter);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка модулей в отгрузке
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadShipModule(QList<Modul> &listModul, int idShip)
// {
//     listModul.clear();
//     QSqlQuery query;

//     query.prepare("select id,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip,"
//                   "m_garantMonth,m_endGarant,m_dateOn "
//                   "from Modules where idShipment=:idShipment");

//     query.bindValue(":idShipment", idShip);

//     query.exec();
//     while(query.next())
//     {
//         Modul mod;
//         mod.idShipment = idShip;
//         mod.id = query.value(0).toInt();
//         mod.idProduct = query.value(1).toInt();
//         mod.idType = query.value(2).toInt();
//         mod.name = query.value(3).toString();
//         mod.number = query.value(4).toString();
//         mod.number2 = query.value(5).toString();
//         mod.dateRegister = query.value(7).toDateTime();
//         mod.isZip = query.value(8).toBool();
//         mod.garantMonth = query.value(9).toInt();
//         mod.EndGarant = query.value(10).toDateTime();
//         mod.dateOn = query.value(11).toDateTime();
//         LoadStatus(mod);
//         listModul.push_back(mod);
//     }

// }

//------------------------------------------------------------------------------------------------------
// Загрузка изделий в отгрузке
//------------------------------------------------------------------------------------------------------
// void RepoMSSQL::LoadShipProduct(QList<Product> &listProduct, int idShip)
// {
//     listProduct.clear();
//     QSqlQuery query;

//     query.prepare("select id,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
//                   "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
//                   "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant,g_dateOn "
//                   "from Product where idShipment = :idShipment");

//     query.bindValue(":idShipment", idShip);

//     query.exec();
//     while(query.next())
//     {
//         Product prod;
//         prod.id = query.value(0).toInt();
//         prod.idShipment = idShip;
//         prod.idSetterOut = query.value(1).toInt();
//         prod.idType = query.value(2).toInt();
//         prod.name = query.value(3).toString();
//         prod.number = query.value(4).toString();
//         prod.number2 = query.value(5).toString();
//         prod.dateRegister = query.value(6).toDateTime();
//         prod.redaction1 = query.value(7).toString();
//         prod.redaction2 = query.value(8).toString();
//         prod.redactionPS = query.value(9).toString();
//         prod.questList = query.value(10).toString();
//         prod.isAvr = query.value(11).toBool();
//         prod.isAkb = query.value(12).toBool();
//         prod.isCooler = query.value(13).toBool();
//         prod.isSkm = query.value(14).toBool();
//         prod.numberBI = query.value(15).toString();
//         prod.numberUSIKP = query.value(16).toString();
//         prod.shunt = query.value(17).toString();
//         prod.isZip = query.value(18).toBool();
//         prod.garantMonth = query.value(19).toInt();
//         prod.EndGarant = query.value(20).toDateTime();
//         prod.dateOn = query.value(21).toDateTime();
//         LoadStatus(prod);
//         listProduct.push_back(prod);

//     }

// }

void RepoMSSQL::LoadClaim(QList<Claim> &listClaim)
{
    listClaim.clear();
    QSqlQuery query;
    query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult,"
                  "ct.nameType,o.orgName "
                  "from Claim c "
                  "join ClaimType ct on ct.id=c.TypeClaimId "
                  "left join Organization o on o.id=c.idOrg");

    query.exec();
    while(query.next())
    {
        Claim claim;
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateRegister = query.value(2).toDateTime();
        claim.FromWho = query.value(3).toString();
        claim.idTypeClaim = query.value(4).toInt();
        claim.idOrg = query.value(5).toInt();
        claim.ObjectInstall = query.value(6).toString();
        claim.Descript = query.value(7).toString();
        claim.TypeComplectId = query.value(8).toInt();
        claim.VNFT = query.value(9).toString();
        claim.Quantity = query.value(10).toInt();
        claim.TypeDeviceId = query.value(11).toInt();
        claim.NumberModul = query.value(12).toString();
        claim.NumberNewModul = query.value(13).toString();
        claim.NumberDevice = query.value(14).toString();
        claim.DateOut = query.value(15).toDateTime();
        claim.IsGuarantee = query.value(16).toBool();
        claim.Reason = query.value(17).toString();
        claim.DateRepair = query.value(18).toDateTime();
        claim.DoRepair = query.value(19).toString();
        claim.FileAnswer = query.value(20).toString();
        claim.TextResult = query.value(21).toString();
        claim.TypeClaimString = query.value(22).toString();
        claim.nameOrganization = query.value(23).toString();
        // claim.TypeDeviceString = query.value(24).toString();
        listClaim.push_back(claim);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка типов рекламаций
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadClaimType(QMap<int, QString> &listTypeClaim)
{
    listTypeClaim.clear();
    QSqlQuery query;
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
//     QSqlQuery query;

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
//     QSqlQuery query;
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
//     QSqlQuery query;

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
//     QSqlQuery query;

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
// bool RepoMSSQL::AddProductToClaim(int idProd, int idClaim)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("insert into ClaimProduct (idClaim,idProduct) values(:idClaim,:idProduct)");

//     query.bindValue(":idClaim", idClaim);
//     query.bindValue(":idProduct", idProd);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при добавлении записи в AddProductToClaim";

//     return res;

// }

// bool RepoMSSQL::DelProductToClaim(int idProd, int idClaim)
// {
//     bool res;
//     QSqlQuery query;

//     query.prepare("delete from ClaimProduct where idClaim=:idClaim and idProduct=:idProduct");

//     query.bindValue(":idClaim", idClaim);
//     query.bindValue(":idProduct", idProd);

//     res = query.exec();

//     if(!res)
//         qDebug() << "Ошибка при удалении записи в DelProductToClaim";

//     return res;

// }

// bool RepoMSSQL::LoadClaimForProduct(int ProdId, Claim &claim)
// {
//     bool res = false;
//     QSqlQuery query;
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



// bool RepoMSSQL::LoadClaimForModul(int ModulId, Claim &claim)
// {
//     bool res = false;
//     QSqlQuery query;
//     query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
//                   "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
//                   "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult "
//                   "from ClaimModule cm join Claim c on c.id=cm.idClaim where cm.idModul=:idModul");

//     query.bindValue(":idModul", ModulId);

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
//         claim.TypeDeviceId = query.value(11).toInt();
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
    QSqlQuery query;
    Claim claim;

    query.prepare("select id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult "
                  "from Claim where id=:id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        claim.id = query.value(0).toInt();
        claim.number = query.value(1).toString();
        claim.dateRegister = query.value(2).toDateTime();
        claim.FromWho = query.value(3).toString();
        claim.idTypeClaim = query.value(4).toInt();
        claim.idOrg = query.value(5).toInt();
        claim.ObjectInstall = query.value(6).toString();
        claim.Descript = query.value(7).toString();
        claim.TypeComplectId = query.value(8).toInt();
        claim.VNFT = query.value(9).toString();
        claim.Quantity = query.value(10).toInt();
        claim.idTypeClaim = query.value(11).toInt();
        claim.NumberModul = query.value(12).toString();
        claim.NumberNewModul = query.value(13).toString();
        claim.NumberDevice = query.value(14).toString();
        claim.DateOut = query.value(15).toDateTime();
        claim.IsGuarantee = query.value(16).toBool();
        claim.Reason = query.value(17).toString();
        claim.DateRepair = query.value(18).toDateTime();
        claim.DoRepair = query.value(19).toString();
        claim.FileAnswer = query.value(20).toString();
        claim.TextResult = query.value(21).toString();
    }
    return claim;

}

//------------------------------------------------------------------------------------------------------
// Загрузка причин ремонта
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadRemontReason(QMap<int, QString> &listReason)
{
    listReason.clear();
    QSqlQuery query;
    query.prepare("select id,name from RemontReason");

    query.exec();
    while(query.next())
    {
        listReason.insert(query.value(0).toInt(), query.value(1).toString());
    }
}

QString RepoMSSQL::GetRemontReason(int id)
{
    QString s;
    QSqlQuery query;
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
bool RepoMSSQL::AddRemont(Remont &remont, ev::DeviceKind kindDevice)
{
    bool res;
    QSqlQuery query;

    if(kindDevice == ev::PRODUCT)
    {
        query.prepare("insert into RemontP (idProduct,idReclamation,StartDate) "
                  "output inserted.id values(:idProduct,:idReclamation,:StartDate)");
        query.bindValue(":idProduct", remont.idParent);

    }
    else if(kindDevice == ev::MODUL)
    {
        query.prepare("insert into RemontM (idModul,idReclamation,StartDate) "
                      "output inserted.id values(:idModul,:idReclamation,:StartDate)");

        query.bindValue(":idModul", remont.idParent);
    }
    else
        return false;

    query.bindValue(":idReclamation", remont.idReclamation);
    query.bindValue(":StartDate", remont.startDate);

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
bool RepoMSSQL::UpdateRemont(Remont &remont, ev::DeviceKind kindDevice)
{
    bool res;
    QSqlQuery query;


    if(kindDevice == ev::PRODUCT)
    {
        query.prepare("update RemontP set idReason=:idReason,Action=:Action,Defect=:Defect,Remark=:Remark,endDate=:endDate "
                  "where id=:id");
    }
    else if(kindDevice == ev::MODUL)
        query.prepare("update RemontM set idReason=:idReason,Action=:Action,Defect=:Defect,Remark=:Remark,endDate=:endDate "
                      "where id=:id");
    else
        return false;

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
void RepoMSSQL::LoadRemont(QList<Remont> &list, int idParent, ev::DeviceKind kindDevice)
{
    list.clear();
    QSqlQuery query;

    if(kindDevice == ev::PRODUCT)
    {
        query.prepare("select id,idProduct,idReclamation,idReason,StartDate,Action,Defect,Remark,endDate "
                  "from RemontP where idProduct=:idProduct");
        query.bindValue(":idProduct", idParent);
    }
    else if(kindDevice == ev::MODUL)
    {
        query.prepare("select id,idModul,idReclamation,idReason,StartDate,Action,Defect,Remark,endDate "
                      "from RemontM where idModul=:idModul");
        query.bindValue(":idModul", idParent);

    }
    else
        return;

    query.exec();
    while(query.next())
    {
        Remont rem;
        rem.id = query.value(0).toInt();
        rem.idParent = query.value(1).toInt();
        rem.idReclamation = query.value(2).toInt();
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
Remont RepoMSSQL::GetCurrentRemont(int idParent, ev::DeviceKind kindDevice)
{
    QSqlQuery query;
    Remont rem;

    if(kindDevice == ev::PRODUCT)
    {
        query.prepare("select id,idProduct,idReclamation,idReason,StartDate,Action,Defect,Remark,endDate "
                      "from RemontP where endDate is null and idProduct=:idProduct");
        query.bindValue(":idProduct", idParent);
    }
    else if(kindDevice == ev::MODUL)
    {
        query.prepare("select id,idModul,idReclamation,idReason,StartDate,Action,Defect,Remark,endDate "
                      "from RemontM where endDate is null and idModul=:idModul");
        query.bindValue(":idModul", idParent);

    }
    else
        return rem;

    query.exec();
    if(query.next())
    {
        rem.id = query.value(0).toInt();
        rem.idParent = query.value(1).toInt();
        rem.idReclamation = query.value(2).toInt();
        rem.idReason = query.value(3).toInt();
        rem.startDate = query.value(4).toDateTime();
        rem.action = query.value(5).toString();
        rem.defect = query.value(6).toString();
        rem.remark = query.value(7).toString();
        rem.endDate = query.value(8).toDateTime();
    }
    return rem;
}


int RepoMSSQL::GetCountRegisterPlate(QString numDoc, int idType)
{
    QSqlQuery query;
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
    QSqlQuery query;
    query.exec(QString("select next value for GenSerial%1").arg(year));
    if(query.next())
        res = query.value(0).toInt();
    return res;
}

void RepoMSSQL::CreateGenerator(uint year)
{
    QSqlQuery query;
    query.exec(QString("CREATE SEQUENCE GenSerial%1 as int START WITH 1 INCREMENT BY 1").arg(year));

}

void RepoMSSQL::RestartSerialNumber(uint year)
{
    QSqlQuery query;
    query.exec(QString("ALTER SEQUENCE GenSerial%1 restart").arg(year));
}

int RepoMSSQL::GetCurrentNumber(uint year)
{
    qDebug() << year;

    int res = -1;
    QSqlQuery query;
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

