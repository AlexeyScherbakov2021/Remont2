#ifndef PLATEMODEL_H
#define PLATEMODEL_H

#include "listplate.h"
// #include <QDebug>
#include <QAbstractTableModel>
#include <infrastructure/IStatus.h>

class PlateModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PlateModel(ItemType::IndexType type, QObject *parent = nullptr);
    ~PlateModel();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    void prepareLoad(const QString _number, int _status, bool _isBusy, bool _isParent);
    void prepareLoad2(const QString _number, QVector<int>& _status, bool _isBusy, bool _isParent);
    Items* GetItem(int row);
    bool DeleteItem(int row);

    void createList(ItemType::IndexType);

    // typedef void (*pLoadItems)(QList<Items>& items);
    // void setFunction(pLoadItems p);

    void AddItem(Items* item);
    void UpdateItem(int row);
    void setBaseOff();

private:
    int startLoad = 0;
    int cntLoad = 80;
    bool isFetch = false;
    QStringList headers;

    std::unique_ptr<ListDevice> listDev;

    QString number = "";
    int status = 0;
    QVector<int> vStatus;
    bool isBusy = false;
    bool isParent = false;

    bool isBaseOff = false;
    // pLoadItems lp = nullptr;

};



#endif // PLATEMODEL_H
