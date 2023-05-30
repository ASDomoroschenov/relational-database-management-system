#ifndef JSON_CONCRETE_H
#define JSON_CONCRETE_H

#include <fstream>
#include "logger.h"
#include "logger_concrete.h"
#include "logger_json_builder.h"

class json_builder_concrete final : public json_builder
{

private:

    std::vector<std::pair<std::string, logger::severity>> _streams;

public:

    logger *build(
            const std::string &path) override;

};

#endif //JSON_CONCRETE_H
