#pragma once

#include <QStandardItemModel>

#include <coreplugin/dialogs/ioptionspage.h>

#include "../go_global.h"
#include "ui_buildnrunsettingspagewidget.h"

namespace Go {
namespace Internal {

class BuildNRunSettingsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuildNRunSettingsPageWidget();

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

    Ui::BuildNRunSettingsPageWidget _ui;
    QStandardItemModel _model;
};

class BuildNRunSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    explicit BuildNRunSettingsPage(QWidget *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    BuildNRunSettingsPageWidget *_widget = nullptr;
};

} // namespace Internal
} // namespace Go
