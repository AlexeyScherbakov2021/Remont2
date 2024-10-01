QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardprodwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    models/listdevice.cpp \
    models/listmodul.cpp \
    models/listplate.cpp \
    models/listproduct.cpp \
    models/listsetter.cpp \
    models/listshipment.cpp \
    repomssql.cpp \
    selectdevicewindow.cpp

HEADERS += \
    cardprodwindow.h \
    infrastructure/IStatus.h \
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
    repomssql.h \
    selectdevicewindow.h

FORMS += \
    cardprodwindow.ui \
    mainwindow.ui \
    selectdevicewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
