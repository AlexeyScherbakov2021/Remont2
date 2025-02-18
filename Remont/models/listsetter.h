#ifndef LISTSETTER_H
#define LISTSETTER_H

#include "listdevice.h"
#include "setterout.h"


class ListSetter : public ListEntity<SetterOut>
{
    friend class SetterModel;

public:
    explicit ListSetter();

    // QList<SetterOut> listItems;

    // ListDevice interface
public:
    // SetterOut GetItem(int id) ;
    // void LoadChild(SetterOut &item) ;
    // void Load() ;
    // void getData(int row, int col) const ;
    // bool DeleteItem(int id) ;
private:
    // RepoMSSQL repo;


    // ListEntity interface
public:
    bool AddItem(SetterOut &item) override;
    int GetRowFromId(int id) override;
    bool DeleteItemId(int id) override;
    void DeleteItemFromList(int row) override;
    bool DeleteItem(int row) override;
    bool UpdateItem(SetterOut &item) override;
    SetterOut GetItem(int id) override;
    SetterOut* GetItemAtRow(int row) override;
    bool LoadChild(SetterOut &item) override;
    QVariant getData(int row, int col, int role) const override;
    void setData(int row, int col, const QVariant value, int role) const override;
    void setItem(int row, SetterOut *setter);
    void GetHeader(QStringList &headers) override;
    int LoadPart(int, int, const QString &, bool);
};

#endif // LISTSETTER_H
