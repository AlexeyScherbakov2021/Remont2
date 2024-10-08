#ifndef LISTSHIPWINDOW_H
#define LISTSHIPWINDOW_H

#include "repomssql.h"

#include <QDialog>

#include <models/listshipment.h>

namespace Ui {
class ListShipWindow;
}

class ListShipWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ListShipWindow(QWidget *parent = nullptr);
    ~ListShipWindow();

private slots:
    void on_pbNew_clicked();
    void on_pbEdit_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_pbDeleteShip_clicked();

private:
    Ui::ListShipWindow *ui;
    RepoMSSQL repo;
    // QList<Shipment> listShip;
    ListShipment Ship;

    void addRowWidget(Shipment &ship, int row = -1);
};

#endif // LISTSHIPWINDOW_H
