#include <sstream>
#include "client_concrete.h"

//begin region client_concrete

client_concrete::client_concrete():
    _data_base(nullptr)
{

}

client_concrete::~client_concrete()
{

}

void client_concrete::send_request(
        const std::string &request)
{

}

void client_concrete::dialogue()
{
    handler *handler = new handler_concrete;
    std::string line_command;

    while (line_command != "STOP")
    {
        std::getline(std::cin, line_command);
        send_request(line_command);
    }

    delete handler;
}

//end region client_concrete