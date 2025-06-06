QT       += core gui sql serialport svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++20

VERSION = 1.1.0.7
QMAKE_TARGET_COMPANY = NGK
QMAKE_TARGET_PRODUCT = ReestrNumbers
QMAKE_TARGET_DESCRIPTION = Reestr Numbers
QMAKE_TARGET_COPYRIGHT = NPO NGK EHZ

# CONFIG(debug) {
#     DEFINES += TESTBASE
# }

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Itemvnftwindow.cpp \
    acceptremontwindow.cpp \
    cardprodwindow.cpp \
    changemoduldlg.cpp \
    changetypedlg.cpp \
    claimdetail.cpp \
    claimwindow.cpp \
    complectproductwindow.cpp \
    createmodwindow.cpp \
    createprodwindow.cpp \
    editdevicedlg.cpp \
    endremontwindow.cpp \
    infrastructure/IStatus.cpp \
    infrastructure/dateedit.cpp \
    infrastructure/scaner.cpp \
    infrastructure/trackrecord.cpp \
    infrastructure/users.cpp \
    listshipwindow.cpp \
    logindlg.cpp \
    logwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    models/claimmodel.cpp \
    models/devicemodel.cpp \
    models/entity.cpp \
    models/listclaim.cpp \
    models/listdevice.cpp \
    models/listentity.cpp \
    models/listmodul.cpp \
    models/listplate.cpp \
    models/listproduct.cpp \
    models/listsetter.cpp \
    models/listshipment.cpp \
    models/modelproxy.cpp \
    models/modelvnft.cpp \
    models/settermodel.cpp \
    models/shipmodel.cpp \
    otkactiondlg.cpp \
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
    searchdevdlg.cpp \
    selectdevicewindow.cpp \
    setterdlg.cpp \
    settereditdlg.cpp \
    shipwindow.cpp \
    startworkwindow.cpp \
    statusdlg.cpp \
    treeitemsform.cpp \
    usersetdlg.cpp \
    worktodlg.cpp

HEADERS += \
    Itemvnftwindow.h \
    acceptremontwindow.h \
    cardprodwindow.h \
    changemoduldlg.h \
    changetypedlg.h \
    claimdetail.h \
    claimwindow.h \
    complectproductwindow.h \
    createmodwindow.h \
    createprodwindow.h \
    editdevicedlg.h \
    endremontwindow.h \
    infrastructure/IStatus.h \
    infrastructure/dateedit.h \
    infrastructure/general.h \
    infrastructure/scaner.h \
    infrastructure/trackrecord.h \
    infrastructure/users.h \
    listshipwindow.h \
    logindlg.h \
    logwindow.h \
    models/IDevice.h \
    models/Items.h \
    models/ItemsType.h \
    models/claim.h \
    models/claimmodel.h \
    models/devicemodel.h \
    models/entity.h \
    mainwindow.h \
    models/listclaim.h \
    models/listdevice.h \
    models/listentity.h \
    models/listmodul.h \
    models/listplate.h \
    models/listproduct.h \
    models/listsetter.h \
    models/listshipment.h \
    models/modelproxy.h \
    models/modelvnft.h \
    models/organization.h \
    models/remont.h \
    models/settermodel.h \
    models/setterout.h \
    models/shipment.h \
    models/shipmodel.h \
    otkactiondlg.h \
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
    searchdevdlg.h \
    selectdevicewindow.h \
    setterdlg.h \
    settereditdlg.h \
    shipwindow.h \
    startworkwindow.h \
    statusdlg.h \
    treeitemsform.h \
    usersetdlg.h \
    worktodlg.h

FORMS += \
    Itemvnftwindow.ui \
    acceptremontwindow.ui \
    cardprodwindow.ui \
    changemoduldlg.ui \
    changetypedlg.ui \
    claimdetail.ui \
    claimwindow.ui \
    complectproductwindow.ui \
    createmodwindow.ui \
    createprodwindow.ui \
    editdevicedlg.ui \
    endremontwindow.ui \
    listshipwindow.ui \
    logindlg.ui \
    logwindow.ui \
    mainwindow.ui \
    otkactiondlg.ui \
    otkwindow.ui \
    platefwwindow.ui \
    platelistwindow.ui \
    platewindow.ui \
    qrwindow.ui \
    remontwindow.ui \
    scan.ui \
    searchdevdlg.ui \
    selectdevicewindow.ui \
    setterdlg.ui \
    settereditdlg.ui \
    shipwindow.ui \
    startworkwindow.ui \
    statusdlg.ui \
    treeitemsform.ui \
    usersetdlg.ui \
    worktodlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


DISTFILES +=
