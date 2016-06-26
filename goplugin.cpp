#include "goplugin.h"
#include "goconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <texteditor/texteditor.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/simplecodestylepreferences.h>
#include <texteditor/codestylepool.h>
#include <texteditor/tabsettings.h>

#include <utils/mimetypes/mimedatabase.h>

#include <QtPlugin>

#include "projects/goprojectwizard.h"
#include "projects/goprojectmanager.h"
#include "editor/goeditorfactory.h"
#include "editor/gocodestylepreferencesfactory.h"

using namespace Go::Internal;

GoPlugin::GoPlugin()
{
    // Create your members
}

GoPlugin::~GoPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
    TextEditor::TextEditorSettings::unregisterCodeStyle(Constants::SettingsID);
    TextEditor::TextEditorSettings::unregisterCodeStylePool(Constants::SettingsID);
    TextEditor::TextEditorSettings::unregisterCodeStyleFactory(Constants::SettingsID);
}

bool GoPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    Utils::MimeDatabase::addMimeTypes(QLatin1String(":/go/mimetypes.xml"));

    // Setup Code Style
    auto codeStyleFactory = new CodeStylePreferencesFactory;
    TextEditor::TextEditorSettings::registerCodeStyleFactory(codeStyleFactory);

    auto codeStylePool = new TextEditor::CodeStylePool(codeStyleFactory, this);
    TextEditor::TextEditorSettings::registerCodeStylePool(Constants::SettingsID, codeStylePool);

    auto globalCodeStyle = new TextEditor::SimpleCodeStylePreferences(this);
    globalCodeStyle->setDelegatingPool(codeStylePool);
    globalCodeStyle->setDisplayName(tr("Global", "Settings"));
    globalCodeStyle->setId("GoGlobal");

    codeStylePool->addCodeStyle(globalCodeStyle);
    TextEditor::TextEditorSettings::registerCodeStyle(Constants::SettingsID, globalCodeStyle);

    auto goCodeStyle = new TextEditor::SimpleCodeStylePreferences;
    goCodeStyle->setId("go");
    goCodeStyle->setDisplayName(tr("GoCreator"));
    goCodeStyle->setReadOnly(true);

    TextEditor::TabSettings goTabSettings;
    goTabSettings.m_tabPolicy = TextEditor::TabSettings::TabsOnlyTabPolicy;
    goTabSettings.m_tabSize = 8;
    goTabSettings.m_indentSize = 8;
    goTabSettings.m_continuationAlignBehavior = TextEditor::TabSettings::ContinuationAlignWithIndent;

    goCodeStyle->setTabSettings(goTabSettings);

    globalCodeStyle->setCurrentDelegate(goCodeStyle);
    codeStylePool->loadCustomCodeStyles();
    globalCodeStyle->fromSettings(QLatin1String(Constants::SettingsID), Core::ICore::settings());

    TextEditor::TextEditorSettings::registerMimeTypeForLanguageId(Constants::MIMEType, Constants::SettingsID);

    //

    addAutoReleasedObject(new EditorFactory);

    addAutoReleasedObject(new ProjectManager);

    Core::IWizardFactory::registerFactoryCreator([]() {
        return QList<Core::IWizardFactory*>() << new ProjectWizard;
    });

    addAutoReleasedObject(new ProjectWizard);

    return true;
}

void GoPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag GoPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}
