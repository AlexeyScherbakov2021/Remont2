#ifndef SEARCHDEVDLG_H
#define SEARCHDEVDLG_H

#include <QDialog>

#include <models/devicemodel.h>

namespace Ui {
class SearchDevDlg;
}

class SearchDevDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDevDlg(QWidget *parent = nullptr);
    ~SearchDevDlg();

private slots:
    void on_pbSelect_clicked();
    void on_pbSearch_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SearchDevDlg *ui;
    DeviceModel *model = nullptr;
};

#endif // SEARCHDEVDLG_H
