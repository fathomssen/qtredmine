TEMPLATE = lib
TARGET = qtredmine

DEFINES += QTREDMINE_LIBRARY

QT += network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/include/qtredmine

HEADERS += \
    include/qtredmine/qtredmine_global.h \
    include/qtredmine/Authenticator.h \
    include/qtredmine/KeyAuthenticator.h \
    include/qtredmine/Logging.h \
    include/qtredmine/PasswordAuthenticator.h \
    include/qtredmine/RedmineClient.h \
    include/qtredmine/SimpleRedmineClient.h \
    include/qtredmine/SimpleRedmineTypes.h \

SOURCES += \
    KeyAuthenticator.cpp \
    Logging.cpp \
    PasswordAuthenticator.cpp \
    RedmineClient.cpp \
    SimpleRedmineClient.cpp \

DISTFILES += \
    .travis.yml \
    qtredmine.pri \
    Example.h \
    Example.cpp \
    README.md \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/qtredmine.pri)
