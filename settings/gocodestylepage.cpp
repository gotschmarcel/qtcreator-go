#include "gocodestylepage.h"
#include "ui_gocodestylepage.h"

#include <QCoreApplication>

#include "../goconstants.h"

namespace Go {
namespace Internal {

GoCodeStylePageWidget::GoCodeStylePageWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::GoCodeStylePageWidget)
{
    _ui->setupUi(this);
}

GoCodeStylePageWidget::~GoCodeStylePageWidget() { delete _ui; }

// CodeStylePage

GoCodeStylePage::GoCodeStylePage(QWidget *parent)
    : Core::IOptionsPage(parent)
    , _widget(nullptr)
{
    setId(Constants::SettingsID);
    setDisplayName(QCoreApplication::translate("Go.Settings", "Code Style"));
    setCategory(Constants::SettingsCategory);
    setDisplayCategory(QLatin1String(Constants::SettingsDisplayCategory));
    setCategoryIcon(Utils::Icon(QLatin1String(Constants::Icon)));
}

QWidget *GoCodeStylePage::widget()
{
    if (!_widget) {
        _widget = new GoCodeStylePageWidget;
    }

    return _widget;
}

void GoCodeStylePage::apply() {}

void GoCodeStylePage::finish()
{
    delete _widget;
    _widget = nullptr;
}

} // Internal
} // Go
