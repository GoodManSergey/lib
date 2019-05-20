//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_SOCKET_TCP_H
#define LEARN_SOCKET_TCP_H

#include "socket.h"
#include "result_code.h"
#include "result.h"
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <memory>
#include <poll.h>
#include <fcntl.h>


class SocketTcp: public Socket
{
public:
    SocketTcp() = default;
    SocketTcp(int fd):
    m_socket(fd)
    {}

private:
    result_code create_socket_fd();
    result_code fill_addr(int port);
    result_code set_in_addr();
    result_code set_remote_addr(const std::string& host);
    result_code listen_socket();

public:
    result_code connect_socket(const std::string& host, int port) override ;
    result<std::shared_ptr<Socket>> accept_socket() override ;
    result_code bind_socket(int port) override ;
    message recv_msg() override ;
    result_code send_msg(message& msg) override ;
    address get_address() override ;
    ~SocketTcp() override ;

private:
    int m_socket{};
    sockaddr_in m_address{};
    int m_buffer_size = 1024;
    char m_buffer[1024]{0};
    int m_buffer_iter{0};
    std::string msg;
};


#endif //LEARN_SOCKET_UDP_H
