#ifndef BUILDER_H
#define BUILDER_H

#include "../logger/logger.h"

class builder
{

public:

    virtual builder* add_stream(std::string const&, logger::severity) = 0;

    virtual builder* clear() = 0;

    virtual logger* build() const = 0;

    virtual ~builder();

};

#endif // BUILDER_H