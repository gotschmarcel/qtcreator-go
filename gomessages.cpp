#include "gomessages.h"
#include "goconstants.h"

using namespace Go::Internal;

LocalMessageStream Messages::append()
{
    return LocalMessageStream() << Constants::MessagePrefix;
}

LocalMessageStream Messages::warning()
{
    return LocalMessageStream() << Constants::MessagePrefix << "Warning: ";
}

LocalMessageStream Messages::error()
{
    return LocalMessageStream() << Constants::MessagePrefix << "Error: ";
}
