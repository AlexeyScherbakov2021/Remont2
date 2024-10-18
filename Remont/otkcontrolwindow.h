#ifndef OTKCONTROLWINDOW_H
#define OTKCONTROLWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QQueue>

#include <models/listmodul.h>
#include <models/listproduct.h>
#include <models/modul.h>
#include <models/product.h>

namespace Ui {
class OTKControlWindow;
}

class OTKControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OTKControlWindow(QWidget *parent = nullptr);
    ~OTKControlWindow();

private slots:
    void on_pbCheck_clicked();
    void on_pbBroken_clicked();
    void on_tbDelCheck_clicked();
    void on_tbDelBroken_clicked();
    void on_pbCheckProd_clicked();
    void on_pbBrokenProd_clicked();
    void on_tbDelCheckProd_clicked();
    void on_tbDelBrokenProd_clicked();
    void on_OTKControlWindow_accepted();

    void on_rbOldDevice_toggled(bool checked);

private:
    Ui::OTKControlWindow *ui;
    RepoMSSQL repo;
    // QList<Modul> listModul;
    ListModul Modules;
    ListProduct Products;
    // QList<Product> listProduct;
    QMap<int, Status> listStatus;
    QMap<int, Status> listStatusProd;

    void loadCreatedDevice();
    void loadBrockenDevice();

    QString scanNumber;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // OTKCONTROLWINDOW_H
