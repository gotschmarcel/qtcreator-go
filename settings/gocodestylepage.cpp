#include "gocodestylepage.h"
#include "ui_gocodestylepage.h"

#include <QCoreApplication>

#include "../goconstants.h"

using namespace Go::Internal;

CodeStylePageWidget::CodeStylePageWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::CodeStylePageWidget)
{
    _ui->setupUi(this);
}

CodeStylePageWidget::~CodeStylePageWidget() { delete _ui; }

// CodeStylePage

CodeStylePage::CodeStylePage(QWidget *parent)
    : Core::IOptionsPage(parent)
    , _widget(nullptr)
{
    setId(Constants::SettingsID);
    setDisplayName(QCoreApplication::translate("Go.Settings", "Code Style"));
    setCategory(Constants::SettingsCategory);
    setDisplayCategory(QLatin1String(Constants::SettingsDisplayCategory));
    setCategoryIcon(Utils::Icon(QLatin1String(Constants::Icon)));
}

QWidget *CodeStylePage::widget()
{
    if (!_widget) {
        _widget = new CodeStylePageWidget;
    }

    return _widget;
}

void CodeStylePage::apply() {}

void CodeStylePage::finish()
{
    delete _widget;
    _widget = nullptr;
}
