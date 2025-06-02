#ifndef WORKTODLG_H
#define WORKTODLG_H

#include "repomssql.h"

#include <QDialog>

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

    void on_pbAnother_clicked();

private:
    Ui::workTO *ui;
    RepoMSSQL repo;

};

#endif // WORKTODLG_H
