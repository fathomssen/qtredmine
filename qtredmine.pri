INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD

!equals(_PRO_FILE_PWD_, $$PWD) {
  win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release -L$$PWD/Release/release -L$$shadowed($$PWD)/release -lqtredmine
  else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug -L$$PWD/Debug/debug -L$$shadowed($$PWD)/debug -lqtredmine
  else:unix: LIBS += -L$$shadowed($$PWD) -lqtredmine
}
