#ifndef STARTWORKWINDOW_H
#define STARTWORKWINDOW_H

#include "repomssql.h"
#include <QDialog>
#include <models/listmodul.h>
#include <models/listproduct.h>
#include <models/product.h>

namespace Ui {
class StartWorkWindow;
}

class StartWorkWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWorkWindow(QWidget *parent = nullptr);
    ~StartWorkWindow();

private slots:
    void on_pbProdToWork_clicked();

private:
    Ui::StartWorkWindow *ui;
    RepoMSSQL repo;
    ListProduct products;
    ListModul modules;
};

#endif // STARTWORKWINDOW_H
