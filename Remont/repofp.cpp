#include "repofp.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

RepoFP::RepoFP()
{
    if(!QSqlDatabase::contains("FP"))
    {
        db = QSqlDatabase::addDatabase("QODBC", "FP");
        ConnectDb();
    }
    else
        db = QSqlDatabase::database("FP");

}

// RepoFP::~RepoFP()
// {
//     db.close();
// }

bool RepoFP::ConnectDb()
{
    db.setDatabaseName("DRIVER={SQL Server};SERVER=SFP\\FPSQLN;DATABASE=Нефтегазкомплекс;");
    db.setUserName("fpLoginName");
    db.setPassword("ctcnhjt,s");
    // if(!db.open())
    // {
    //     QMessageBox::critical(0, "Ошибка", "Ошибка соединения с базой данных ФП.");
    //     return false;
    // }
    // qDebug() << "Содединение с базой ФП успешно.";
    return true;

}

// Nakl RepoFP::getDoc(const QString &number)
// {
//     Nakl nakl;
//     if(!db.open())
//     {
//         QMessageBox::critical(0, "Ошибка", "Ошибка соединения с базой данных ФП.");
//         qDebug() << db.lastError();
//         return nakl;
//     }

//     QSqlQuery query(db);
//     QString sql = "exec pr_Smena_NaklFind 0,%1,'%2',0";

//     query.exec(sql.arg(number).arg("lvwNC"));
//     while(query.next())
//     {
//         nakl.number = number;
//         nakl.name = query.value(1).toString();
//         nakl.VNFT = query.value(2).toString();
//         nakl.count = query.value(3).toInt();
//         nakl.plan = query.value(6).toString();
//     }

//     db.close();
//     return nakl;
// }

bool RepoFP::getDoc(const QString &number, QList<Nakl> &listNakl)
{
    bool res;
    listNakl.clear();
    if(!db.open())
    {
        QMessageBox::critical(0, "Ошибка", "Ошибка соединения с базой данных ФП.");
        qDebug() << db.lastError();
        return false;
    }

    QSqlQuery query(db);
    QString sql = "exec pr_Smena_NaklFind 0,%1,'%2',0";

    res = query.exec(sql.arg(number).arg("lvwNC"));
    while(query.next())
    {
        Nakl nakl;
        nakl.number = number;
        nakl.name = query.value(1).toString();
        nakl.VNFT = query.value(2).toString();
        nakl.count = query.value(3).toInt();
        nakl.plan = query.value(6).toString();
        listNakl.push_back(nakl);
    }

    db.close();
    return res;

}
