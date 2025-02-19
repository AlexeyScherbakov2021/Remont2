#ifndef LISTSHIPWINDOW_H
#define LISTSHIPWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <models/listshipment.h>
#include <models/shipmodel.h>

namespace Ui {
class ListShipWindow;
}

class ListShipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ListShipWindow(QWidget *parent = nullptr, bool isFree = true);
    ~ListShipWindow();

private slots:
    void on_pbNew_clicked();
    void on_pbEdit_clicked();
    // void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_pbDeleteShip_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tbSearch_clicked();

private:
    Ui::ListShipWindow *ui;
    RepoMSSQL repo;
    ListShipment Ship;

    // void addRowWidget(Shipment &ship, int row = -1);
    // void setRowWidget(Shipment &ship, int row);

    ShipModel *model;
};

#endif // LISTSHIPWINDOW_H
