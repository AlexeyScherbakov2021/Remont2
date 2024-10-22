#ifndef CREATEDEVICEWINDOW_H
#define CREATEDEVICEWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QTreeWidget>

// #include <models/modultype.h>
#include <models/modultype.h>
#include <models/plate.h>
#include <models/prodtype.h>
// #include <models/prodtype.h>

namespace Ui {
class CreateDeviceWindow;
}

class CreateDeviceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDeviceWindow(QWidget *parent = nullptr);
    ~CreateDeviceWindow();

private slots:
    void on_tbSearchPlate_clicked();
    void on_tbDeleteModul_clicked();
    void on_tbDeleteProduct_clicked();
    void on_pbRegProduct_clicked();
    void on_pbRegModul_clicked();
    void on_tbDelPlate_clicked();
    void on_cbProduct_currentIndexChanged(int index);
    void on_cbModul_currentIndexChanged(int index);
    void slotReadScan(QString s);

private:
    Ui::CreateDeviceWindow *ui;
    RepoMSSQL repo;
    QMap<int, ModulType> listTypeModule;
    QMap<int, ProductType> listTypeProduct;
    void addLinePlate(Plate *plate);
    void addLineModul(Modul &mod);
    QMetaObject::Connection conn;
    void AddPlateToScreen( Plate &plate);

    QList<Plate> listAddingPlate;
};

#endif // CREATEDEVICEWINDOW_H
