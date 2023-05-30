#include <algorithm>
#include "handle_request.h"
#include "handle_request_concrete.h"

int main()
{

    handler *interpreter = new handler_concrete;
    std::string command;

    while (command != "STOP")
    {
        std::getline(std::cin, command);

        try
        {
            interpreter->accept_request(command);
        }
        catch (const std::exception &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    delete interpreter;

    return 0;
}