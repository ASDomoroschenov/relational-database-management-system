#include "logger.h"

std::map<std::string, std::pair<std::ofstream*, size_t>> logger::_collection_streams = std::map<std::string, std::pair<std::ofstream*, size_t>>();

logger::~logger()
{
    
}