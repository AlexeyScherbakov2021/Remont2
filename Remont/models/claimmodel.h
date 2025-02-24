#ifndef CLAIMMODEL_H
#define CLAIMMODEL_H

#include "claim.h"
#include "listclaim.h"

#include <QAbstractTableModel>

class ClaimModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ClaimModel(QObject *parent = nullptr);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // bool setHeaderData(int section,
    //                    Qt::Orientation orientation,
    //                    const QVariant &value,
    //                    int role = Qt::EditRole) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setData(const QModelIndex &index, Claim* item);

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;



    void prepareLoad(const QString _number);
    Claim* GetItem(int row);
    bool DeleteItem(int row);
    void DeleteItemFromListId(int id);
    void DeleteItemFromList(int row);

    void createList();
    void AddItem(Claim* item);
    void UpdateItem(int row);
    void setBaseOff();

private:
    int startLoad = 0;
    int cntLoad = 60;
    bool isFetch = false;
    QStringList headers;

    std::unique_ptr<ListClaim> listClaim;

    QString number = "";
    // bool isFree = true;
    bool isBaseOff = false;
};

#endif // CLAIMMODEL_H
