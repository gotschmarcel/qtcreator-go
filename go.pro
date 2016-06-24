# Qt Creator linking

isEmpty(QTC_SOURCE): error(Set QTC_SOURCE to the QtCreator source code path)
isEmpty(QTC_BUILD): error(Set QTC_BUILD to the QtCreator build path)

message("QtCreator sources @ $$QTC_SOURCE")
message("QtCreator build @ $$QTC_BUILD")

QTCREATOR_SOURCES = $$QTC_SOURCE
IDE_BUILD_TREE = $$QTC_BUILD

# Setup

DEFINES += GO_LIBRARY

CONFIG += c++11

# Go files

SOURCES += goplugin.cpp \
    projects/goproject.cpp \
    projects/goprojectmanager.cpp \
    projects/goprojectwizard.cpp

HEADERS += goplugin.h \
        go_global.h \
        goconstants.h \
    projects/goproject.h \
    projects/goprojectmanager.h \
    projects/goprojectnode.h \
    projects/goprojectwizard.h

RESOURCES += \
    go.qrc

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = Go
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    texteditor \
    projectexplorer

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
