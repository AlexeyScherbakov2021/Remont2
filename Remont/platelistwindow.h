#ifndef PLATELISTWINDOW_H
#define PLATELISTWINDOW_H

#include <QDialog>

#include <models/listplate.h>

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

private:
    Ui::PlateListWindow *ui;
    ListPlate listPlate;

    void UpdateForm();
};

#endif // PLATELISTWINDOW_H
