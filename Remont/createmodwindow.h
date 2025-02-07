#ifndef CREATEMODWINDOW_H
#define CREATEMODWINDOW_H

#include "repomssql.h"

#include <QDialog>
#include <QTreeWidget>

#include <models/ItemsType.h>
// #include <models/modultype.h>
// #include <models/modultype.h>
// #include <models/plate.h>
// #include <models/prodtype.h>
// #include <models/prodtype.h>

namespace Ui {
class CreateModulWindow;
}

class CreateModulWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateModulWindow(QWidget *parent = nullptr);
    ~CreateModulWindow();

private slots:
    void on_tbDeleteModul_clicked();
    void on_pbRegModul_clicked();
    void on_cbModul_currentIndexChanged(int index);
    void slotReadScan(QString s);
    void on_tbDoc_clicked();

private:
    Ui::CreateModulWindow *ui;
    RepoMSSQL repo;
    int countFromDoc = 0;
    int countUse = 0;
    QVector<ItemType> listTypeModule;

    void addLineModul(Items &mod);
    QMetaObject::Connection conn;
    void UpdateUseCount();

};

#endif // CREATEMODWINDOW_H
