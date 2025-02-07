QT       += core gui sql serialport svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Itemvnftwindow.cpp \
    acceptremontwindow.cpp \
    cardprodwindow.cpp \
    claimdetail.cpp \
    claimwindow.cpp \
    complectproductwindow.cpp \
    createmodwindow.cpp \
    createprodwindow.cpp \
    endremontwindow.cpp \
    infrastructure/IStatus.cpp \
    infrastructure/scaner.cpp \
    infrastructure/trackrecord.cpp \
    listshipwindow.cpp \
    logwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    models/listclaim.cpp \
    models/listdevice.cpp \
    models/listmodul.cpp \
    models/listplate.cpp \
    models/listproduct.cpp \
    models/listsetter.cpp \
    models/listshipment.cpp \
    models/platemodel.cpp \
    otkcontrolwindow.cpp \
    otkwindow.cpp \
    platefwwindow.cpp \
    platelistwindow.cpp \
    platewindow.cpp \
    qr/QrCodeGenerator.cpp \
    qr/qrcodegen/qrcodegen.cpp \
    qrwindow.cpp \
    remontwindow.cpp \
    repofp.cpp \
    repomssql.cpp \
    scan.cpp \
    selectdevicewindow.cpp \
    shipwindow.cpp \
    startworkwindow.cpp

HEADERS += \
    Itemvnftwindow.h \
    acceptremontwindow.h \
    cardprodwindow.h \
    claimdetail.h \
    claimwindow.h \
    complectproductwindow.h \
    createmodwindow.h \
    createprodwindow.h \
    endremontwindow.h \
    infrastructure/IStatus.h \
    infrastructure/scaner.h \
    infrastructure/trackrecord.h \
    listshipwindow.h \
    logwindow.h \
    models/IDevice.h \
    models/Items.h \
    models/ItemsType.h \
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
    models/organization.h \
    models/platemodel.h \
    models/remont.h \
    models/setterout.h \
    models/shipment.h \
    otkcontrolwindow.h \
    otkwindow.h \
    platefwwindow.h \
    platelistwindow.h \
    platewindow.h \
    qr/QrCodeGenerator.h \
    qr/qrcodegen/qrcodegen.h \
    qrwindow.h \
    remontwindow.h \
    repofp.h \
    repomssql.h \
    scan.h \
    selectdevicewindow.h \
    shipwindow.h \
    startworkwindow.h

FORMS += \
    Itemvnftwindow.ui \
    acceptremontwindow.ui \
    cardprodwindow.ui \
    claimdetail.ui \
    claimwindow.ui \
    complectproductwindow.ui \
    createmodwindow.ui \
    createprodwindow.ui \
    endremontwindow.ui \
    listshipwindow.ui \
    logwindow.ui \
    mainwindow.ui \
    otkcontrolwindow.ui \
    otkwindow.ui \
    platefwwindow.ui \
    platelistwindow.ui \
    platewindow.ui \
    qrwindow.ui \
    remontwindow.ui \
    scan.ui \
    selectdevicewindow.ui \
    shipwindow.ui \
    startworkwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

VERSION = 1.0.0.5
QMAKE_TARGET_COMPANY = NGK
QMAKE_TARGET_PRODUCT = Remont
QMAKE_TARGET_DESCRIPTION = Programm Remont
QMAKE_TARGET_COPYRIGHT = NPO NGK EHZ

DISTFILES +=
