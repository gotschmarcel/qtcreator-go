#include "gotool.h"

#include <QUuid>

using namespace Go::Internal;

GoTool::GoTool(const GoTool::Detection& detection, const Utils::FileName& executablePath)
    : _detection(detection), _executablePath(executablePath),
      _id(Core::Id::fromString(QUuid::createUuid().toString()))
{

}

GoTool::GoTool(const Detection& detection)
    : GoTool(detection, Utils::FileName::fromString(QLatin1String("")))
{

}

bool GoTool::isAutodetected() const
{
    return _detection == Detection::AUTO;
}

Core::Id GoTool::id() const
{
    return _id;
}

Utils::FileName GoTool::executablePath() const
{
    return _executablePath;
}
