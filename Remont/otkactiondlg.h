#ifndef OTKACTIONDLG_H
#define OTKACTIONDLG_H

#include <QDialog>
#include <QTreeWidget>

#include <models/Items.h>

namespace Ui {
class OtkActionDlg;
}

class OtkActionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OtkActionDlg(Items* item, QWidget *parent = nullptr);
    ~OtkActionDlg();

private slots:
    void on_pbOk_clicked();

private:
    Ui::OtkActionDlg *ui;
    RepoMSSQL repo;
    Items *dev;

    void GetTreeChild(Items* item);
    void AddTreeWidget(Items *dev, QTreeWidgetItem* item);
};

#endif // OTKACTIONDLG_H
