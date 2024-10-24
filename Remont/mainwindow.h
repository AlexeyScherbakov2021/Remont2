#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "repomssql.h"
#include <QKeyEvent>
#include <QMainWindow>
#include <QSqlDatabase>
#include <infrastructure/scaner.h>

class Shipmrnt;
class SetterOut;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbClaim_clicked();
    void on_pbCard_clicked();
    void on_pbApplyRemont_clicked();
    void on_pbRegister_clicked();
    void on_pbOTK_clicked();
    void on_pbCreatePlate_clicked();
    void on_pbShip_clicked();
    void on_pbComplect_clicked();
    void on_pbStartRemont_clicked();
    void on_pbStartWork_clicked();
    void on_pbEndRemont_clicked();
    void on_aRegPlate_triggered();
    void on_aRegDevice_triggered();
    void on_aInstallModul_triggered();
    void on_aControlOTK_triggered();
    void on_aShipBefore_triggered();
    void on_aStartWork_triggered();
    void on_aClaim_triggered();
    void on_aGetOSO_triggered();
    void on_aGetRepair_triggered();
    void on_aEndRepair_triggered();
    void on_aCardDevice_triggered();
    void on_aExchModul_triggered();
    void on_aScaner_triggered();
    void slotReadScan(QString s);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QMetaObject::Connection conn;

};

#endif // MAINWINDOW_H
