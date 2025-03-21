#include "testbase.h"

#include <models/Items.h>

TestBase::TestBase()
{

}

void TestBase::TestURAItem()
{
    // добавдление item в базу
    Items item;
    item.id = -1;
    item.idType = 7;
    item.number = "ControlTest";
    repo.AddItem(item);

    // загрузка item из базы
    Items item2 =  repo.GetItem(item.id);

    // обновление item
    item.name = "TestName";
    repo.UpdateItem(item);
    Items item3 =  repo.GetItem(item.id);

    // добавление статуса
    Status stat;
    stat.idItem = item.id;
    stat.idStatus = StatusItem::FAULTY_ON_OSO;
    repo.AddStatus(item, stat);

    // загрузка списка статусов
    repo.LoadStatus(item);
    Status stat2 = item.listStatus.last();

    // удаление последнего статуса
    repo.DelLastStatus(item, StatusItem::FAULTY_ON_OSO);
    repo.LoadStatus(item);

    // удаление item
    repo.DeleteItem(item.id);

    // попытка получения item
    Items item4 =  repo.GetItem(item.id);

    // проверка id вставленного item
    QVERIFY(item2.id == item.id);
    // проверка наименования измененного item
    QVERIFY(item3.name == item.name);
    // проверка добавленного статуса
    QVERIFY(stat2.idStatus == stat.idStatus);
    // проверка размера списка статуса после удаления
    QVERIFY(item.listStatus.size() == 0);
    // проверка удаления item
    QVERIFY(item4.id == 0);


}
