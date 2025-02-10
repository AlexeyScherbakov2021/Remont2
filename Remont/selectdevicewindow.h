#ifndef SELECTDEVICEWINDOW_H
#define SELECTDEVICEWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringListModel>
#include "repomssql.h"
#include <models/listmodul.h>
#include <models/listproduct.h>
#include <infrastructure/IStatus.h>
#include <models/shipment.h>
#include <models/setterout.h>
#include <models/platemodel.h>

namespace Ui {
class SelectDeviceWindow;
}

class SelectDeviceWindow : public QDialog
{
    Q_OBJECT

public:
    // enum TypeDevice {TypeAll, TypeProduct, TypeModul};

    SelectDeviceWindow(ItemType::IndexType _type, QWidget *parent = nullptr);
    SelectDeviceWindow(ItemType::IndexType _type, QVector<int>& _status, bool _isBusy = false, bool isParent = false, QWidget *parent = nullptr);

    // Items* SelectDevice(bool isNow, QString searchNum = "", QVector<Status::Stat> statusList = QVector<Status::Stat>());
    // Items* SelectDevice(bool isNow, QString searchNum = "", Status::Stat status = Status::Stat::NONE);
    Items* SelectDevice(bool isNow, /*ItemType::IndexType _type, */QVector<int>& statusList, QString searchNum = "",  bool _isBusy = false, bool isParent = false);
    // void setTypeSearch(TypeDevice type);
    // void setParamSearch(ItemType::IndexType _type, QVector<int>& _status, bool _isBusy = false, bool isParent = false);
    // void setNotShipped();
    void setDisableSearch();

    ~SelectDeviceWindow();
    Items *device = nullptr;
    Items device2;

    void AddSelectedType(ItemType::IndexType _type);

private slots:
    void on_tbSearch_clicked();
    void on_pbSelect_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_cbType_currentIndexChanged(int index);

private:
    Ui::SelectDeviceWindow *ui;
    // ListProduct listProduct;
    // ListModul listModul;
    RepoMSSQL repo;
    QStandardItemModel typeModel;
    // Status::Stat status;
    // QVector<Status::Stat> listStatus;
    // TypeDevice typeDevice;
    // bool isNotShipped = false;

    // int SearchModul(QString number, int status);
    // int SearchProduct(QString number, int status);
    void Search(QString searchNum);
    // void listToScreen();

    PlateModel *model = nullptr;
    void startLoad();

    ItemType::IndexType type;
    QVector<int> vStatus;
    bool isBusy = false;
    bool isParent = false;

protected:
    // void timerEvent(QTimerEvent *event) override;
};


#endif // SELECTDEVICEWINDOW_H
