#include "generalmessages.h"
#include "goconstants.h"

namespace Go {
namespace Internal {

GeneralMessages::Stream GeneralMessages::append() { return Stream() << Constants::MessagePrefix; }

GeneralMessages::Stream GeneralMessages::warning()
{
    return Stream() << Constants::MessagePrefix << "Warning: ";
}

GeneralMessages::Stream GeneralMessages::error()
{
    return Stream() << Constants::MessagePrefix << "Error: ";
}

} // Internal
} // Go
