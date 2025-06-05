#ifndef EDITDEVICEDLG_H
#define EDITDEVICEDLG_H

#include <QDialog>

#include <models/Items.h>

namespace Ui {
class EditDeviceDlg;
}

class EditDeviceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EditDeviceDlg(QWidget *parent = nullptr);
    ~EditDeviceDlg();

private slots:
    void on_tbSearch_clicked();
    void on_pbSaveItem_clicked();
    void on_pbAddStatus_clicked();
    void on_pbDeleteStatus_clicked();

private:
    Ui::EditDeviceDlg *ui;
    Items device;
    RepoMSSQL repo;

    void ShowStatus();
};

#endif // EDITDEVICEDLG_H
