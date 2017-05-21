#pragma once

#include <QSharedPointer>
#include <QString>
#include <QTextStream>

#include <coreplugin/messagemanager.h>

namespace Go {
namespace Internal {

class GeneralMessages
{
    class Stream
    {
    public:
        explicit Stream()
            : _d(new Data)
        {
        }

        template <typename Printable>
        Stream &operator<<(const Printable &p)
        {
            _d->stream << p;
            return *this;
        }

    private:
        struct Data
        {
            Data()
                : stream(&string)
            {
            }
            ~Data()
            {
                Core::MessageManager::showOutputPane();
                Core::MessageManager::write(string);
            }

            QString string;
            QTextStream stream;
        };

        QSharedPointer<Data> _d;
    };

public:
    static Stream append();
    static Stream warning();
    static Stream error();
};

} // namespace Internal
} // namespace Go
