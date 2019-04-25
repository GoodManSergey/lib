#ifndef LEARN_SOCKET_H
#define LEARN_SOCKET_H

#include "result_code.h"
#include "address.h"
#include "protocol.h"
#include "result.h"
#include <string>


class Socket
{
public:
    virtual result_code create_socket_fd() = 0;
    virtual result_code fill_addr(int port) = 0;
    virtual result_code set_in_addr() = 0;
    virtual result_code set_remote_addr(const std::string& server_host) = 0;
    virtual result_code connect_socket() = 0;
    virtual result<address> accept_socket() = 0;
    virtual result_code listen_socket() = 0;
    virtual result_code bind_socket() = 0;
    virtual std::string recv_msg(address& socket_addr) = 0;
    virtual result_code send_msg(std::string&& msg, address& socket_addr) = 0;
    virtual protocol return_protocol() = 0;
};


#endif //LEARN_SOCKET_H
