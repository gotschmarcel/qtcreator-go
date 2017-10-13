#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

#include <QWidget>

#include "../go_global.h"

namespace Go {
namespace Internal {
namespace Ui {
class GoCodeStylePageWidget;
}

class GoCodeStylePageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GoCodeStylePageWidget(QWidget *parent = 0);
    ~GoCodeStylePageWidget();

private:
    Ui::GoCodeStylePageWidget *_ui;
};

class GoCodeStylePage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    explicit GoCodeStylePage(QWidget *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    GoCodeStylePageWidget *_widget;
};

} // namespace Internal
} // namespace Go
