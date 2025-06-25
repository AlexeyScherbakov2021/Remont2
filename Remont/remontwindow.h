#ifndef REMONTWINDOW_H
#define REMONTWINDOW_H

#include <QDialog>

// #include <models/modul.h>
#include <models/claim.h>
#include <models/remontm.h>

namespace Ui {
class RemontWindow;
}

class RemontWindow : public QDialog
{
    Q_OBJECT

public:
    // QList<RemontM> *listRemont;

    explicit RemontWindow(QWidget *parent = nullptr);
    ~RemontWindow();

private slots:
    void on_pbOK_clicked();
    void on_tbNumber_clicked();
    void slotReadScan(QString s);

private:
    Ui::RemontWindow *ui;
    // Modul *modul;
    // RemontEntity *remontEntity;
    RepoMSSQL repo;
    Items device;
    // QList<RemontStepStatus> listStatus;
    Claim claim;
    void AddDevice(Items *item);
    void UpdateButtonEnabled();

};

#endif // REMONTWINDOW_H
