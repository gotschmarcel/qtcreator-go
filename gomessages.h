#pragma once

#include <QString>
#include <QTextStream>

#include <coreplugin/messagemanager.h>

namespace Go {
namespace Internal {

class LocalMessageStream {
public:
    explicit LocalMessageStream() : _d(new Data) {}

    ~LocalMessageStream() {
        if (--_d->refs == 0) {
            Core::MessageManager::showOutputPane();
            Core::MessageManager::write(_d->s.readAll());
            delete _d;
        }
    }

    LocalMessageStream(const LocalMessageStream& o) : _d(o._d) { ++_d->refs; }

    template <typename Printable>
    LocalMessageStream& operator<<(const Printable& p) {
        _d->s << p;
        return *this;
    }

private:
    struct Data {
        Data() : s(&d), refs(1) {}

        QString d;
        QTextStream s;
        int refs;
    }* _d;
};

class Messages {
public:
    static LocalMessageStream append();
    static LocalMessageStream warning();
    static LocalMessageStream error();
};

} // Internal
} // Go
