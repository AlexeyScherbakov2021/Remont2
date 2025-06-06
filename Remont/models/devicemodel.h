#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "listplate.h"
#include "modelproxy.h"
// #include <QDebug>
#include <QAbstractTableModel>
#include <infrastructure/IStatus.h>


class DeviceModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DeviceModel(IndexType type, QObject *parent = nullptr);
    ~DeviceModel();

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

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    void prepareLoad(const QString _number, int _status, bool _isBusy, LoadPartType _hasParent);
    void prepareLoad2(const QString _number, QVector<StatusItem>& _status, bool _isBusy, LoadPartType _hasParent);
    Items* GetItem(int row);
    Items* GetItem(QModelIndex *index);
    bool DeleteItem(int row);
    void DeleteItemFromListId(int id);
    void DeleteItemFromList(int row);

    void prepareSearch(optionSearch &opt);
    bool isSearch = false;

    void createList(IndexType);

    void AddItem(Items* item);
    void UpdateItem(int row);
    void setBaseOff();
    Items* GetItem(QString number);

private:
    int startLoad = 0;
    int cntLoad = 60;
    bool isFetch = false;
    QStringList headers;

    std::unique_ptr<ListDevice> listDev;

    QString number = "";
    int status = 0;
    QVector<StatusItem> vStatus;
    bool isBusy = false;
    LoadPartType hasParent = LoadPartType::NO_HAS_PARENT;
    bool isBaseOff = false;

    optionSearch opt;

};



#endif // DEVICEMODEL_H
