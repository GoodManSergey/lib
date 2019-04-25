//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_SOCKET_TCP_H
#define LEARN_SOCKET_TCP_H

#include "socket.h"
#include "result_code.h"
#include "result.h"
#include "protocol.h"
#include "address.h"
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class SocketTcp: public Socket
{
public:
    result_code create_socket_fd() override ;
    result_code fill_addr(int port) override ;
    result_code set_in_addr() override ;
    result_code set_remote_addr(const std::string& host) override ;
    result_code connect_socket() override ;
    result<address> accept_socket() override ;
    result_code listen_socket() override ;
    result_code bind_socket() override ;
    std::string recv_msg(address& socket_addr) override ;
    result_code send_msg(std::string&& msg, address& socket_addr) override ;
    protocol return_protocol() override ;
private:
    int m_socket;
    sockaddr_in m_address;
};


#endif //LEARN_SOCKET_UDP_H
