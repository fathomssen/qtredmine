QT += network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = qtredmine
TEMPLATE = lib

DEFINES += QTREDMINE_LIBRARY

# No debug statements in release
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(debug, debug|release):DEFINES += DEBUG_OUTPUT

SOURCES += \
    PasswordAuthenticator.cpp \
    KeyAuthenticator.cpp \
    Example.cpp \
    SimpleRedmineClient.cpp \
    RedmineClient.cpp

HEADERS += \
    KeyAuthenticator.h \
    PasswordAuthenticator.h \
    Authenticator.h \
    logging.h \
    Example.h \
    qtredmine_global.h \
    RedmineClient.h \
    SimpleRedmineTypes.h \
    SimpleRedmineClient.h

DISTFILES += \
    README.md \
    .travis.yml

