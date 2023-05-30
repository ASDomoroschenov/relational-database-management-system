#include "logger_holder.h"
#include "logger_concrete.h"

logger *logger_holder::log_with_guard(
        const std::string &target_log,
        logger::severity level) const
{
    logger *logger_item = get_logger();

    if (logger_item == nullptr)
    {
        std::cout << current_date_time() << " " << enum_to_string(level) << " " << target_log << std::endl;

        return nullptr;
    }

    return logger_item->log(target_log, level);
}