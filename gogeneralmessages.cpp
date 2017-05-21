#include "gogeneralmessages.h"
#include "goconstants.h"

using namespace Go::Internal;

GeneralMessages::Stream GeneralMessages::append() { return Stream() << Constants::MessagePrefix; }

GeneralMessages::Stream GeneralMessages::warning()
{
    return Stream() << Constants::MessagePrefix << "Warning: ";
}

GeneralMessages::Stream GeneralMessages::error()
{
    return Stream() << Constants::MessagePrefix << "Error: ";
}
