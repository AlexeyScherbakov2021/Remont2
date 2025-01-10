#ifndef PRODVNFTWINDOW_H
#define PRODVNFTWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <qtablewidget.h>

namespace Ui {
class ProdVNFTWindow;
}

class ProdVNFTWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProdVNFTWindow(QWidget *parent = nullptr);
    ~ProdVNFTWindow();

private slots:
    // void on_twVNFT_itemChanged(QTableWidgetItem *item);
    void on_tbAdd_clicked();
    void on_tbDelete_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();

private:
    Ui::ProdVNFTWindow *ui;
    RepoMSSQL repo;
    QSqlTableModel model2;
};

#endif // PRODVNFTWINDOW_H
