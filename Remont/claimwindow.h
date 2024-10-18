#ifndef CLAIMWINDOW_H
#define CLAIMWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <models/claim.h>
#include <models/listclaim.h>

namespace Ui {
class ClaimWindow;
}

class ClaimWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClaimWindow(QWidget *parent = nullptr/*, bool isSelected = false*/);
    ~ClaimWindow();
    // Claim selectedClaim;

private slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void on_pbEdit_clicked();
    void on_twClaim_cellDoubleClicked(int row, int column);
    // void on_pbSelect_clicked();

private:
    Ui::ClaimWindow *ui;
    // QList<Claim> listClaim;
    ListClaim claims;
    RepoMSSQL repo;
    // bool isSelected;

    int AddLineScreen(const Claim *claim);
};

#endif // CLAIMWINDOW_H
