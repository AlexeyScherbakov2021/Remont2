#ifndef CLAIMWINDOW_H
#define CLAIMWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <models/claim.h>
#include <models/claimmodel.h>
#include <models/devicemodel.h>
#include <models/listclaim.h>

namespace Ui {
class ClaimWindow;
}

class ClaimWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClaimWindow(QWidget *parent = nullptr);
    ~ClaimWindow();

private slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void on_pbEdit_clicked();
    // void on_twClaim_cellDoubleClicked(int row, int column);
    // void on_pbSelect_clicked();

    void on_twClaim_doubleClicked(const QModelIndex &index);

private:
    Ui::ClaimWindow *ui;
    // QList<Claim> listClaim;
    ListClaim claims;
    void AddLineToWidget(int row);

    RepoMSSQL repo;
    // bool isSelected;
    ClaimModel *model;

    int AddLineScreen(const Claim *claim);
};

#endif // CLAIMWINDOW_H
