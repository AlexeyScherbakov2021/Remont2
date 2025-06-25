#ifndef QRWINDOW_H
#define QRWINDOW_H

#include <QDialog>
#include <QPrinter>

namespace Ui {
class QRWindow;
}

class QRWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QRWindow(QWidget *parent = nullptr);
    ~QRWindow();

private slots:
    void on_pbGenerate_clicked();
    void on_pbPrint_clicked();
    void paintPages(QPrinter *printer);

    void on_cbYear_currentIndexChanged(int index);

private:
    const int widthStick = 1020;
    const int heightStick = 350;
    const int sizeQR = 300;
    Ui::QRWindow *ui;
    QStringList listNumbers;
    // void formatDocument(QTextDocument *doc);
    void paintStick(QPainter &painter, int x, int y, QString number);
    void ShowCurrentNumber();
    void UpdateButtonEnabled();
};

#endif // QRWINDOW_H
