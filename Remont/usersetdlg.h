#ifndef USERSETDLG_H
#define USERSETDLG_H

#include "repomssql.h"

#include <QDialog>
#include <QListWidget>

namespace Ui {
class UserSetDlg;
}

class UserSetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserSetDlg(QWidget *parent = nullptr);
    ~UserSetDlg();

private slots:
    void slotCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pbSave_clicked();
    void slotItemChanged(QListWidgetItem *item);

private:
    Ui::UserSetDlg *ui;
    RepoMSSQL repo;
    QList<Users> listUsers;
    Users *currentUser = nullptr;
    bool isSaved = true;
};

#endif // USERSETDLG_H
