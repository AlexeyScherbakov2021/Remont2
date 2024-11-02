#ifndef PLATEVNFTWINDOW_H
#define PLATEVNFTWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <qtablewidget.h>

namespace Ui {
class PlateVNFTWindow;
}

class PlateVNFTWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateVNFTWindow(QWidget *parent = nullptr);
    ~PlateVNFTWindow();

private slots:
    // void on_twVNFT_itemChanged(QTableWidgetItem *item);
    void on_tbAdd_clicked();
    void on_tbDelete_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();

private:
    Ui::PlateVNFTWindow *ui;
    RepoMSSQL repo;
    QSqlTableModel model2;
};

#endif // PLATEVNFTWINDOW_H
