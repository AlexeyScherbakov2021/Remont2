
#include <models/modul.h>
#include <models/product.h>
#include <models/remontm.h>
#include <models/setterout.h>
#include <models/shipment.h>
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

#ifdef QT_DEBUG
    // qDebug() << "Отладочная версия.";
    // db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;Encrypt=yes;Trusted_Connection=yes");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SCHERBAKOV-A\\SQLEXPRESS;DATABASE=FinGoods;");
    db.setUserName("sa");
    db.setPassword("gonduras");

#else
    qDebug() << "Рабочая версия.";
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SFP\\FPSQLN;DATABASE=FinGoods;");
    db.setUserName("fpLoginName");
    db.setPassword("ctcnhjt,s");

#endif

    if(!db.open())
    {
        // qDebug() << "Ошибка соединения с базой данных." << db.lastError().text();
        return false;
    }


    // qDebug() << "Содединение с базой успешно.";
    return true;
}

bool RepoMSSQL::AddItem(Product &/*prod*/)
{
    return true;
}

bool RepoMSSQL::AddItem(Modul &modul)
{
    bool res;
    QSqlQuery query;

    query.prepare("insert into Modules (m_modTypeId,m_name,m_number,m_numberFW,m_dateCreate) "
                  "output inserted.id values(:m_modTypeId,:m_name,:m_number,:m_numberFW,:m_dateCreate)");

    query.bindValue(":m_modTypeId", modul.idType);
    query.bindValue(":m_name", modul.name);
    query.bindValue(":m_number", modul.number);
    query.bindValue(":m_numberFW", modul.number2);
    query.bindValue(":m_dateCreate", modul.dateRegister);

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

bool RepoMSSQL::AddItem(Plate &/*plate*/)
{
    return true;
}

bool RepoMSSQL::UpdateItem(Product &/*prod*/)
{
    return true;

}

bool RepoMSSQL::UpdateItem(Modul &/*mod*/)
{
    return true;

}

bool RepoMSSQL::UpdateItem(Plate &/*plate*/)
{
    return true;
}

bool RepoMSSQL::DeleteItem(Product &/*prod*/)
{
    return true;
}

bool RepoMSSQL::DeleteItem(Modul &/*mod*/)
{
    return true;
}

bool RepoMSSQL::DeleteItem(Plate &/*plate*/)
{
    return true;
}

bool RepoMSSQL::DeleteItem(Shipment &/*ship*/)
{
    return true;

}

bool RepoMSSQL::DeleteItem(SetterOut &setter)
{
    return false;
}



//------------------------------------------------------------------------------------------------------
// Поиск изделий по строке серийного номера
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(const QString &serialNumber, QList<Product> &listProduct, int status)
{
    listProduct.clear();
    QSqlQuery query;


    if(status == Status::NONE)
    {
        query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                      "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                      "g_numberUSIKP,g_shunt,g_zip "
                      "from Product where g_number like :number order by g_name");

    }
    else
    {
        query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                      "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                      "g_numberUSIKP,g_shunt,g_zip "
                      "from Product p "
                      "join "
                      "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                      "from ProductStatus group by idProduct "
                      "having max(idStatus)=:idStatus "
                      ") ms on ms.idProduct=p.id where g_number like :number order by g_name"
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
        prod.prodTypeId = query.value(3).toInt();
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
        LoadStatus(prod/*, this*/);
        listProduct.push_back(prod);
    }

}

//------------------------------------------------------------------------------------------------------
// Поиск изделий
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Product> &listProduct, int status)
{
    listProduct.clear();
    QSqlQuery query;


    if(status == Status::NONE)
    {
        query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip "
                  "from Product order by g_name");
    }
    else
    {
        query.prepare("select id,idShipment,idSetter,g_ProductTypeId,g_name,g_number,g_numberBox,g_dateRegister,"
                  "g_redaction1,g_redaction2,g_redactionPS,g_questList,g_avr,g_akb,g_cooler,g_skm,g_numberBI,"
                  "g_numberUSIKP,g_shunt,g_zip "
                  "from Product p "
                  "join "
                  "(select idProduct, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                  "from ProductStatus group by idProduct "
                  "having max(idStatus)=:idStatus "
                  ") ms on ms.idProduct=p.id order by g_name"
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
        prod.prodTypeId = query.value(3).toInt();
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
        LoadStatus(prod/*, this*/);
        listProduct.push_back(prod);
    }
}


//------------------------------------------------------------------------------------------------------
// Поиск модулей
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(QList<Modul> &listModul, int status)
{
    listModul.clear();
    QSqlQuery query;

    if(status == Status::NONE)
    {
        query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip "
                  "from Modules order by m_name");
    }
    else
    {
        query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip "
                  "from Modules m "
                  "join "
                  "(select idModul, max(DateStatus) dateStatus, max(idStatus) as idStatus "
                  "from ModulStatus group by idModul "
                  "having max(idStatus)=:idStatus "
                  ") ms on ms.idModul=m.id order by m_name"
                  );
        query.bindValue(":idStatus", status);
    }

    query.exec();
    while(query.next())
    {
        Modul mod;

        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idParent = query.value(2).toInt();
        mod.idType = query.value(3).toInt();
        mod.name = query.value(4).toString();
        mod.number = query.value(5).toString();
        mod.number2 = query.value(6).toString();
        mod.dateRegister = query.value(8).toDateTime();
        mod.isZip = query.value(9).toBool();
        LoadStatus(mod/*, this*/);
        listModul.push_back(mod);
    }
}

//------------------------------------------------------------------------------------------------------
// Поиск модулей по серийному номеру
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(const QString &serialNumber, QList<Modul> &listModul, int /*status*/)
{
    listModul.clear();
    QSqlQuery query;
    query.prepare("select id,idShipment,idProduct,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip "
                  "from Modules where m_number like :number order by m_name");

    query.bindValue(":number", QString("%%1%").arg(serialNumber));

    query.exec();
    while(query.next())
    {
        Modul mod;

        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idParent = query.value(2).toInt();
        mod.idType = query.value(3).toInt();
        mod.name = query.value(4).toString();
        mod.number = query.value(5).toString();
        mod.number2 = query.value(6).toString();
        mod.dateRegister = query.value(8).toDateTime();
        mod.isZip = query.value(9).toBool();
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
void RepoMSSQL::LoadModuleType(QMap<int, QString> &listTypeModule)
{
    listTypeModule.clear();
    QSqlQuery query;
    query.prepare("select id,mt_name from ModuleType");

    query.exec();
    while(query.next())
    {
        listTypeModule.insert(query.value(0).toInt(), query.value(1).toString());
    }
}


//------------------------------------------------------------------------------------------------------
// Загрузка типов изделий
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::LoadProductType(QMap<int, QString> &listTypeProduct)
{
    listTypeProduct.clear();
    QSqlQuery query;
    query.prepare("select id,gt_name from ProductType");

    query.exec();
    while(query.next())
    {
        listTypeProduct.insert(query.value(0).toInt(), query.value(1).toString());
    }
}

//------------------------------------------------------------------------------------------------------
// Поиск списка плат по номеру
//------------------------------------------------------------------------------------------------------
void RepoMSSQL::FindItems(const QString &number, QList<Plate> &listPlate, int /*status*/)
{
    QSqlQuery query;
    listPlate.clear();

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
    query.prepare("select id,idShipment,m_modTypeId,m_name,m_number,m_numberFW,m_dateEnd,m_dateCreate,m_zip "
                  "from Modules where idProduct = :idProduct");

    query.bindValue(":idProduct", prod.id);

    query.exec();
    while(query.next())
    {
        Modul mod;
        mod.id = query.value(0).toInt();
        mod.idShipment = query.value(1).toInt();
        mod.idParent = prod.id;
        mod.idType = query.value(2).toInt();
        mod.name = query.value(3).toString();
        mod.number = query.value(4).toString();
        mod.number2 = query.value(5).toString();
        mod.isZip = query.value(8).toBool();
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

Product RepoMSSQL::GetProduct(int /*id*/)
{
    Product prod;
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
    return false;
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
        prod.prodTypeId = query.value(3).toInt();
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

