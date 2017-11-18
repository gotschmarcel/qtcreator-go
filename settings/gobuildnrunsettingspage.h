#pragma once

#include <QStandardItemModel>

#include <coreplugin/dialogs/ioptionspage.h>

#include "../go_global.h"
#include "ui_gobuildnrunsettingspagewidget.h"

namespace Go {
namespace Internal {

class GoBuildNRunSettingsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GoBuildNRunSettingsPageWidget();

private slots:
    void addTool();
    void cloneTool();
    void removeTool();
    void makeDefaultTool();
    void showDetails(const QModelIndex &index);
    void updateName(const QString &name);
    void updateGoRoot(const QString &goRoot);
    void updateGoPath(const QString &goPath);

private:
    QStandardItem *autoDetectItem() const;
    QStandardItem *manualItem() const;
    QStandardItem *selectedItem() const;

    Ui::GoBuildNRunSettingsPageWidget _ui;
    QStandardItemModel _model;
};

class GoBuildNRunSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    explicit GoBuildNRunSettingsPage(QWidget *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    GoBuildNRunSettingsPageWidget *_widget = nullptr;
};

} // namespace Internal
} // namespace Go
