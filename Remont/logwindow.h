#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>

namespace Ui {
class LogWindow;
}

class LogWindow : public QDialog
{
    Q_OBJECT
private:
    inline static LogWindow* ptr = nullptr;

public:
    ~LogWindow();

    static LogWindow* getInstance();
    static void AddLine(QString s);
    static void close();

private slots:
    void on_pbClear_clicked();

private:
    explicit LogWindow(QWidget *parent = nullptr);
    Ui::LogWindow *ui;
};

#endif // LOGWINDOW_H
