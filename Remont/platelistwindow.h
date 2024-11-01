#ifndef PLATELISTWINDOW_H
#define PLATELISTWINDOW_H

#include <QDialog>

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

private slots:
    void on_tbSearch_clicked();

    void on_pbDelete_clicked();

private:
    Ui::PlateListWindow *ui;
    ListPlate listPlate;
    QVector<PlateType> listType;
    QMap<int, QString> listPlateType;

    void UpdateForm();
    void LinkTypePlate();
};

#endif // PLATELISTWINDOW_H
