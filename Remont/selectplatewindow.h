#ifndef SELECTPLATEWINDOW_H
#define SELECTPLATEWINDOW_H

#include <QDialog>

#include <models/plate.h>

namespace Ui {
class SelectPlateWindow;
}

class SelectPlateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPlateWindow(QList<Plate> &listPlate, QWidget *parent = nullptr);
    ~SelectPlateWindow();

    int selectedIndex;

private slots:
    void on_SelectPlateWindow_accepted();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::SelectPlateWindow *ui;
};

#endif // SELECTPLATEWINDOW_H
