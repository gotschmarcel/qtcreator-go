#pragma once

#include <QString>

template <typename Key, typename Value>
class QMap;

namespace Go {
namespace Internal {

class GoTemplateRenderer
{
public:
    explicit GoTemplateRenderer(const QString &&tpl);

    void setTemplate(const QString &&tpl);
    QString render(const QMap<QString, QString> &&locals) const;

private:
    QString _tpl;
};

} // namespace Internal
} // namespace Go
