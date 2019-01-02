QT += core gui widgets sql

TARGET = cved
TEMPLATE = app

SOURCES += src/main.cc \
           src/cved.cc \
           src/update.cc \
           src/finder.cc \
           src/manager.cc

HEADERS += src/cved.h \
           src/update.h \
           src/finder.h \
           src/manager.h
           src/ui.h

RESOURCES = resources.qrc

RC_ICONS += resources/images/global/icon-cved.icon

CONFIG += release

OBJECTS_DIR = .build/.obj
MOC_DIR = .build/.moc
RCC_DIR = .build/.rcc

target.path = /usr/local/bin

INSTALLS += target

!exists($$(HOME)/.cved/db.sqlite) {
    system("mkdir $$(HOME)/.cved")
    system("cp $$(PWD)/db/db.sqlite $$(HOME)/.cved")
    system("cp -r $$(PWD)/db/git $$(HOME)/.cved/.git")
}
