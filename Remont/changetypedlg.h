#ifndef CHANGETYPEDLG_H
#define CHANGETYPEDLG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>

#include <models/Items.h>

namespace Ui {
class ChangeTypeDlg;
}

class ChangeTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTypeDlg(Items *item, QWidget *parent = nullptr);
    ~ChangeTypeDlg();

private slots:
    void slotSelect();
    void on_tbSearch_clicked();

private:
    Ui::ChangeTypeDlg *ui;
    Items *dev;
    QSqlQueryModel *model;
    QSortFilterProxyModel *proxy;
};

#endif // CHANGETYPEDLG_H
