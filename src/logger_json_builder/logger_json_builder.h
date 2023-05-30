#ifndef LOGGER_JSON_IMPLEMENTATION_H
#define LOGGER_JSON_IMPLEMENTATION_H
#include "forJson/single_include/nlohmann/json.hpp"
#include "../logger/logger.h"

class json_builder
{

public:

    virtual logger *build(
            const std::string &path) = 0;

    virtual ~json_builder();

};

#endif //LOGGER_JSON_IMPLEMENTATION_H
