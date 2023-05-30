#include "builder_concrete.h"

builder* builder_concrete::add_stream(
    std::string const& name_stream,
    logger::severity level)
{
    _streams.emplace_back(name_stream, level);

    return this;
}

builder* builder_concrete::clear()
{
    _streams.clear();

    return this;
}

logger* builder_concrete::build() const
{
    return new logger_concrete(_streams);
}