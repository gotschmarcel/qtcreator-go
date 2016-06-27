#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

#include <QWidget>

#include "../go_global.h"

namespace Ui {
class CodeStylePageWidget;
}

namespace Go {

class CodeStylePageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CodeStylePageWidget(QWidget *parent = 0);
    ~CodeStylePageWidget();

private:
    Ui::CodeStylePageWidget* _ui;
};

class GOSHARED_EXPORT CodeStylePage final : public Core::IOptionsPage {
    Q_OBJECT

public:
    explicit CodeStylePage(QWidget* parent = nullptr);

    QWidget* widget() override;
    void apply() override;
    void finish() override;

private:
    CodeStylePageWidget* _widget;
};

} // Go
