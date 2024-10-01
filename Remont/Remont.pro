QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardprodwindow.cpp \
    complectproductwindow.cpp \
    createdevicewindow.cpp \
    listshipwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    models/listdevice.cpp \
    models/listmodul.cpp \
    models/listplate.cpp \
    models/listproduct.cpp \
    models/listsetter.cpp \
    models/listshipment.cpp \
    otkcontrolwindow.cpp \
    platewindow.cpp \
    repomssql.cpp \
    selectdevicewindow.cpp \
    selectplatewindow.cpp \
    shipwindow.cpp

HEADERS += \
    cardprodwindow.h \
    complectproductwindow.h \
    createdevicewindow.h \
    infrastructure/IStatus.h \
    listshipwindow.h \
    models/IDevice.h \
    models/claim.h \
    models/entity.h \
    mainwindow.h \
    models/listdevice.h \
    models/listmodul.h \
    models/listplate.h \
    models/listproduct.h \
    models/listsetter.h \
    models/listshipment.h \
    models/modul.h \
    models/plate.h \
    models/product.h \
    models/remontstep.h \
    models/setterout.h \
    models/shipment.h \
    otkcontrolwindow.h \
    platewindow.h \
    repomssql.h \
    selectdevicewindow.h \
    selectplatewindow.h \
    shipwindow.h

FORMS += \
    cardprodwindow.ui \
    complectproductwindow.ui \
    createdevicewindow.ui \
    listshipwindow.ui \
    mainwindow.ui \
    otkcontrolwindow.ui \
    platewindow.ui \
    selectdevicewindow.ui \
    selectplatewindow.ui \
    shipwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
