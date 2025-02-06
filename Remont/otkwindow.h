#ifndef OTKWINDOW_H
#define OTKWINDOW_H

#include <QMainWindow>
#include <models/listdevice.h>

namespace Ui {
class OTKWindow;
}

class OTKWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OTKWindow(QWidget *parent = nullptr);
    explicit OTKWindow(ListDevice *dev, QWidget *parent = nullptr);
    // explicit OTKWindow(ListDevice<Modul> *dev, QWidget *parent = nullptr);

    // template <typename T>
    // explicit OTKWindow(ListDevice<Plate> *dev, QWidget *parent = nullptr);
    ~OTKWindow();

private:
    Ui::OTKWindow *ui;
};

#endif // OTKWINDOW_H
