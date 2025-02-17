#ifndef SETTERDLG_H
#define SETTERDLG_H

#include <QDialog>

#include <models/settermodel.h>

namespace Ui {
class SetterDlg;
}

class SetterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetterDlg(QWidget *parent = nullptr);
    ~SetterDlg();

private slots:
    void on_tbSearch_clicked();

    void on_pbNew_clicked();

    void on_pbEdit_clicked();

    void on_pbDelete_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SetterDlg *ui;

    SetterModel *model;
};

#endif // SETTERDLG_H
