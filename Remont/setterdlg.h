#ifndef SETTERDLG_H
#define SETTERDLG_H

#include <QDialog>
#include <QPointer>

#include <models/settermodel.h>

namespace Ui {
class SetterDlg;
}

class SetterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetterDlg(bool isSelect = false, bool _isFree = true, QWidget *parent = nullptr);
    ~SetterDlg();
    SetterOut *selectSetter = nullptr;

private slots:
    void on_tbSearch_clicked();
    void on_pbNew_clicked();
    void on_pbEdit_clicked();
    void on_pbDelete_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pbSelect_clicked();

private:
    Ui::SetterDlg *ui;
    SetterModel* model;
    bool isFree = false;
    void UpdateButtonEnabled();
};

#endif // SETTERDLG_H
