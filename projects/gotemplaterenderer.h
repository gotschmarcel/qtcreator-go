#pragma once

#include <QString>

template <typename Key, typename Value>
class QMap;

namespace Go {

class TemplateRenderer
{
public:
    explicit TemplateRenderer(const QString&& tpl);

    void setTemplate(const QString&& tpl);
    QString render(const QMap<QString, QString>&& locals) const;

private:
    QString _tpl;
};

} // Go
