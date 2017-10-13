DEFINES += GO_LIBRARY

SOURCES += goplugin.cpp \
    projects/goproject.cpp \
    editor/goeditor.cpp \
    editor/goeditorwidget.cpp \
    editor/goeditorfactory.cpp \
    editor/gosyntaxhighlighter.cpp \
    editor/goscanner.cpp \
    editor/goindenter.cpp \
    settings/gocodestylepage.cpp \
    projects/goapplicationwizardfactory.cpp \
    projects/goimportwizardfactory.cpp \
    projects/golibrarywizardfactory.cpp \
    projects/gotemplaterenderer.cpp \
    projects/goprojectnode.cpp \
    gotoolchain.cpp \
    projects/goprojectfile.cpp \
    gogeneralmessages.cpp \
    settings/gobuildnrunsettingspage.cpp \
    gotoolmanager.cpp \
    gotool.cpp

HEADERS += goplugin.h \
    go_global.h \
    goconstants.h \
    projects/goproject.h \
    projects/goprojectnode.h \
    editor/goeditor.h \
    editor/goeditorwidget.h \
    editor/goeditorfactory.h \
    editor/gosyntaxhighlighter.h \
    editor/goscanner.h \
    editor/goindenter.h \
    editor/sourcecodestream.h \
    settings/gocodestylepage.h \
    projects/goapplicationwizardfactory.h \
    projects/goimportwizardfactory.h \
    projects/golibrarywizardfactory.h \
    projects/gotemplaterenderer.h \
    gotoolchain.h \
    projects/goprojectfile.h \
    settings/gobuildnrunsettingspage.h \
    gotoolmanager.h \
    gotool.h \
    generalmessages.h

RESOURCES += \
    go.qrc

FORMS += \
    settings/gocodestylepage.ui \
    settings/gobuildnrunsettingspagewidget.ui

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
#USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = Go
QTC_LIB_DEPENDS += \
    utils
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    texteditor \
    projectexplorer

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include(config.pri)
include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)
