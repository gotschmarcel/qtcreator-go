#include "goapplicationwizardfactory.h"

#include <coreplugin/basefilewizard.h>
#include <coreplugin/id.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>

#include <QDir>
#include <QFileInfo>

#include "../goconstants.h"

namespace Go {
namespace Internal {

GoApplicationWizardFactory::GoApplicationWizardFactory()
{
    setSupportedProjectTypes({Constants::ProjectID});

    setDisplayName(tr("Go Application"));
    setId("Z.GoNew");
    setDescription(tr("Creates a new Go application project."));
    setCategory(QLatin1String(ProjectExplorer::Constants::QT_APPLICATION_WIZARD_CATEGORY));
    setDisplayCategory(
        QLatin1String(ProjectExplorer::Constants::QT_APPLICATION_WIZARD_CATEGORY_DISPLAY));

    setIcon(QIcon(QLatin1String(Constants::Icon)));
}

Core::BaseFileWizard *
GoApplicationWizardFactory::create(QWidget *parent,
                                   const Core::WizardDialogParameters &parameters) const
{
    Core::BaseFileWizard *wizard = new Core::BaseFileWizard(this, parameters.extraValues(), parent);
    wizard->setWindowTitle(displayName());

    Utils::FileWizardPage *page = new Utils::FileWizardPage;
    page->setPath(parameters.defaultPath());
    wizard->addPage(page);

    for (auto p : wizard->extensionPages()) {
        wizard->addPage(p);
    }

    return wizard;
}

Core::GeneratedFiles GoApplicationWizardFactory::generateFiles(const QWizard *widget,
                                                               QString *errorMessage) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    const auto page = wizard->find<Utils::FileWizardPage>();

    const QDir projectRoot(page->path());
    const QDir projectDir = projectRoot.absoluteFilePath(page->fileName());

    // Create the project directory.
    if (!projectDir.exists() && !projectRoot.mkdir(page->fileName())) {
        *errorMessage = tr("Failed to create project directory: %1").arg(projectDir.absolutePath());
        return {};
    }

    // Create the project file.
    const QString projectFilePath = projectDir.absoluteFilePath(page->fileName())
                                        .append(QLatin1String(Constants::ProjectFileExt));

    Core::GeneratedFile projectFile(projectFilePath);
    projectFile.setContents(QLatin1String("# Go Project\n"));
    projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    // Open the template file.
    QFile templateFile(QLatin1String("://templates/application.go"));
    if (!templateFile.open(QIODevice::ReadOnly)) {
        *errorMessage = tr("Failed to load template");
        return {};
    }

    // Create the main.go file from the template.
    const QString mainFilePath = projectDir.filePath(QLatin1String("main.go"));
    Core::GeneratedFile mainFile(mainFilePath);
    mainFile.setBinaryContents(templateFile.readAll());
    mainFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return {projectFile, mainFile};
}

bool GoApplicationWizardFactory::postGenerateFiles(const QWizard *, const Core::GeneratedFiles &files,
                                                   QString *errorMessage) const
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}

} // Internal
} // Go
