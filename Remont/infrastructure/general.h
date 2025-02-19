#ifndef GENERAL_H
#define GENERAL_H

enum IndexType : int { Product, Modul, Plate, ClaimType, SetterType, ShipType, All = 100 };

enum StatusItem : int {
    NONE,               // нет статуса
    CREATE,             // создан
    FAULTY,             // неисправен на производстве
    CORRECT,            // исправен на производстве
    INSTALL,            // установен в оборудовании
    SHIPPED,            // отгружен
    WORK,               // в работе
    FAULTY_ON_OBJECT,   // неисправен на объекте
    FAULTY_ON_OSO,      // неисправен в ОСО
    REMONT,             // в ремонте
    CORRECT_OSO,        // исправен б/у в осо
    END_WORK            // утилизирован
};


#endif // GENERAL_H
