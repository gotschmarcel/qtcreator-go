#include "golibrarywizardfactory.h"

#include <coreplugin/basefilewizard.h>
#include <coreplugin/id.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>

#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QRegularExpression>

#include "../goconstants.h"
#include "gotemplaterenderer.h"

using namespace Go::Internal;

LibraryWizardFactory::LibraryWizardFactory()
{
    setSupportedProjectTypes({Constants::ProjectID});

    setDisplayName(tr("Go Library"));
    setId("Z.GoLibrary");
    setDescription(tr("Creates a new Go library project."));
    setCategory(QLatin1String(ProjectExplorer::Constants::LIBRARIES_WIZARD_CATEGORY));
    setDisplayCategory(
        QLatin1String(ProjectExplorer::Constants::LIBRARIES_WIZARD_CATEGORY_DISPLAY));

    setIcon(QIcon(QLatin1String(Constants::Icon)));
}

Core::BaseFileWizard *
LibraryWizardFactory::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const
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

Core::GeneratedFiles LibraryWizardFactory::generateFiles(const QWizard *widget,
                                                         QString *errorMessage) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    const auto page = wizard->find<Utils::FileWizardPage>();

    const QDir projectRoot(page->path());
    const QDir projectDir = projectRoot.absoluteFilePath(page->fileName());
    const QString projectName = page->fileName();
    const QString packageName = projectName.trimmed().toLower().replace(
        QRegularExpression(QLatin1String("\\s+")), QLatin1String("_"));

    // Create the project directory.
    if (!projectDir.exists() && !projectRoot.mkdir(projectName)) {
        *errorMessage = tr("Failed to create project directory: %1").arg(projectDir.absolutePath());
        return {};
    }

    // Create the project file.
    const QString projectFilePath
        = projectDir.absoluteFilePath(projectName).append(QLatin1String(Constants::ProjectFileExt));

    Core::GeneratedFile projectFile(projectFilePath);
    projectFile.setContents(QLatin1String("# Go Project\n"));
    projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    // Open the template file.
    QFile templateFile(QLatin1String("://templates/library.go"));
    if (!templateFile.open(QIODevice::ReadOnly)) {
        *errorMessage = tr("Failed to load template");
        return {};
    }

    // Render the template
    TemplateRenderer renderer(QString::fromUtf8(templateFile.readAll()));

    // Create the main.go file from the template.
    const QString mainFilePath = projectDir.filePath(page->fileName().toLower() + QLatin1String(".")
                                                     + QLatin1String(Constants::FileExt));
    Core::GeneratedFile mainFile(mainFilePath);
    mainFile.setContents(renderer.render({{QLatin1String("package_name"), packageName}}));
    mainFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return {projectFile, mainFile};
}

bool LibraryWizardFactory::postGenerateFiles(const QWizard *, const Core::GeneratedFiles &files,
                                             QString *errorMessage) const
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}
