#pragma once

#include <QList>
#include <QObject>

#include "go_global.h"

namespace Go {
namespace Internal {

class GoTool;

class GoToolManager : public QObject
{
    Q_OBJECT
public:
    static GoToolManager &instance();

    void autoDetectTools(const QStringList &additionalLocations = QStringList());
    const QList<GoTool *> &tools() const;

private:
    Q_DISABLE_COPY(GoToolManager)
    explicit GoToolManager();
    ~GoToolManager();

    QList<GoTool *> _tools;
};

} // namespace Internal
} // namespace Go
