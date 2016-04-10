TEMPLATE = lib
TARGET = qtredmine

QT += network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11

DEFINES += QTREDMINE_LIBRARY

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

SOURCES += \
    PasswordAuthenticator.cpp \
    KeyAuthenticator.cpp \
    Example.cpp \
    SimpleRedmineClient.cpp \
    RedmineClient.cpp

DISTFILES += \
    README.md \
    .travis.yml

# No debug statements in release
CONFIG(release, debug|release):DEFINES += DEBUG_OUTPUT # QT_NO_DEBUG_OUTPUT
CONFIG(debug, debug|release):DEFINES += DEBUG_OUTPUT
