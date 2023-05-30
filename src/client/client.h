#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include "types.h"

class client
{

public:

    virtual void send_request(
            const std::string &request) = 0;

    virtual void dialogue() = 0;

    virtual ~client();

};

#endif //CLIENT_H