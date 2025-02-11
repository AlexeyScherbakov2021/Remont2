#ifndef PLATEWINDOW_H
#define PLATEWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <qsqlquerymodel.h>

#include <models/listplate.h>
// #include <models/plate.h>

namespace Ui {
class PlateWindow;
}

class PlateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateWindow(QWidget *parent = nullptr);
    ~PlateWindow();

private slots:
    void on_pbAdd_clicked();
    void on_tbDelete_clicked();
    void on_leNumber_textChanged(const QString &arg1);
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::PlateWindow *ui;
    RepoMSSQL repo;
    QMetaObject::Connection conn;
    // QSqlQueryModel model;
    QVector<ItemType> listVNFT;
    int countFromDoc = 0;
    int countUse = 0;
    void UpdateUseCount();

private slots:
    void slotReadScan(QString s);
    void on_tbDoc_clicked();
};

#endif // PLATEWINDOW_H
