#ifndef STARTWORKWINDOW_H
#define STARTWORKWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <models/listmodul.h>
#include <models/listproduct.h>
// #include <models/product.h>

namespace Ui {
class StartWorkWindow;
}

class StartWorkWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWorkWindow(QWidget *parent = nullptr);
    ~StartWorkWindow();

private slots:
    void on_pbProdToWork_clicked();
    void on_tbSearch_clicked();
    void on_tbDelete_clicked();
    void slotReadScan(QString s);

protected:
    Ui::StartWorkWindow *ui;
    void AddDevice(Items* dev);
    RepoMSSQL repo;
    QSet<int> listAddId;

private:
    // ListProduct products;
    // ListModul modules;
    QList<Items> listDev;
    virtual void SelectDevice();
    virtual void SetStatusAllDevice(Items* item, QDateTime& dateOn);
};


class EnterWorkWindow : public StartWorkWindow
{
    Q_OBJECT
public:
    EnterWorkWindow(QWidget *parent = nullptr);
private:
    void SelectDevice() override;
    void SetStatusAllDevice(Items* item, QDateTime& dateOn) override;

};


#endif // STARTWORKWINDOW_H
