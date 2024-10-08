QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acceptremontwindow.cpp \
    cardprodwindow.cpp \
    claimdetail.cpp \
    claimwindow.cpp \
    complectproductwindow.cpp \
    createdevicewindow.cpp \
    endremontwindow.cpp \
    listshipwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    models/listclaim.cpp \
    models/listdevice.cpp \
    models/listmodul.cpp \
    models/listplate.cpp \
    models/listproduct.cpp \
    models/listsetter.cpp \
    models/listshipment.cpp \
    otkcontrolwindow.cpp \
    platewindow.cpp \
    remontwindow.cpp \
    repomssql.cpp \
    selectdevicewindow.cpp \
    selectplatewindow.cpp \
    shipwindow.cpp \
    startworkwindow.cpp

HEADERS += \
    acceptremontwindow.h \
    cardprodwindow.h \
    claimdetail.h \
    claimwindow.h \
    complectproductwindow.h \
    createdevicewindow.h \
    endremontwindow.h \
    infrastructure/IStatus.h \
    listshipwindow.h \
    models/IDevice.h \
    models/claim.h \
    models/entity.h \
    mainwindow.h \
    models/enumvariable.h \
    models/listclaim.h \
    models/listdevice.h \
    models/listmodul.h \
    models/listplate.h \
    models/listproduct.h \
    models/listsetter.h \
    models/listshipment.h \
    models/modul.h \
    models/modultype.h \
    models/plate.h \
    models/prodtype.h \
    models/product.h \
    models/remontstep.h \
    models/setterout.h \
    models/shipment.h \
    otkcontrolwindow.h \
    platewindow.h \
    remontwindow.h \
    repomssql.h \
    selectdevicewindow.h \
    selectplatewindow.h \
    shipwindow.h \
    startworkwindow.h

FORMS += \
    acceptremontwindow.ui \
    cardprodwindow.ui \
    claimdetail.ui \
    claimwindow.ui \
    complectproductwindow.ui \
    createdevicewindow.ui \
    endremontwindow.ui \
    listshipwindow.ui \
    mainwindow.ui \
    otkcontrolwindow.ui \
    platewindow.ui \
    remontwindow.ui \
    selectdevicewindow.ui \
    selectplatewindow.ui \
    shipwindow.ui \
    startworkwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
