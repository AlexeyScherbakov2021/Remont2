#ifndef ENDREMONTWINDOW_H
#define ENDREMONTWINDOW_H

#include "repomssql.h"

#include <QDialog>

// #include <models/product.h>
// #include <models/modul.h>
#include <models/Items.h>
#include <models/claim.h>

namespace Ui {
class EndRemontWindow;
}

class EndRemontWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EndRemontWindow(QWidget *parent = nullptr);
    ~EndRemontWindow();

private slots:
    void on_tbNumber_clicked();
    void on_pbEndRemont_clicked();
    void slotReadScan(QString s);

private:
    Ui::EndRemontWindow *ui;
    Items device;
    Claim claim;
    RepoMSSQL repo;
    void AddDevice(Items *item);
    void UpdateButtonEnabled();
};

#endif // ENDREMONTWINDOW_H
