#ifndef OTKWINDOW_H
#define OTKWINDOW_H

#include <QMainWindow>
#include <models/listdevice.h>
#include <models/devicemodel.h>

namespace Ui {
class OTKWindow;
}

class OTKWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OTKWindow(QWidget *parent = nullptr);
    ~OTKWindow();

private slots:
    void on_toolButton_clicked();
    void on_pbSelect_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tbDelCheckProd_clicked();
    void on_tbDelBrokenProd_clicked();
    void slotReadScan(QString s);

private:
    Ui::OTKWindow *ui;

    DeviceModel *model = nullptr;
    QVector<StatusItem> stat = { StatusItem::CREATE, StatusItem::FAULTY};

    void RestartLoad();
    void StartControlWindow(Items* item);

};

#endif // OTKWINDOW_H
