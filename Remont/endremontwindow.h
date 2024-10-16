#ifndef ENDREMONTWINDOW_H
#define ENDREMONTWINDOW_H

#include "repomssql.h"

#include <QDialog>

#include <models/product.h>
#include <models/modul.h>

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

private:
    Ui::EndRemontWindow *ui;
    // int idProd = 0;
    // int idMod = 0;
    Product product;
    Modul modul;
    RepoMSSQL repo;

};

#endif // ENDREMONTWINDOW_H
