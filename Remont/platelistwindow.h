#ifndef PLATELISTWINDOW_H
#define PLATELISTWINDOW_H

#include <QDialog>
#include <qtablewidget.h>

#include <models/listplate.h>
#include <models/platemodel.h>
// #include <models/platetype.h>

namespace Ui {
class PlateListWindow;
}

class PlateListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateListWindow(QWidget *parent = nullptr);
    ~PlateListWindow();

    Items SelectPlate(QString number = "");
    void setSelect();
    void setNotLinked();
    void RemoveListPlate(QList<Items> &listExclude);

    QList<Items> selectedPlates;

private slots:
    void on_tbSearch_clicked();
    void on_pbDelete_clicked();
    void on_rbNotLink_clicked();
    void on_rbAll_clicked();
    void on_pbSelect_clicked();
    // void on_twPlates_itemDoubleClicked(QTableWidgetItem *item);
    void slotReadScan(QString s);

    // void on_actionBroken_triggered();
    // void on_twPlates_customContextMenuRequested(const QPoint &pos);

private:
    Ui::PlateListWindow *ui;
    bool isSelectPlate = false;
    bool isNotLinked = false;
    // ListPlate listPlate;
    QVector<ItemType> listType;
    QList<Items> *listExcludePlate = nullptr;
    QMap<int, QString> listPlateType;
    Items selectPlate;
    QMetaObject::Connection conn;

    void startLoad();

    void UpdateForm();
    void LinkTypePlate();

    PlateModel *model;
};

#endif // PLATELISTWINDOW_H
