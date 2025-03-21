#ifndef CHANGEMODULDLG_H
#define CHANGEMODULDLG_H

#include <QDialog>

#include <models/Items.h>

namespace Ui {
class ChangeModulDlg;
}

class ChangeModulDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeModulDlg(Items* dev, QWidget *parent = nullptr);
    ~ChangeModulDlg();

private slots:
    void on_tbSearch_clicked();

    void on_pbOK_clicked();

private:
    Ui::ChangeModulDlg *ui;
    Items *brokenDev;
    Items newDev;
    RepoMSSQL repo;

    bool testChildGoodStatus(Items *dev);

};

#endif // CHANGEMODULDLG_H
