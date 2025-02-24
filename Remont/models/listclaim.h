#ifndef LISTCLAIM_H
#define LISTCLAIM_H

#include "claim.h"
#include "listdevice.h"


class ListClaim : public ListEntity<Claim>
{
    friend class ClaimModel;

public:
    ListClaim();

    void setItem(int row, Claim *clain);
    int LoadPart(int, int, const QString &);


    bool AddItem(Claim &item) override;
    int GetRowFromId(int id) override;
    bool DeleteItemId(int id) override;
    void DeleteItemFromList(int row) override;
    bool DeleteItem(int row) override;
    bool UpdateItem(Claim &item) override;
    Claim GetItem(int row) override;
    bool LoadChild(Claim &item) override;
    QVariant getData(int row, int col, int role = Qt::DisplayRole) const override;
    void setData(int row, int col, const QVariant value, int role = Qt::EditRole) const override;
    void GetHeader(QStringList &headers) override;
    // int LoadPart2(int start, int count, const QString &number, QVector<int> vStatus, bool isBusy, bool isParent) override;

    // bool Load(const QString Number = "");
    // int getColumnCount();
    // int getRowCount();

    Claim* GetItemAtRow(int row) override;
    void AddItemToList(Claim* item) override;

private:
    // QStringList headers;

};

#endif // LISTCLAIM_H
