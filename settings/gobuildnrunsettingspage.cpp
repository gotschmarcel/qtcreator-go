#include "gobuildnrunsettingspage.h"

#include <QCoreApplication>
#include <QWidget>

#include <coreplugin/icore.h>
#include <utils/qtcassert.h>
#include <projectexplorer/projectexplorerconstants.h>

#include "../goconstants.h"

using namespace Go::Internal;

//
// BuildNRunSettingsPageWidget
//

BuildNRunSettingsPageWidget::BuildNRunSettingsPageWidget() {
    _ui.setupUi(this);

    connect(_ui.addButton, SIGNAL(clicked(bool)), SLOT(addTool()));
    connect(_ui.cloneButton, SIGNAL(clicked(bool)), SLOT(cloneTool()));
    connect(_ui.removeButton, SIGNAL(clicked(bool)), SLOT(removeTool()));
    connect(_ui.makeDefaultButton, SIGNAL(clicked(bool)), SLOT(makeDefaultTool()));

    // Hide details view initially.
    _ui.details->setVisible(false);

    // Setup table model.
    _model.setColumnCount(3);
    _model.setHorizontalHeaderLabels({QLatin1String("Name"), QLatin1String("Root"), QLatin1String("Path")});
    _model.insertRow(0, new QStandardItem(QLatin1String("Auto-detected")));
    _model.insertRow(1, new QStandardItem(QLatin1String("Manual")));

    // Setup table view.
    _ui.treeView->setModel(&_model);
    _ui.treeView->setUniformRowHeights(true);
    _ui.treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui.treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui.treeView->expandAll();

    auto header = _ui.treeView->header();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(_ui.treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(showDetails(QModelIndex)));
}

void BuildNRunSettingsPageWidget::addTool() {
    // TODO: Implement.
}

void BuildNRunSettingsPageWidget::cloneTool() {
    // TODO: Implement.
}

void BuildNRunSettingsPageWidget::removeTool() {
    // TODO: Implement.
}

void BuildNRunSettingsPageWidget::makeDefaultTool() {
    // TODO: Implement.
}

void BuildNRunSettingsPageWidget::showDetails(const QModelIndex& index) {
    Q_UNUSED(index);
    // TODO: Implement.
}

QStandardItem*BuildNRunSettingsPageWidget::autoDetectItem() {
    return _model.item(0);
}

QStandardItem*BuildNRunSettingsPageWidget::manualItem() {
    return _model.item(1);
}

//
// BuildNRunSettingsPage
//

BuildNRunSettingsPage::BuildNRunSettingsPage(QWidget* parent)
    : IOptionsPage(parent) {
    setId(Constants::SettingsBuildNRunID);
    setDisplayName(tr("Go"));
    setCategory(ProjectExplorer::Constants::PROJECTEXPLORER_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("ProjectExplorer", ProjectExplorer::Constants::PROJECTEXPLORER_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(ProjectExplorer::Constants::PROJECTEXPLORER_SETTINGS_CATEGORY_ICON));
}

QWidget* BuildNRunSettingsPage::widget() {
    if (!_widget) {
        _widget = new BuildNRunSettingsPageWidget;
    }

    return _widget;
}

void BuildNRunSettingsPage::apply() {
    QTC_ASSERT(_widget, return);
    // TODO: Apply
}

void BuildNRunSettingsPage::finish() {
    delete _widget;
    _widget = nullptr;
}
