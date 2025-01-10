#ifndef MODVNFTWINDOW_H
#define MODVNFTWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <qtablewidget.h>

namespace Ui {
class ModVNFTWindow;
}

class ModVNFTWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ModVNFTWindow(QWidget *parent = nullptr);
    ~ModVNFTWindow();

private slots:
    // void on_twVNFT_itemChanged(QTableWidgetItem *item);
    void on_tbAdd_clicked();
    void on_tbDelete_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();

private:
    Ui::ModVNFTWindow *ui;
    RepoMSSQL repo;
    QSqlTableModel model2;
};

#endif // MODVNFTWINDOW_H
