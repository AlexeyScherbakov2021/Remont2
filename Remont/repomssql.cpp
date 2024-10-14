
#include <models/claim.h>
#include <models/modul.h>
#include <models/product.h>
#include <models/remontm.h>
#include <models/setterout.h>
#include <models/shipment.h>
#include<models/plate.h>
#include <models/prodtype.h>
#include <models/modultype.h>
#include <QSqlRecord>
#include "repomssql.h"

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

// #ifdef QT_DEBUG
    // qDebug() << "Отладочная версия.";
    // db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;Encrypt=yes;Trusted_Connection=yes");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;");
    db.setUserName("sa");
    db.setPassword("gonduras");

// #else
//     qDebug() << "Рабочая версия.";
//     db.setDatabaseName("DRIVER={SQL Server};SERVER=SFP\\FPSQLN;DATABASE=FinGoods;");
//     db.setUserName("fpLoginName");
//     db.setPassword("ctcnhjt,s");

// #endif

    if(!db.open())
    {
        // qDebug() << "Ошибка соединения с базой данных." << db.lastError().text();
        return false;
    }


    // qDebug() << "Содединение с базой успешно.";
    return true;
}

bool RepoMSSQL::AddItem(Product &prod)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into Product (g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth) "
                  "output inserted.id values(:g_ProductTypeId,:g_name,:g_number,:g_numberBox,:g_dateRegister,"
                  ":g_redaction1,:g_redaction2,:g_redactionPS,:g_questList,:g_avr,:g_akb,:g_cooler,:g_skm,:g_numberBI,"
                  ":g_numberUSIKP,:g_shunt,:g_zip,:g_garantMonth)");

    query.bindValue(":g_ProductTypeId", prod.idType);
    query.bindValue(":g_name", prod.name);
    query.bindValue(":g_number", prod.number);
    query.bindValue(":g_numberBox", prod.number2);
    query.bindValue(":g_dateRegister", prod.dateRegister);
    query.bindValue(":g_redaction1", prod.redaction1);
    query.bindValue(":g_redaction2", prod.redaction2);
    query.bindValue(":g_redactionPS", prod.redactionPS);
    query.bindValue(":g_questList", prod.questList);
    query.bindValue(":g_avr", prod.isAvr);
    query.bindValue(":g_akb", prod.isAkb);
    query.bindValue(":g_cooler", prod.isCooler);
    query.bindValue(":g_skm", prod.isSkm);
    query.bindValue(":g_numberBI", prod.numberBI);
    query.bindValue(":g_numberUSIKP", prod.numberUSIKP);
    query.bindValue(":g_shunt", prod.shunt);
    query.bindValue(":g_zip", prod.isZip);
    query.bindValue(":g_garantMonth", prod.garantMonth);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в Product";
    else
    {
        if(query.next())
            prod.id = query.value(0).toInt();
    }

    return res;
}

bool RepoMSSQL::AddItem(Modul &modul)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into Modules (m_modTypeId,m_name,m_number,m_numberFW,m_dateCreate,m_zip,m_garantMonth) "
                  "output inserted.id values(:m_modTypeId,:m_name,:m_number,:m_numberFW,:m_dateCreate,:m_zip,:m_garantMonth)");

    query.bindValue(":m_modTypeId", modul.idType);
    query.bindValue(":m_name", modul.name);
    query.bindValue(":m_number", modul.number);
    query.bindValue(":m_numberFW", modul.number2);
    query.bindValue(":m_dateCreate", modul.dateRegister);
    query.bindValue(":m_zip", modul.isZip);
    query.bindValue(":m_garantMonth", modul.garantMonth);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в Modules";
    else
    {
        if(query.next())
            modul.id = query.value(0).toInt();
    }
    return res;
}

bool RepoMSSQL::AddItem(Plate &plate)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into Plate (CreateDate,Number,NumberFW,NumberDoc,VNFT) "
                  "output inserted.id values(:CreateDate,:Number,:NumberFW,:NumberDoc,:VNFT)");

    query.bindValue(":CreateDate", plate.dateRegister);
    query.bindValue(":Number", plate.number);
    query.bindValue(":NumberFW", plate.number2);
    query.bindValue(":NumberDoc", plate.numberDoc);
    query.bindValue(":VNFT", plate.VNFT);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в RemontM";
    else
    {
        if(query.next())
            plate.id = query.value(0).toInt();
    }

    return res;
}

bool RepoMSSQL::UpdateItem(Product &prod)
{
    bool res;
    QSqlQuery query;

    query.prepare("update Product set idShipment=:idShipment,idSetter=:idSetter,g_ProductTypeId=:g_ProductTypeId"
                  ",g_name=:g_name,g_number=:g_number,g_numberBox=:g_numberBox,g_dateRegister=:g_dateRegister,"
                  "g_questList=:g_questList,g_avr=:g_avr,g_akb=:g_akb,g_cooler=:g_cooler,g_skm=:g_skm,"
                  "g_numberBI=:g_numberBI,g_numberUSIKP=:g_numberUSIKP,g_shunt=:g_shunt,g_zip=:g_zip,"
                  "g_garantMonth=:g_garantMonth,g_endGarant=:g_endGarant where id=:id");

    QVariant var = prod.idShipment > 0 ? prod.idShipment : QVariant();
    query.bindValue(":idShipment", var);

    var = prod.idSetterOut > 0 ? prod.idSetterOut : QVariant();
    query.bindValue(":idSetter", var);

    query.bindValue(":g_ProductTypeId", prod.idType);
    query.bindValue(":g_name", prod.name);
    query.bindValue(":g_number", prod.number);
    query.bindValue(":g_numberBox", prod.number2);
    query.bindValue(":g_dateRegister", prod.dateRegister);
    query.bindValue(":g_questList", prod.questList);
    query.bindValue(":g_avr", prod.isAvr);
    query.bindValue(":g_akb", prod.isAkb);
    query.bindValue(":g_cooler", prod.isCooler);
    query.bindValue(":g_skm", prod.isSkm);
    query.bindValue(":g_numberBI", prod.numberBI);
    query.bindValue(":g_numberUSIKP", prod.numberUSIKP);
    query.bindValue(":g_shunt", prod.shunt);
    query.bindValue(":g_zip", prod.isZip);
    query.bindValue(":g_garantMonth", prod.garantMonth);
    query.bindValue(":g_endGarant", prod.EndGarant);

    query.bindValue(":id", prod.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в Product";

    return res;
}

bool RepoMSSQL::UpdateItem(Modul &modul)
{
    bool res;
    QSqlQuery query;

    query.prepare("update Modules set idShipment=:idShipment,idProduct=:idProduct,m_modTypeId=:m_modTypeId,m_name=:m_name,"
                  "m_number=:m_number,m_numberFW=:m_numberFW,m_dateCreate=:m_dateCreate,m_zip=:m_zip,"
                  "m_garantMonth=:m_garantMonth,m_endGarant=:m_endGarant "
                  "where id=:id");

    QVariant var = modul.idShipment > 0 ? modul.idShipment : QVariant();
    query.bindValue(":idShipment", var);
    var = modul.idProduct > 0 ? modul.idProduct : QVariant();
    query.bindValue(":idProduct", var);
    query.bindValue(":m_modTypeId", modul.idType);
    query.bindValue(":m_name", modul.name);
    query.bindValue(":m_number", modul.number);
    query.bindValue(":m_numberFW", modul.number2);
    query.bindValue(":m_dateCreate", modul.dateRegister);
    query.bindValue(":m_zip", modul.isZip);
    query.bindValue(":m_garantMonth", modul.garantMonth);
    query.bindValue(":m_endGarant", modul.EndGarant);
    query.bindValue(":id", modul.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при изменении записи в Modules";

    return res;
}

bool RepoMSSQL::UpdateItem(Plate &/*plate*/)
{
    return true;
}

//------------------------------------------------------------------------------------------------------
// Удаление изделия
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteProduct(int id)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from Product where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Product";

    return res;
}

//------------------------------------------------------------------------------------------------------
// Удаление модуля
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeleteModul(int id)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from Modules where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в Modules";

    return res;

}



//------------------------------------------------------------------------------------------------------
// Удаление платы
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::DeletePlate(int id)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from Plate where id=:id");
    query.bindValue(":id", id);
    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в RemontM";

    return res;

}


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
void RepoMSSQL::FindItems(const QString &serialNumber, QList<Product> &listProduct, int status, bool isFree)
{
    listProduct.clear();
    QSqlQuery query;

    if(serialNumber.isEmpty())
        return FindItems(listProduct, status, isFree);


    if(status == Status::NONE)
    {
        if(isFree)
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                      "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                      "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                      "from Product where g_number like :number order by g_name");
        else
        {
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                          "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                          "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                          "from Product where g_number like :number and idShipment is null and idSetter is null order by g_name");
        }
    }
    else
    {
        if(isFree)
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                      "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                      "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                      "from Product p "
                      "join "
                      "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                      "from ProductStatus group by idProduct "
                      "having max(idStatus)=:idStatus "
                      ") ms on ms.idProduct=p.id where g_number like :number order by g_name"
                      );
        else
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                          "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                          "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                          "from Product p "
                          "join "
                          "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ProductStatus group by idProduct "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idProduct=p.id where g_number like :number and idShipment is null and idSetter is null order by g_name"
                          );

        query.bindValue(":idStatus", status);
    }
    query.bindValue(":number", QString("%%1%").arg(serialNumber));

    query.exec();
    while(query.next())
    {
        Product prod;

        prod.id = query.value(0).toInt();
        prod.idShipment = query.value(1).toInt();
        prod.idSetterOut = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateRegister = query.value(7).toDateTime();
        prod.redaction1 = query.value(8).toString();
        prod.redaction2 = query.value(9).toString();
        prod.redactionPS = query.value(10).toString();
        prod.questList = query.value(11).toString();
        prod.isAvr = query.value(12).toBool();
        prod.isAkb = query.value(13).toBool();
        prod.isCooler = query.value(14).toBool();
        prod.isSkm = query.value(15).toBool();
        prod.numberBI = query.value(16).toString();
        prod.numberUSIKP = query.value(17).toString();
        prod.shunt = query.value(18).toString();
        prod.isZip = query.value(19).toBool();
        prod.garantMonth = query.value(20).toInt();
        prod.EndGarant = query.value(21).toDateTime();
        LoadStatus(prod/*, this*/);
        listProduct.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Поиск изделий
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Product> &listProduct, int status, bool isFree)
{
    listProduct.clear();
    QSqlQuery query;


    if(status == Status::NONE)
    {
        if(isFree)
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                  "from Product order by g_name");
        else
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                          "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                          "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                          "from Product where idShipment is null and idSetter is null order by g_name");
    }
    else
    {
        if(isFree)
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                  "from Product p "
                  "join "
                  "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                  "from ProductStatus group by idProduct "
                  "having max(idStatus)=:idStatus "
                  ") ms on ms.idProduct=p.id order by g_name"
                  );
        else
            query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                          "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                          "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                          "from Product p "
                          "join "
                          "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ProductStatus group by idProduct "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idProduct=p.id where idShipment is null and idSetter is null order by g_name"
                          );

        query.bindValue(":idStatus", status);
    }

    query.exec();
    while(query.next())
    {
        Product prod;

        prod.id = query.value(0).toInt();
        prod.idShipment = query.value(1).toInt();
        prod.idSetterOut = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateRegister = query.value(7).toDateTime();
        prod.redaction1 = query.value(8).toString();
        prod.redaction2 = query.value(9).toString();
        prod.redactionPS = query.value(10).toString();
        prod.questList = query.value(11).toString();
        prod.isAvr = query.value(12).toBool();
        prod.isAkb = query.value(13).toBool();
        prod.isCooler = query.value(14).toBool();
        prod.isSkm = query.value(15).toBool();
        prod.numberBI = query.value(16).toString();
        prod.numberUSIKP = query.value(17).toString();
        prod.shunt = query.value(18).toString();
        prod.isZip = query.value(19).toBool();
        prod.garantMonth = query.value(20).toInt();
        prod.EndGarant = query.value(21).toDateTime();
        LoadStatus(prod/*, this*/);
        listProduct.push_back(prod);
    }
}


//------------------------------------------------------------------------------------------------------
// Поиск модулей
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Modul> &listModul, int status, bool isFree)
{
    listModul.clear();
    QSqlQuery query;

    if(status == Status::NONE)
    {
        if(isFree)
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                      "m_zip,m_garantMonth,m_endGarant "
                  "from Modules order by m_name");
        else
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                          "m_zip,m_garantMonth,m_endGarant "
                          "from Modules where idShipment is null order by m_name");
    }
    else
    {
        if(isFree)
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                    "m_zip,m_garantMonth,m_endGarant "
                    "from Modules m "
                    "join "
                    "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                    "from ModulStatus group by idModul "
                    "having max(idStatus)=:idStatus "
                    ") ms on ms.idModul=m.id order by m_name"
                  );
        else
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                          "m_zip,m_garantMonth,m_endGarant "
                          "from Modules m "
                          "join "
                          "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ModulStatus  group by idModul "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idModul=m.id  where idShipment is null order by m_name"
                          );

        query.bindValue(":idStatus", status);
    }

    query.exec();
    while(query.next())
    {
        Modul mod;

        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idProduct = query.value(2).toInt();
        mod.idType = query.value(3).toInt();
        mod.name = query.value(4).toString();
        mod.number = query.value(5).toString();
        mod.number2 = query.value(6).toString();
        mod.dateRegister = query.value(8).toDateTime();
        mod.isZip = query.value(9).toBool();
        mod.garantMonth = query.value(10).toInt();
        mod.EndGarant = query.value(11).toDateTime();
        LoadStatus(mod/*, this*/);
        listModul.push_back(mod);
    }
}

//------------------------------------------------------------------------------------------------------
// Поиск модулей по серийному номеру
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(const QString &serialNumber, QList<Modul> &listModul, int status, bool isFree)
{
    listModul.clear();
    QSqlQuery query;

    if(serialNumber.isEmpty())
        return FindItems(listModul, status, isFree);

    if(status == Status::NONE)
    {
        if(isFree)
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                      "m_zip,m_garantMonth,m_endGarant "
                      "from Modules where m_number like :number order by m_name");
        else
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                          "m_zip,m_garantMonth,m_endGarant "
                          "from Modules where m_number like :number and idShipment is null order by m_name");
    }
    else
    {
        if(isFree)
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                      "m_zip,m_garantMonth,m_endGarant "
                      "from Modules m "
                      "join "
                      "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                      "from ModulStatus group by idModul "
                      "having max(idStatus)=:idStatus "
                      ") ms on ms.idModul=m.id where m_number like :number order by m_name"
                      );
        else
            query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,"
                          "m_zip,m_garantMonth,m_endGarant "
                          "from Modules m "
                          "join "
                          "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                          "from ModulStatus group by idModul "
                          "having max(idStatus)=:idStatus "
                          ") ms on ms.idModul=m.id where m_number like :number and idShipment is null order by m_name"
                          );

        query.bindValue(":idStatus", status);
    }

    query.bindValue(":number", QString("%%1%").arg(serialNumber));

    query.exec();
    while(query.next())
    {
        Modul mod;

        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idProduct = query.value(2).toInt();
        mod.idType = query.value(3).toInt();
        mod.name = query.value(4).toString();
        mod.number = query.value(5).toString();
        mod.number2 = query.value(6).toString();
        mod.dateRegister = query.value(8).toDateTime();
        mod.isZip = query.value(9).toBool();
        mod.garantMonth = query.value(10).toInt();
        mod.EndGarant = query.value(11).toDateTime();
        LoadStatus(mod);
        listModul.push_back(mod);
    }
}



//------------------------------------------------------------------------------------------------------
// Загрузка списка статусов для изделия
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadStatus(Product &prod)
{
    prod.listStatus.clear();

    QSqlQuery query;
    query.prepare("select ms.id,idProduct,idStatus,DateStatus,Comment,sd.nameStatus "
                  "from ProductStatus ms "
                  "join StatusDevice sd on sd.id=ms.idStatus "
                  "where ms.idProduct=:id "
                  "order by ms.DateStatus");

    query.bindValue(":id", prod.id);

    query.exec();
    while(query.next())
    {
        Status stat;
        stat.id = query.value(0).toInt();
        stat.idDevice = query.value(1).toInt();
        stat.idStatus = (Status::Stat)query.value(2).toInt();
        stat.dateStatus = query.value(3).toDateTime();
        stat.Comment = query.value(4).toString();
        stat.nameStatus = query.value(5).toString();
        prod.listStatus.push_back(stat);
    }
}

//------------------------------------------------------------------------------------------------------
// Загрузка списка статусов для модуля
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadStatus(Modul &mod)
{
    mod.listStatus.clear();

    QSqlQuery query;
    query.prepare("select ms.id,idModul,idStatus,DateStatus,Comment,sd.nameStatus "
                  "from ModulStatus ms "
                  "join StatusDevice sd on sd.id=ms.idStatus "
                  "where ms.idModul=:id "
                  "order by ms.DateStatus");

    query.bindValue(":id", mod.id);

    query.exec();
    while(query.next())
    {
        Status stat;
        stat.id = query.value(0).toInt();
        stat.idDevice = query.value(1).toInt();
        stat.idStatus = (Status::Stat)query.value(2).toInt();
        stat.dateStatus = query.value(3).toDateTime();
        stat.Comment = query.value(4).toString();
        stat.nameStatus = query.value(5).toString();
        mod.listStatus.push_back(stat);
    }
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

void RepoMSSQL::LoadModuleType(QMap<int, ModulType> &listTypeModule)
{
    listTypeModule.clear();
    QSqlQuery query;
    query.prepare("select id,mt_name,mt_number,mt_garantMonth from ModuleType");

    query.exec();
    while(query.next())
    {
        ModulType mType;
        mType.id = query.value(0).toInt();
        mType.name = query.value(1).toString();
        mType.number = query.value(2).toString();
        mType.garantMonth = query.value(3).toInt();
        listTypeModule.insert(query.value(0).toInt(), mType);
    }

}


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

void RepoMSSQL::LoadProductType(QMap<int, ProductType> &listTypeProduct)
{
    listTypeProduct.clear();
    QSqlQuery query;
    query.prepare("select id,gt_name,gt_number,gt_garantMonth from ProductType");

    query.exec();
    while(query.next())
    {
        ProductType pType;
        pType.id = query.value(0).toInt();
        pType.name = query.value(1).toString();
        pType.number = query.value(2).toString();
        pType.garantMonth = query.value(3).toInt();
        listTypeProduct.insert(query.value(0).toInt(), pType);
    }

}

//------------------------------------------------------------------------------------------------------
// Поиск списка плат по номеру
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(const QString &number, QList<Plate> &listPlate, int /*status*/, bool /*isFree*/)
{
    QSqlQuery query;
    listPlate.clear();

    // if(number.isEmpty())
    //     return FindItems(listPlate, status);

    query.prepare("select id,CreateDate,Number,NumberFW,NumberDoc,VNFT "
                  "from Plate where Number like :Number and idModul is null");

    query.bindValue(":Number", QString("%%1%").arg(number));

    query.exec();
    while(query.next())
    {
        Plate plate;
        plate.id = query.value(0).toInt();
        plate.dateRegister = query.value(1).toDateTime();
        plate.number = query.value(2).toString();
        plate.number2 = query.value(3).toString();
        plate.numberDoc = query.value(4).toString();
        plate.VNFT = query.value(5).toString();
        listPlate.push_back(plate);
    }
}

void RepoMSSQL::FindItems(const QString &/*number*/, QList<Shipment> &listShip, int status)
{
    return FindItems(listShip, status);
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
void RepoMSSQL::LoadChildProduct(Product &prod)
{
    prod.listModules.clear();
    QSqlQuery query;
    query.prepare("select id,idShipment,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip,m_garantMonth,m_endGarant"
                  "from Modules where idProduct = :idProduct");

    query.bindValue(":idProduct", prod.id);

    query.exec();
    while(query.next())
    {
        Modul mod;
        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idProduct = prod.id;
        mod.idType = query.value(2).toInt();
        mod.name = query.value(3).toString();
        mod.number = query.value(4).toString();
        mod.number2 = query.value(5).toString();
        mod.dateUtil = query.value(6).toDateTime();
        mod.dateRegister = query.value(7).toDateTime();
        mod.isZip = query.value(8).toBool();
        mod.garantMonth = query.value(9).toInt();
        mod.EndGarant = query.value(10).toDateTime();
        prod.listModules.push_back(mod);
    }
}

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

Product RepoMSSQL::GetProduct(int id)
{
    Product prod;
    QSqlQuery query;

    query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                  "from Product where id=:id");

    query.exec();
    if(query.next())
    {
        prod.id = query.value(0).toInt();
        prod.idShipment = query.value(1).toInt();
        prod.idSetterOut = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateRegister = query.value(7).toDateTime();
        prod.redaction1 = query.value(8).toString();
        prod.redaction2 = query.value(9).toString();
        prod.redactionPS = query.value(10).toString();
        prod.questList = query.value(11).toString();
        prod.isAvr = query.value(12).toBool();
        prod.isAkb = query.value(13).toBool();
        prod.isCooler = query.value(14).toBool();
        prod.isSkm = query.value(15).toBool();
        prod.numberBI = query.value(16).toString();
        prod.numberUSIKP = query.value(17).toString();
        prod.shunt = query.value(18).toString();
        prod.isZip = query.value(19).toBool();
        prod.garantMonth = query.value(20).toInt();
        prod.EndGarant = query.value(21).toDateTime();
        LoadStatus(prod);
    }
    return prod;
}

//------------------------------------------------------------------------------------------------------
// Добавление новой отгрузки
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddItem(Shipment &ship)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into Shipment (c_number,c_objectInstall,c_dateOut,c_customer,c_questList,c_schet,"
                  "c_cardOrder,c_numberUPD,c_buyer,c_dateUPD) "
                  "output inserted.id values(:c_number,:c_objectInstall,:c_dateOut,:c_customer,:c_questList,:c_schet,"
                  ":c_cardOrder,:c_numberUPD,:c_buyer,:c_dateUPD)");

    query.bindValue(":c_number", ship.number);
    query.bindValue(":c_objectInstall", ship.objectInstall);
    query.bindValue(":c_dateOut", ship.dateRegister);
    query.bindValue(":c_customer", ship.customer);
    query.bindValue(":c_questList", ship.questList);
    query.bindValue(":c_schet", ship.schet);
    query.bindValue(":c_cardOrder", ship.cardOrder);
    query.bindValue(":c_numberUPD", ship.numberUPD);
    query.bindValue(":c_buyer", ship.buyer);
    query.bindValue(":c_dateUPD", ship.dateUPD);

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
                  "c_customer=:c_customer,c_questList=:c_questList,c_schet=:c_schet,c_cardOrder=:c_cardOrder,"
                  "c_numberUPD=:c_numberUPD,c_buyer=:c_buyer,c_dateUPD=:c_dateUPD where id=:id");

    query.bindValue(":c_number", ship.number);
    query.bindValue(":c_objectInstall", ship.objectInstall);
    query.bindValue(":c_dateOut", ship.dateRegister);
    query.bindValue(":c_customer", ship.customer);
    query.bindValue(":c_questList", ship.questList);
    query.bindValue(":c_schet", ship.schet);
    query.bindValue(":c_cardOrder", ship.cardOrder);
    query.bindValue(":c_numberUPD", ship.numberUPD);
    query.bindValue(":c_buyer", ship.buyer);
    query.bindValue(":c_dateUPD", ship.dateUPD);
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
                  "FileAnswer=:FileAnswer,TextResult=:TextResult "
                  "where id=:id");

    query.bindValue(":id", claim.id);
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

    query.prepare("select c_number,c_objectInstall,c_dateOut,c_customer,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD "
                  "from Shipment where id = :id");

    query.bindValue(":id", id);

    query.exec();
    if(query.next())
    {
        ship.id = id;
        ship.number = query.value(0).toString();
        ship.objectInstall = query.value(1).toString();
        ship.dateRegister = query.value(2).toDateTime();
        ship.customer = query.value(3).toString();
        ship.questList = query.value(4).toString();
        ship.schet = query.value(5).toString();
        ship.cardOrder = query.value(6).toString();
        ship.numberUPD = query.value(7).toString();
        ship.buyer = query.value(8).toString();
        ship.dateUPD = query.value(9).toDateTime();
    }
    return ship;

}

Modul RepoMSSQL::GetModul(int /*id*/)
{
    Modul mod;
    return mod;
}

//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadShipment(QList<Shipment> &listShip, bool /*isFinish*/)
{
    listShip.clear();
    QSqlQuery query;

    query.prepare("select c_number,c_objectInstall,c_dateOut,c_customer,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,idOrganization "
                  "from Shipment where c_dateOut is null");

    query.exec();
    while(query.next())
    {
        Shipment ship;
        ship.number = query.value(0).toString();
        ship.objectInstall = query.value(1).toString();
        ship.dateRegister = query.value(2).toDateTime();
        ship.customer = query.value(3).toString();
        ship.questList = query.value(4).toString();
        ship.schet = query.value(5).toString();
        ship.cardOrder = query.value(6).toString();
        ship.numberUPD = query.value(7).toString();
        ship.buyer = query.value(8).toString();
        ship.dateUPD = query.value(9).toDateTime();
        ship.id = query.value(10).toInt();
        ship.isOrganization = query.value(11).toInt();
        listShip.push_back(ship);
    }

}

//------------------------------------------------------------------------------------------------------
// Загрузка изделий для набора
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadChildSetter(SetterOut &setter)
{
    setter.listProduct.clear();
    QSqlQuery query;
    query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip "
                  "from Product where idSetter = :idSetter");

    query.bindValue(":idSetter", setter.id);

    query.exec();
    while(query.next())
    {
        Product prod;

        prod.id = query.value(0).toInt();
        prod.idShipment = query.value(1).toInt();
        prod.idSetterOut = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateRegister = query.value(7).toDateTime();
        prod.redaction1 = query.value(8).toString();
        prod.redaction2 = query.value(9).toString();
        prod.redactionPS = query.value(10).toString();
        prod.questList = query.value(11).toString();
        prod.isAvr = query.value(12).toBool();
        prod.isAkb = query.value(13).toBool();
        prod.isCooler = query.value(14).toBool();
        prod.isSkm = query.value(15).toBool();
        prod.numberBI = query.value(16).toString();
        prod.numberUSIKP = query.value(17).toString();
        prod.shunt = query.value(18).toString();
        prod.isZip = query.value(19).toBool();
        LoadStatus(prod);
        setter.listProduct.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Добавление статуса модулю
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddStatus(Modul &modul, Status &status)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into ModulStatus (idModul,idStatus,DateStatus,Comment) "
                  "output inserted.id values(:idModul,:idStatus,:DateStatus,:Comment)");

    query.bindValue(":idModul", modul.id);
    query.bindValue(":idStatus", status.idStatus);
    query.bindValue(":DateStatus", status.dateStatus);
    query.bindValue(":Comment", status.Comment);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в ModulStatus";
    else
    {
        if(query.next())
            status.id = query.value(0).toInt();
    }

    return res;
}

//------------------------------------------------------------------------------------------------------
// Добавление статуса изделию
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddStatus(Product &product, Status &status)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into ProductStatus (idProduct,idStatus,DateStatus,Comment) "
                  "output inserted.id values(:idProduct,:idStatus,:DateStatus,:Comment)");

    query.bindValue(":idProduct", product.id);
    query.bindValue(":idStatus", status.idStatus);
    query.bindValue(":DateStatus", status.dateStatus);
    query.bindValue(":Comment", status.Comment);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при добавлении записи в ModulStatus";
    else
    {
        if(query.next())
            status.id = query.value(0).toInt();
    }

    return res;

}

bool RepoMSSQL::DelLastStatus(Modul &modul)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from ModulStatus where id = (select Top(1) id FROM ModulStatus where idModul=:idModul "
                  "order by DateStatus desc)");

    query.bindValue(":idModul", modul.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при удалении записи в ModulStatus";

    return res;

}

bool RepoMSSQL::DelLastStatus(Product &product)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from ProductStatus where id = (select Top(1) id FROM ProductStatus where idProduct=:idProduct "
                  "order by DateStatus desc)");

    query.bindValue(":idProduct", product.id);

    res = query.exec();
    if(!res)
        qDebug() << "Ошибка при удалении записи в ProductStatus";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Загрузка списка отгрузок
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Shipment> &listShip, int /*isFinish*/)
{
    listShip.clear();
    QSqlQuery query;

    query.prepare("select c_number,c_objectInstall,c_dateOut,c_customer,c_questList,"
                  "c_schet,c_cardOrder,c_numberUPD,c_buyer,c_dateUPD,id,idOrganization "
                  "from Shipment where c_dateOut is null");

    query.exec();
    while(query.next())
    {
        Shipment ship;
        ship.number = query.value(0).toString();
        ship.objectInstall = query.value(1).toString();
        ship.dateRegister = query.value(2).toDateTime();
        ship.customer = query.value(3).toString();
        ship.questList = query.value(4).toString();
        ship.schet = query.value(5).toString();
        ship.cardOrder = query.value(6).toString();
        ship.numberUPD = query.value(7).toString();
        ship.buyer = query.value(8).toString();
        ship.dateUPD = query.value(9).toDateTime();
        ship.id = query.value(10).toInt();
        ship.idOrganization = query.value(11).toInt();
        listShip.push_back(ship);
    }

}

void RepoMSSQL::LoadOrganization(QMap<int, QString> &listOrg)
{
    QSqlQuery query;
    listOrg.clear();

    query.prepare("select id,OrgName from Organization");

    query.exec();
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        listOrg.insert(id, name);
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
void RepoMSSQL::LoadShipModule(QList<Modul> &listModul, int idShip)
{
    listModul.clear();
    QSqlQuery query;

    query.prepare("select id,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip "
                  "from Modules where idShipment=:idShipment");

    query.bindValue(":idShipment", idShip);

    query.exec();
    while(query.next())
    {
        Modul mod;
        mod.idShipment = idShip;
        mod.id = query.value(0).toInt();
        mod.idProduct = query.value(1).toInt();
        mod.idType = query.value(2).toInt();
        mod.name = query.value(3).toString();
        mod.number = query.value(4).toString();
        mod.number2 = query.value(5).toString();
        mod.dateRegister = query.value(7).toDateTime();
        mod.isZip = query.value(8).toBool();
        LoadStatus(mod);
        listModul.push_back(mod);
    }

}

//------------------------------------------------------------------------------------------------------
// Загрузка изделий в отгрузке
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadShipProduct(QList<Product> &listProduct, int idShip)
{
    listProduct.clear();
    QSqlQuery query;

    query.prepare("select id,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip "
                  "from Product where idShipment = :idShipment");

    query.bindValue(":idShipment", idShip);

    query.exec();
    while(query.next())
    {
        Product prod;
        prod.id = query.value(0).toInt();
        prod.idShipment = idShip;
        prod.idSetterOut = query.value(1).toInt();
        prod.idType = query.value(2).toInt();
        prod.name = query.value(3).toString();
        prod.number = query.value(4).toString();
        prod.number2 = query.value(5).toString();
        prod.dateRegister = query.value(6).toDateTime();
        prod.redaction1 = query.value(7).toString();
        prod.redaction2 = query.value(8).toString();
        prod.redactionPS = query.value(9).toString();
        prod.questList = query.value(10).toString();
        prod.isAvr = query.value(11).toBool();
        prod.isAkb = query.value(12).toBool();
        prod.isCooler = query.value(13).toBool();
        prod.isSkm = query.value(14).toBool();
        prod.numberBI = query.value(15).toString();
        prod.numberUSIKP = query.value(16).toString();
        prod.shunt = query.value(17).toString();
        prod.isZip = query.value(18).toBool();
        LoadStatus(prod);
        listProduct.push_back(prod);

    }

}

void RepoMSSQL::LoadClaim(QList<Claim> &listClaim)
{
    listClaim.clear();
    QSqlQuery query;
    query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult,"
                  "ct.nameType "
                  "from Claim c "
                  "join ClaimType ct on ct.id=c.TypeClaimId");

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
        // claim.TypeComplectString = query.value(23).toString();
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
void RepoMSSQL::LoadClaimModules(int idClaim, QList<Modul> &listModul)
{
    listModul.clear();
    QSqlQuery query;

    query.prepare("select m.id,m.idShipment,m.idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,"
                  "m_dateCreate,m_zip,m_garantMonth,m_endGarant "
                  "from ClaimModule cm "
                  "join Modules m on m.id=cm.idModul where cm.idClaim=:idClaim"
                  );

    query.bindValue(":idClaim", idClaim);

    query.exec();
    while(query.next())
    {
        Modul mod;

        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idProduct = query.value(2).toInt();
        mod.idType = query.value(3).toInt();
        mod.name = query.value(4).toString();
        mod.number = query.value(5).toString();
        mod.number2 = query.value(6).toString();
        mod.dateRegister = query.value(8).toDateTime();
        mod.isZip = query.value(9).toBool();
        mod.garantMonth = query.value(10).toInt();
        mod.EndGarant = query.value(11).toDateTime();
        LoadStatus(mod);
        listModul.push_back(mod);
    }

}

void RepoMSSQL::LoadClaimProducts(int idClaim, QList<Product> &listProduct)
{
    listProduct.clear();
    QSqlQuery query;
    query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip,g_garantMonth,g_endGarant "
                  "from ClaimProduct cp "
                  "join Product p on p.id=cp.idProduct "
                  "where cp.idClaim=:idClaim");

    query.bindValue(":idClaim", idClaim);
    query.exec();
    while(query.next())
    {
        Product prod;

        prod.id = query.value(0).toInt();
        prod.idShipment = query.value(1).toInt();
        prod.idSetterOut = query.value(2).toInt();
        prod.idType = query.value(3).toInt();
        prod.name = query.value(4).toString();
        prod.number = query.value(5).toString();
        prod.number2 = query.value(6).toString();
        prod.dateRegister = query.value(7).toDateTime();
        prod.redaction1 = query.value(8).toString();
        prod.redaction2 = query.value(9).toString();
        prod.redactionPS = query.value(10).toString();
        prod.questList = query.value(11).toString();
        prod.isAvr = query.value(12).toBool();
        prod.isAkb = query.value(13).toBool();
        prod.isCooler = query.value(14).toBool();
        prod.isSkm = query.value(15).toBool();
        prod.numberBI = query.value(16).toString();
        prod.numberUSIKP = query.value(17).toString();
        prod.shunt = query.value(18).toString();
        prod.isZip = query.value(19).toBool();
        prod.garantMonth = query.value(20).toInt();
        prod.EndGarant = query.value(21).toDateTime();
        LoadStatus(prod);
        listProduct.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Добавление модуля в рекламацию
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddModulToClaim(int idModul, int idClaim)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into ClaimModule (idClaim,idModul) values(:idClaim,:idModul)");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idModul", idModul);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в AddModulToClaim";

    return res;

}

bool RepoMSSQL::DelModulFromClaim(int idModul, int idClaim)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from ClaimModule where idClaim=:idClaim and idModul=:idModul");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idModul", idModul);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в DelModulFromClaim";

    return res;

}

//------------------------------------------------------------------------------------------------------
// Добавление изделия в рекламацию
//------------------------------------------------------------------------------------------------------
bool RepoMSSQL::AddProductToClaim(int idProd, int idClaim)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into ClaimProduct (idClaim,idProduct) values(:idClaim,:idProduct)");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idProduct", idProd);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при добавлении записи в AddProductToClaim";

    return res;

}

bool RepoMSSQL::DelProductToClaim(int idProd, int idClaim)
{
    bool res;
    QSqlQuery query;

    query.prepare("delete from ClaimProduct where idClaim=:idClaim and idProduct=:idProduct");

    query.bindValue(":idClaim", idClaim);
    query.bindValue(":idProduct", idProd);

    res = query.exec();

    if(!res)
        qDebug() << "Ошибка при удалении записи в DelProductToClaim";

    return res;

}

bool RepoMSSQL::LoadClaimForProduct(int ProdId, Claim &claim)
{
    bool res = false;
    QSqlQuery query;
    query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult "
                  "from ClaimProduct cp join Claim c on c.id=cp.idClaim where cp.idProduct=:idProd");

    query.bindValue(":idProd", ProdId);

    query.exec();
    while(query.next())
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
        res = true;
    }
    return res;
}



bool RepoMSSQL::LoadClaimForModul(int ModulId, Claim &claim)
{
    bool res = false;
    QSqlQuery query;
    query.prepare("select c.id,Number,DateClaim,FromWho,TypeClaimId,idOrg,ObjectInstall,"
                  "Descript,TypeComplectId,VNFT,Quantity,TypeDeviceId,NumberModul,NumberNewModul,"
                  "NumberDevice,DateOut,Guarantee,Reason,DateRepair,DoRepair,FileAnswer,TextResult "
                  "from ClaimModule cm join Claim c on c.id=cm.idClaim where cm.idModul=:idModul");

    query.bindValue(":idModul", ModulId);

    query.exec();
    while(query.next())
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
        res = true;
    }
    return res;

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

