#ifndef WORKTODLG_H
#define WORKTODLG_H

#include "repomssql.h"
#include "models/Items.h"
#include <QDialog>
#include <models/setterout.h>

namespace Ui {
class workTO;
}

class workTODlg : public QDialog
{
    Q_OBJECT

public:
    explicit workTODlg(QWidget *parent = nullptr);
    ~workTODlg();

private slots:
    void on_tbSearch_clicked();
    void on_pbExchange_clicked();
    void on_pbFirmware_clicked();
    void on_pbGarantLong_clicked();

private:
    Ui::workTO *ui;
    RepoMSSQL repo;
    Items mainDev;
    SetterOut setter;

    void SetNewGarantDate(Items *dev, QDateTime &date);

};

#endif // WORKTODLG_H
