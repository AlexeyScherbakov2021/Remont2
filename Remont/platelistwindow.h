#ifndef PLATELISTWINDOW_H
#define PLATELISTWINDOW_H

#include <QDialog>
#include <qtablewidget.h>

#include <models/listplate.h>
#include <models/devicemodel.h>
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
    void setSelectForm();
    void setNotLinked();
    void RemoveListPlate(QList<Items> &listExclude);
    QList<Items> selectedPlates;

private slots:
    void on_tbSearch_clicked();
    void on_pbDelete_clicked();
    void on_rbNotLink_clicked();
    void on_rbAll_clicked();
    void on_pbSelect_clicked();
    void slotReadScan(QString s);

private:
    Ui::PlateListWindow *ui;
    bool isSelectPlate = false;
    bool isNotLinked = false;
    QVector<ItemType> listType;
    QList<Items> *listExcludePlate = nullptr;
    QMap<int, QString> listPlateType;
    Items selectPlate;
    QMetaObject::Connection conn;
    void startLoad();
    void UpdateForm();
    DeviceModel *model;
};

#endif // PLATELISTWINDOW_H
