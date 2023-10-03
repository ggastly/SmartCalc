QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    models/qcustomplot.cc \
    main.cc \
    view/mainwindow.cc \

HEADERS += \
    view/mainwindow.h \
    models/qcustomplot.h \
    models/calculator_model.h \
    models/validator_model.h \
    models/converter_model.h \
    models/plot_model.h \
    controller/calc_controller.h


FORMS += \
    view/mainwindow.ui

win32:RC_FILE = icons/icon.rc
macx:ICON = icons/app_icon.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/icon.rc \
    icons/icon_calculator.ico
