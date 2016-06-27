#include "goimportwizardfactory.h"

#include <coreplugin/basefilewizard.h>
#include <coreplugin/id.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>

#include <QDir>
#include <QFileInfo>

#include "../goconstants.h"

using namespace Go;

ImportWizardFactory::ImportWizardFactory()
{
    setSupportedProjectTypes({ Constants::ProjectID });

    setDisplayName(tr("Import Existing Go Project"));
    setId("Z.GoImport");
    setDescription(tr("Imports existing Go projects."));
    setCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY));
    setDisplayCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY_DISPLAY));

    setIcon(QIcon(QLatin1String(Constants::Icon)));
}

Core::BaseFileWizard* Go::ImportWizardFactory::create(QWidget* parent, const Core::WizardDialogParameters& parameters) const
{
    Core::BaseFileWizard* wizard = new Core::BaseFileWizard(this, parameters.extraValues(), parent);
    wizard->setWindowTitle(displayName());

    Utils::FileWizardPage *page = new Utils::FileWizardPage;
    page->setPath(parameters.defaultPath());
    wizard->addPage(page);

    for (auto p : wizard->extensionPages()) {
        wizard->addPage(p);
    }

    return wizard;
}

Core::GeneratedFiles ImportWizardFactory::generateFiles(const QWizard* widget, QString*) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard*>(widget);
    const auto page   = wizard->find<Utils::FileWizardPage>();

    const QDir projectDir(page->path());
    const QString projectName(page->fileName());
    const QString projectFilePath = QFileInfo(
                projectDir, projectName + QLatin1String(Constants::ProjectFileExt)).absoluteFilePath();

    Core::GeneratedFile projectFile(projectFilePath);
    projectFile.setContents(QLatin1String("# Go Project\n"));
    projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    return { projectFile };
}

bool ImportWizardFactory::postGenerateFiles(const QWizard*, const Core::GeneratedFiles& files, QString* errorMessage) const
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}
