#include "qrwindow.h"
#include "repomssql.h"
#include "ui_qrwindow.h"

#include <qr/QrCodeGenerator.h>

#include <QDate>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QScreen>

#include <qr/qrcodegen/qrcodegen.h>

QRWindow::QRWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QRWindow)
{
    ui->setupUi(this);
    ui->cbYear->setCurrentIndex(QDate::currentDate().year() - 2024);
    ShowCurrentNumber();
    UpdateButtonEnabled();
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
QRWindow::~QRWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::on_pbGenerate_clicked()
{
    listNumbers.clear();
    // QrCodeGenerator gen;

    uint year = ui->cbYear->currentText().toUInt();
    RepoMSSQL repo;
    repo.CreateGenerator(year);

    for(int i = 0; i < ui->sbCount->value(); ++i)
    {
        uint num = repo.GetNextNumber(year);
        listNumbers.push_back(QString("%1%2").arg(year - 2000).arg(num, 5, 10, QChar('0')));
    }

    ui->lbResult->setText(QString("с %1 по %2").arg(listNumbers.first()).arg(listNumbers.last()));
    // QString sNum = QString("%1%2").arg(year - 2000).arg(num, 5, 10, QChar('0'));
    // QImage image = gen.generateQr(ui->leText->text(), 500, 1);
    // ui->lbImage->setPixmap(QPixmap::fromImage(image));
    ShowCurrentNumber();
    UpdateButtonEnabled();
}


//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::on_pbPrint_clicked()
{
    if(listNumbers.empty())
        return;

    QPrinter printer(QPrinter::HighResolution);
    QMargins margins(10,10,10,10);
    printer.setPageMargins(margins);
    printer.setPageSize(QPageSize::A4);

    QPrintPreviewDialog *dlg = new QPrintPreviewDialog(&printer, this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    QScreen *screen = QApplication::screens().at(0);
    QSize size = screen->availableGeometry().size();
    size.rwidth() /= 2;

    dlg->resize(size);
    auto conn = connect(dlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(paintPages(QPrinter*)));
    dlg->exec();
    disconnect(conn);

}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::paintPages(QPrinter *printer)
{
    QTextDocument doc;
    doc.setPageSize(printer->pageRect(QPrinter::Unit::Point).size());
    printer->setDocName("Sticker");

    QPainter painter(printer);
    painter.begin(printer);

    // auto pg = printer->pageRect(QPrinter::Millimeter);
    // painter.setWindow(pg.toRect());

    // int cntStick = 20;
    QRect r(painter.viewport());
    // painter.setBrush(Qt::white);
    // painter.drawRect(r);

    int x = 0;
    int y = 0;
    for(auto &it : listNumbers)
    {
        if(y + heightStick >= r.height())
        {
            printer->newPage();
            y = 0;
            x = 0;
        }

        paintStick(painter, x, y, it);
        x += widthStick;
        if((x + widthStick) >= r.width())
        {
            x = 0;
            y += heightStick;
        }
    }
    painter.end();
}



// void QRWindow::formatDocument(QTextDocument *doc)
// {
//     QString s = "TEST";


//     doc->setPlainText(s);

//     QString mainHtml("<html style='font-size:9pt;font-family:times'><body>");
//     // mainHtml += QString("<h3>Бланк Контролера</h3>");
//     // mainHtml += QString("<p style='font-size:9pt;font-family:times'>Дата: %1        Время: %2</p>")
//     //                 .arg(2)
//     //                 .arg(3);

//     // mainHtml += QString("<p style='margin-bottom: 10px'><div>ФИО конкурсанта: %1</div>")
//     //                 .arg("0000000");

//     // mainHtml += QString("<div>Наименование ДО: %1</div>").arg("qqq");

//     // mainHtml += QString("<div>Билет №: %1</div></p>").arg(1);

//     mainHtml += "<div style='background-color: beige; vertical-align: middle; width: 200px; height: 60px; border: 1px solid black'>";

//     mainHtml += "<img src='image/testscan.jpg' width='50px' height='50px'>";
//     mainHtml += "№ 1710251547";

//     mainHtml += "</div>";

//     // mainHtml += "<div style='margin-top:10px;'>";
//     // mainHtml += "</div>";

//     // mainHtml += "<p style='margin-top:10px;'>";
//     // mainHtml += "</p>";

//     // mainHtml += "<p style='margin-top:10px;'>";
//     // mainHtml += "</p>";

//     // mainHtml += "<p style='margin-top:10px;'>";
//     // mainHtml += "</p>";

//     // mainHtml += QString("<br><div>ФИО/Подпись контролера _________________________________/____________</div>");
//     // mainHtml += QString("<br><div>Подпись конкурсанта       ___________________________________</div>");

//     mainHtml += "</body></html>";

//     doc->setHtml(mainHtml);

// }

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::paintStick(QPainter &painter, int x, int y, QString number)
{
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(x, y, widthStick, heightStick);
    QrCodeGenerator gen;
    QImage image = gen.generateQr(number, sizeQR, 0, qrcodegen::QrCode::Ecc::HIGH);
    painter.drawImage(x + 20 , y + 20, image);

    // painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    QRect r(x + sizeQR + 40, y + 20, widthStick - sizeQR - 40, sizeQR );
    painter.drawText(r, Qt::AlignCenter, QString("№ %1").arg(number));
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::ShowCurrentNumber()
{
    RepoMSSQL repo;
    uint year = ui->cbYear->currentText().toUInt();
    int currNumber = repo.GetCurrentNumber(year);
    ui->lbCurrentNum->setText(QString::number(currNumber));

}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void QRWindow::UpdateButtonEnabled()
{
    ui->pbPrint->setEnabled(!ui->lbResult->text().isEmpty());
}




void QRWindow::on_cbYear_currentIndexChanged(int /*index*/)
{
    ShowCurrentNumber();
}

