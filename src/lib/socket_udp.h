//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_SOCKET_UDP_H
#define LEARN_SOCKET_UDP_H

#include "socket.h"
#include "result_code.h"
#include "result.h"
#include "address.h"
#include "message.h"
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include <poll.h>


class SocketUdp: public Socket, std::enable_shared_from_this<Socket>
{
public:
    result_code create_socket_fd() override ;
    result_code fill_addr(int port) override ;
    result_code set_in_addr() override ;
    result_code set_remote_addr(const std::string& host) override ;
    result_code connect_socket() override ;
    result<std::shared_ptr<Socket>> accept_socket() override ;
    result_code listen_socket() override ;
    result_code bind_socket() override ;
    message recv_msg() override ;
    result_code send_msg(message& msg) override ;
    address get_address() override ;
    ~SocketUdp() override ;
private:
    int m_socket;
    sockaddr_in m_address;
    pollfd m_fds[1];

};


#endif //LEARN_SOCKET_UDP_H