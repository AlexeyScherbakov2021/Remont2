#ifndef PLATELISTWINDOW_H
#define PLATELISTWINDOW_H

#include <QDialog>
#include <qtablewidget.h>

#include <models/listplate.h>
#include <models/platetype.h>

namespace Ui {
class PlateListWindow;
}

class PlateListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateListWindow(QWidget *parent = nullptr);
    ~PlateListWindow();

    Plate SelectPlate(QString number = "");
    void setSelect();
    void setNotLinked();
    void RemoveListPlate(QList<Plate> &listExclude);

    QList<Plate> selectedPlates;

private slots:
    void on_tbSearch_clicked();
    void on_pbDelete_clicked();
    void on_rbNotLink_clicked();
    void on_rbAll_clicked();
    void on_pbSelect_clicked();
    void on_twPlates_itemDoubleClicked(QTableWidgetItem *item);
    void slotReadScan(QString s);

    void on_actionBroken_triggered();

    void on_twPlates_customContextMenuRequested(const QPoint &pos);

private:
    Ui::PlateListWindow *ui;
    bool isSelectPlate = false;
    bool isNotLinked = false;
    ListPlate listPlate;
    QVector<PlateType> listType;
    QList<Plate> *listExcludePlate = nullptr;
    QMap<int, QString> listPlateType;
    Plate selectPlate;
    QMetaObject::Connection conn;


    void UpdateForm();
    void LinkTypePlate();

};

#endif // PLATELISTWINDOW_H
