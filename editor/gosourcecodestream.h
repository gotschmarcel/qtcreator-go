#pragma once

#include <QString>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT SourceCodeStream
{
public:
    SourceCodeStream(const QString &text)
        : _text(text)
        , _ptr(text.data())
        , _len(text.length())
        , _pos(0)
        , _anchor(0)
    {
    }

    inline void setAnchor() { _anchor = _pos; }
    inline void advance() { ++_pos; }
    inline int pos() const { return _pos; }
    inline int len() const { return _pos - _anchor; }
    inline int anchor() const { return _anchor; }
    inline bool atEnd() const { return _pos >= _len; }

    inline QChar peek(const int offset = 0) const
    {
        const int pos = _pos + offset;

        if (pos >= _len) {
            return QChar();
        }

        return _ptr[pos];
    }

    inline QStringRef value() const { return QStringRef(&_text, _anchor, len()); }
    inline QStringRef value(const int begin, const int length) const
    {
        return QStringRef(&_text, begin, length);
    }

private:
    const QString &_text;
    const QChar *_ptr;
    const int _len;
    int _pos;
    int _anchor;
};

} // namespace Internal
} // namespace Go
