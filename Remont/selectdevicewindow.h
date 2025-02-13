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
#include <models/devicemodel.h>

namespace Ui {
class SelectDeviceWindow;
}

class SelectDeviceWindow : public QDialog
{
    Q_OBJECT

public:

    SelectDeviceWindow(ItemType::IndexType _type, QWidget *parent = nullptr);
    Items* SelectDevice(bool isNow, QVector<int>& statusList, QString searchNum = "",  bool _isBusy = false, bool isParent = false);
    void setDisableSearch();

    ~SelectDeviceWindow();
    Items *device = nullptr;
    Items device2;

    void AddSelectedType(ItemType::IndexType _type);

private slots:
    void on_tbSearch_clicked();
    void on_pbSelect_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void slotTypeChanged(int index);
    // void slotReadScan(QString s);

private:
    Ui::SelectDeviceWindow *ui;
    RepoMSSQL repo;
    QStandardItemModel typeModel;

    void Search(QString searchNum);

    DeviceModel *model = nullptr;
    void startLoad();

    ItemType::IndexType type;
    QVector<int> vStatus;
    bool isBusy = false;
    bool isParent = false;

};


#endif // SELECTDEVICEWINDOW_H
