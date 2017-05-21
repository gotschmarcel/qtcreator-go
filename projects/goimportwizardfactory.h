#pragma once

#include <coreplugin/basefilewizardfactory.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT ImportWizardFactory final : public Core::BaseFileWizardFactory
{
    Q_OBJECT

public:
    ImportWizardFactory();

protected:
    Core::BaseFileWizard *create(QWidget *parent,
                                 const Core::WizardDialogParameters &parameters) const override;
    Core::GeneratedFiles generateFiles(const QWizard *widget, QString *) const override;
    bool postGenerateFiles(const QWizard *, const Core::GeneratedFiles &files,
                           QString *errorMessage) const override;
};

} // namespace Internal
} // namespace Go
