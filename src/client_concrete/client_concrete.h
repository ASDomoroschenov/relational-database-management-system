#ifndef CLIENT_CONCRETE_H
#define CLIENT_CONCRETE_H

#include "client.h"
#include "types.h"
#include "handle_request.h"
#include "handle_request_concrete.h"

std::string delete_spaces(
        const std::string &str);

std::vector<std::string> split_by_spaces(
        const std::string &str);

class client_concrete: public client
{

private:

    type_data_base_rb *_data_base;

public:

    client_concrete();

    void send_request(
            const std::string &request) override;

    void dialogue() override;

    ~client_concrete() override;

};

#endif //CLIENT_CONCRETE_H