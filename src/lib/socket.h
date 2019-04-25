#ifndef LEARN_SOCKET_H
#define LEARN_SOCKET_H

#include "result_code.h"
#include "address.h"


class Socket
{
public:
    virtual result_code create_socket() = 0;
    virtual result_code fill_addr(int port) = 0;
    virtual result_code set_in_addr() = 0;
    virtual result_code set_remote_addr(const std::string& server_host) = 0;
    virtual result_code connect() = 0;
    virtual result_code accept() = 0;
    virtual result_code listen() = 0;
    virtual result_code bind() = 0;
    virtual std::string recv(address& socket_addr) = 0;
    virtual result_code send(const std::string& msg, const address& socket_addr) = 0;
};


#endif //LEARN_SOCKET_H
