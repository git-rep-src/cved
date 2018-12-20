QT += core gui widgets sql

TARGET = cved
TEMPLATE = app

SOURCES += src/main.cc \
           src/cved.cc \
           src/finder.cc \
           src/manager.cc \
           src/process.cc

HEADERS  += src/cved.h \
            src/finder.h \
            src/manager.h \
            src/process.h \
            src/ui.h

RESOURCES = resources.qrc

RC_FILE = resources/images/global/icon.rc

OBJECTS_DIR = .build/.obj
MOC_DIR = .build/.moc
RCC_DIR = .build/.rcc
