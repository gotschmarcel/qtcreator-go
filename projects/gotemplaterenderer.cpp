#include "gotemplaterenderer.h"

#include <QMap>
#include <QStringBuilder>

namespace Go {
namespace Internal {

class CharacterStream
{
public:
    explicit CharacterStream(const QString &string)
        : _s(string)
        , _pos(0)
    {
    }

    QChar peek(const int offset = 0) const
    {
        const int pos = _pos + offset;

        if (pos >= _s.length()) {
            return QChar();
        }

        return _s[pos];
    }

    void next() { ++_pos; }
    bool atEnd() const { return _pos >= _s.length(); }

private:
    const QString &_s;
    int _pos;
};

TemplateRenderer::TemplateRenderer(const QString &&tpl)
    : _tpl(std::move(tpl))
{
}

void TemplateRenderer::setTemplate(const QString &&tpl) { _tpl = std::move(tpl); }

QString TemplateRenderer::render(const QMap<QString, QString> &&locals) const
{
    enum State
    {
        S_COPY,
        S_PARSE_NAME
    };

    QString result;
    CharacterStream stream(_tpl);
    State state = S_COPY;

    while (!stream.atEnd()) {
        switch (state) {
        case S_COPY:
            if (stream.peek() == QLatin1Char('{') && stream.peek(1) == QLatin1Char('{')) {
                state = S_PARSE_NAME;
                stream.next(); // Eat '{'
                stream.next(); // Eat '{'
                continue;
            }

            result = result % stream.peek();
            stream.next();
            break;
        case S_PARSE_NAME:
            QString name;
            QChar c = stream.peek();

            while (!stream.atEnd() && (c.isSpace() || c.isLetter() || c == QLatin1Char('_'))) {
                name = name % c;
                stream.next(); // Eat
                c = stream.peek();
            }

            // Ignore error silently by ignoring this partly placeholder.
            if (c != QLatin1Char('}') || stream.peek(1) != QLatin1Char('}')) {
                state = S_COPY;
                result = result % QLatin1String("{{") % name;
                break;
            }

            // Ignore non existing value placeholder.
            const QString &trimmed = name.trimmed();

            if (!locals.contains(trimmed)) {
                state = S_COPY;
                result = result % QLatin1String("{{") % name % QLatin1String("}}");
                break;
            }

            // Replace with value.
            result = result % locals.value(trimmed);

            stream.next(); // Eat '}'
            stream.next(); // Eat '}'

            state = S_COPY;
            break;
        }
    }

    return result;
}

} // Internal
} // Go
