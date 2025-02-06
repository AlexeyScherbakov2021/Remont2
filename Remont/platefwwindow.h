#ifndef PLATEFWWINDOW_H
#define PLATEFWWINDOW_H

#include <QDialog>

#include <models/listplate.h>
#include <models/platemodel.h>

namespace Ui {
class PlateFWWindow;
}

class PlateFWWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlateFWWindow(QWidget *parent = nullptr);
    ~PlateFWWindow();

    void LoadItems(QList<Items>& items);


private slots:
    void on_tbSearch_clicked();
    void on_pbApply_clicked();
    void on_pbOK_clicked();
    void on_tbDelete_clicked();
    void slotReadScan(QString s);

private:
    Ui::PlateFWWindow *ui;
    ListPlate listPlate;
    QMetaObject::Connection conn;
    PlateModel *model;


};

#endif // PLATEFWWINDOW_H
