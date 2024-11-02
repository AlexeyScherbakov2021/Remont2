#ifndef PLATEFWWINDOW_H
#define PLATEFWWINDOW_H

#include <QDialog>

#include <models/listplate.h>

namespace Ui {
class PlateFWWindow;
}

class PlateFWWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateFWWindow(QWidget *parent = nullptr);
    ~PlateFWWindow();

private slots:
    void on_tbSearch_clicked();

    void on_pbApply_clicked();

    void on_pbOK_clicked();

    void on_tbDelete_clicked();

private:
    Ui::PlateFWWindow *ui;
    ListPlate listPlate;
};

#endif // PLATEFWWINDOW_H
