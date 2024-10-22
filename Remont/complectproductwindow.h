#ifndef COMPLECTPRODUCTWINDOW_H
#define COMPLECTPRODUCTWINDOW_H

#include "repomssql.h"

#include <QDialog>

#include <models/listmodul.h>
#include <models/product.h>
#include <infrastructure/trackrecord.h>

namespace Ui {
class ComplectProductWindow;
}

class ComplectProductWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ComplectProductWindow(QWidget *parent = nullptr, Product *prod = nullptr);
    ~ComplectProductWindow();

private slots:
    void on_tbSearchModul_clicked();
    void on_tbProdSearch_clicked();
    void on_pbAddModul_clicked();
    void on_pbDeleteModul_clicked();
    void on_pbOK_clicked();
    void slotReadScan(QString s);

private:
    Ui::ComplectProductWindow *ui;
    RepoMSSQL repo;
    Product prod;
    // QList<Modul> listModul;
    ListModul Modules;
    TrackRecord<Modul> trackModul;
    // QSet<Modul> addModul;
    // QSet<Modul> delModul;
    QMetaObject::Connection conn;

    void LoadProductToScreen(Product &prod);
    void addModulToScreen(Modul &mod);

};

#endif // COMPLECTPRODUCTWINDOW_H
