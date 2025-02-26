#ifndef CLAIMDETAIL_H
#define CLAIMDETAIL_H

#include "repomssql.h"
#include <QDialog>
#include <QFutureWatcher>
#include <models/claim.h>
#include <infrastructure/trackrecord.h>

namespace Ui {
class ClaimDetail;
}

class ClaimDetail : public QDialog
{
    Q_OBJECT

public:
    explicit ClaimDetail(Claim *cl, QWidget *parent = nullptr);
    ~ClaimDetail();

private slots:
    void on_pbOK_clicked();
    void on_tbAddDevice_clicked();
    void on_tbDeleteDevice_clicked();
    void slotEnabledWidget();

private:
    Ui::ClaimDetail *ui;
    QFutureWatcher<void> *watcher;
    // explicit ClaimDetail(QWidget *parent = nullptr);
    Claim *claim;
    QMap<int, QString> listTypeClaim;
    // QMap<int, QString> listTypeModule;
    // QMap<int, QString> listTypeProduct;
    QList<Organization> listOrg;

    TrackRecord<Items> trackProduct;
    // TrackRecord<Items> trackModul;

    RepoMSSQL repo;

    void ClaimToScreen(/*Claim *claim*/);
    // void AddModulToTableScreen(const Items &modul);
    void AddProductToTableScreen(const Items *prod);

};

#endif // CLAIMDETAIL_H
