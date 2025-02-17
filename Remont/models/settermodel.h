#ifndef SETTERMODEL_H
#define SETTERMODEL_H

#include "listsetter.h"

#include <QAbstractTableModel>

class SetterModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SetterModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // bool setHeaderData(int section,
    //                    Qt::Orientation orientation,
    //                    const QVariant &value,
    //                    int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    // bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;


    // void prepareLoad(const QString _number, int _status, bool _isBusy, bool _isParent);
    void prepareLoad(const QString _number, bool _isBusy = false);
    SetterOut* GetItem(int row);
    bool DeleteItem(int row);
    void DeleteItemFromListId(int id) ;
    void DeleteItemFromList(int row) ;

    void createList();

    void AddItem(SetterOut* item);
    void UpdateItem(int row);
    void setBaseOff();
    Items* GetItem(QString number);

private:
    int startLoad = 0;
    int cntLoad = 60;
    bool isFetch = false;
    QStringList headers;

    std::unique_ptr<ListSetter> listSetter;

    QString number = "";
    bool isBusy = false;
    bool isBaseOff = false;
    // pLoadItems lp = nullptr;


};

#endif // SETTERMODEL_H
