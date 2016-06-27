#pragma once

#include <coreplugin/basefilewizardfactory.h>

#include "../go_global.h"

namespace Go {

class GOSHARED_EXPORT ProjectWizard final : public Core::BaseFileWizardFactory
{
    Q_OBJECT

public:
    ProjectWizard();

protected:
    Core::BaseFileWizard* create(QWidget* parent, const Core::WizardDialogParameters &parameters) const override;
    Core::GeneratedFiles generateFiles(const QWizard* widget, QString*) const override;
    bool postGenerateFiles(const QWizard*, const Core::GeneratedFiles &files, QString* errorMessage) const override;
};

} // Go
