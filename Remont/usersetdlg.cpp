#include "usersetdlg.h"
#include "ui_usersetdlg.h"

#include <infrastructure/users.h>

UserSetDlg::UserSetDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserSetDlg)
{
    ui->setupUi(this);
    QMap<RolesType, QString> listRole;

    repo.LoadRoles(listRole);

    int row = 0;
    QMapIterator<RolesType, QString> iter(listRole);
    while(iter.hasNext())
    {
        iter.next();
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, (int)iter.key());
        item->setText(iter.value());
        item->setCheckState(Qt::CheckState::Unchecked);
        ui->lwRoles->addItem(item);
        // mapRole[iter.key()] = row;
        // ++row;
    }

    repo.LoadUsers(listUsers);
    ui->twUsers->setRowCount(listUsers.size());
    // vecTrackRoles.fill(TrackRecord<RolesType>(), listUsers.size());

    row = 0;
    for(auto &it : listUsers)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(it.UserName);
        ui->twUsers->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(it.UserFullName);
        ui->twUsers->setItem(row, 1, item);

        repo.LoadRolesUser(it.id, it.listRoles);
        ++row;
    }

    connect(ui->twUsers, SIGNAL(currentCellChanged(int,int,int,int)), SLOT(slotCurrentCellChanged(int,int,int,int)));
    connect(ui->lwRoles, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(slotItemChanged(QListWidgetItem*)));
}

//-----------------------------------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------------------------------
UserSetDlg::~UserSetDlg()
{
    delete ui;
}


//-----------------------------------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------------------------------
void UserSetDlg::slotCurrentCellChanged(int currentRow, int /*currentColumn*/, int previousRow, int /*previousColumn*/)
{
    if(currentRow == previousRow || currentRow < 0)
        return;

    currentUser = &listUsers[currentRow];

    for (int row = 0; row < ui->lwRoles->count(); ++row)
    {
        QListWidgetItem *item =  ui->lwRoles->item(row);
        RolesType id = (RolesType)item->data(Qt::UserRole).toInt();
        Qt::CheckState state = currentUser->listRoles.contains(id) ? Qt::Checked : Qt::Unchecked;
        item->setCheckState(state);
    }
}


//-----------------------------------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------------------------------
void UserSetDlg::on_pbSave_clicked()
{
    if(!isSaved)
    {
        isSaved = true;

        for(auto &it : listUsers)
            repo.SaveUserRoles(it);
    }
}


//-----------------------------------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------------------------------
void UserSetDlg::slotItemChanged(QListWidgetItem *item)
{
    // int row = ui->twUsers->currentRow();
    RolesType type = (RolesType)item->data(Qt::UserRole).toInt();
    if(item->checkState() == Qt::Checked)
        currentUser->listRoles.insert(type);
    else
        currentUser->listRoles.remove(type);

    isSaved = false;
}

