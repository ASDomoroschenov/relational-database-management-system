#ifndef LOGGER_HOLDER_H
#define LOGGER_HOLDER_H

#include "logger.h"

class logger_holder
{

public:

    virtual ~logger_holder() = default;

    logger *log_with_guard(
            const std::string &target_log,
            logger::severity level) const;

    virtual logger *get_logger() const = 0;

};

#endif //LOGGER_HOLDER_H