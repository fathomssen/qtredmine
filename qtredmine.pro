TEMPLATE = lib
TARGET = qtredmine

DEFINES += QTREDMINE_LIBRARY

QT += network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/include/qtredmine

HEADERS += \
    Example.h \
    include/qtredmine/logging.h \
    include/qtredmine/qtredmine_global.h \
    include/qtredmine/Authenticator.h \
    include/qtredmine/KeyAuthenticator.h \
    include/qtredmine/PasswordAuthenticator.h \
    include/qtredmine/RedmineClient.h \
    include/qtredmine/SimpleRedmineClient.h \
    include/qtredmine/SimpleRedmineTypes.h \

SOURCES += \
    PasswordAuthenticator.cpp \
    KeyAuthenticator.cpp \
    Example.cpp \
    SimpleRedmineClient.cpp \
    RedmineClient.cpp

DISTFILES += \
    README.md \
    .travis.yml \
    qtredmine.pri \

# No debug statements in release
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(debug, debug|release):DEFINES += DEBUG_OUTPUT

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/qtredmine.pri)
