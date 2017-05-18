#pragma once

#include <QObject>
#include <QList>

#include "go_global.h"

namespace Go {
namespace Internal {

class GoTool;

class GOSHARED_EXPORT GoToolManager : public QObject
{
    Q_OBJECT
public:
    static GoToolManager& instance();

    void autoDetectTools(const QStringList& additionalLocations = QStringList());

private:
    Q_DISABLE_COPY(GoToolManager)
    explicit GoToolManager();
    ~GoToolManager();

    QList<GoTool*> _tools;
};

} // Internal
} // Go
