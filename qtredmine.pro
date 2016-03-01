QT       += network xml
QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = qtredmine
TEMPLATE = lib
CONFIG += staticlib

DEFINES += REDMINEQT_LIBRARY

SOURCES += \
	RedmineClient.cpp \
	PasswordAuthenticator.cpp \
	KeyAuthenticator.cpp

HEADERS += \
        redmine-qt_global.hpp \
	RedmineClient.hpp \
	PasswordAuthenticator.hpp \
	IAuthenticator.hpp \
	KeyAuthenticator.hpp

