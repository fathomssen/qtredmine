QT += network xml
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = qtredmine
TEMPLATE = lib

DEFINES += QTREDMINE_LIBRARY

# No debug statements in release
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(debug, debug|release):DEFINES += DEBUG

SOURCES += \
    Redmine.cpp \
    PasswordAuthenticator.cpp \
    KeyAuthenticator.cpp \
    Example.cpp

HEADERS += \
    KeyAuthenticator.h \
    PasswordAuthenticator.h \
    Redmine.h \
    Authenticator.h \
    logging.h \
    Example.h \
    qtredmine_global.h

DISTFILES += \
    README.md \
    .travis.yml

