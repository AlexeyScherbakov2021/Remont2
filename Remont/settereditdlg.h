#ifndef SETTEREDITDLG_H
#define SETTEREDITDLG_H

#include <QDialog>

#include <models/setterout.h>

namespace Ui {
class SetterEditDlg;
}

class SetterEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetterEditDlg(SetterOut* _setter, QWidget *parent = nullptr);
    ~SetterEditDlg();

private slots:
    void on_tbDelete_clicked();

    void on_tbSearch_clicked();

private:
    Ui::SetterEditDlg *ui;
    SetterOut* setter;
    RepoMSSQL repo;

    void AddLineToWidget(Items* item, int row);
};

#endif // SETTEREDITDLG_H
