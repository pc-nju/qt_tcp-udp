QT       += network

include(./customtcp/customtcp.pri)

include(./customudp/customudp.pri)

include(./utils/utils.pri)

HEADERS += \
    $$PWD/netapifactory.h \
    $$PWD/netapinamespace.h

SOURCES += \
    $$PWD/netapifactory.cpp
