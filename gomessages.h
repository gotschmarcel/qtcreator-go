#pragma once

#include <QString>
#include <QTextStream>
#include <QSharedPointer>

#include <coreplugin/messagemanager.h>

namespace Go {
namespace Internal {

class LocalMessageStream {
public:
    explicit LocalMessageStream() : _d(new Data) {}

    template <typename Printable>
    LocalMessageStream& operator<<(const Printable& p) {
        _d->stream << p;
        return *this;
    }

private:
    struct Data {
        Data() : stream(&string) {}
        ~Data() {
            Core::MessageManager::showOutputPane();
            Core::MessageManager::write(string);
        }

        QString string;
        QTextStream stream;
    };

    QSharedPointer<Data> _d;
};

class Messages {
public:
    static LocalMessageStream append();
    static LocalMessageStream warning();
    static LocalMessageStream error();
};

} // Internal
} // Go
