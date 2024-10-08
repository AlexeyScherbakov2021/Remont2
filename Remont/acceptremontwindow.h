#ifndef ACCEPTREMONTWINDOW_H
#define ACCEPTREMONTWINDOW_H

#include "repomssql.h"
#include "models/claim.h"
#include <QDialog>
#include <models/listclaim.h>

namespace Ui {
class AcceptRemontWindow;
}

class AcceptRemontWindow : public QDialog
{
    enum TypeDevice { ProductType, ModulType};
    Q_OBJECT

public:
    explicit AcceptRemontWindow(QWidget *parent = nullptr);
    ~AcceptRemontWindow();

private slots:
    void on_pbExchange_clicked();
    void on_pbApply_clicked();
    void on_tbNumber_clicked();

private:
    Ui::AcceptRemontWindow *ui;
    RepoMSSQL repo;
    // QList<Claim> listClaim;
    ListClaim claims;
    int idProd = 0;
    int idMod = 0;
    int idParentProd = 0;

};

#endif // ACCEPTREMONTWINDOW_H
