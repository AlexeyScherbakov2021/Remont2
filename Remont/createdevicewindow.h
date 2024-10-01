#ifndef CREATEDEVICEWINDOW_H
#define CREATEDEVICEWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QTreeWidget>

#include <models/plate.h>

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

private:
    Ui::CreateDeviceWindow *ui;
    RepoMSSQL repo;
    QMap<int, QString> listTypeModule;
    QMap<int, QString> listTypeProduct;
    // int selectIdPlate;
    void addLinePlate(Plate *plate);
    void addLineModul(Modul &mod);

    QList<Plate> listAddingPlate;
};

#endif // CREATEDEVICEWINDOW_H
