#include "mainwindow.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
// #include <QStyleFactory>
// #include <QTranslator>
// #include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // qApp->setStyle(QStyleFactory::create("Fusion"));
    // QApplication::setStyle(QStyleFactory::create("QWindowsStyle"));

    // QApplication::setStyle("QFusionStyle");
    // a.setStyleSheet("QGroupBox {"
    //                             "border: 1px solid #9F9F9F; "
    //                             "font-size: 14px; "
    //                             "font-weight: 800;"
    //                             "color: black;"
    //                             "border-radius: 4px;"
    //                             "margin-top: 2.5ex; "
    //                             "padding-top: 2ex;"
    //                             "QGroupBox::title { "
    //                                 "subcontrol-origin: margin;"
    //                                 "subcontrol-position: top center;}"
    //                             "QPushButton {"
    //                                 "border-radius: 3px;border: 1px solid #9F9F9F;"
    //                                 "padding-left: 8px;"
    //                                 "padding-right: 8px;"
    //                                 "padding-top: 4px;"
    //                                 "padding-bottom: 4px;}"
    //                             "QPushButton:hover {"
    //                                 "background-color: white;}"
    //                             "QPushButton:pressed {"
    //                                 "background-color: LightGray;"
    //                                 "position: relative;"
    //                                 "top: 4px;"
    //                                 "left: 4px; }");

    QTranslator qtTranslator;
    QString locale = QLocale::system().name();
    bool res = qtTranslator.load("qt_" + locale, QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    if(res)
        a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();
    return a.exec();
}
