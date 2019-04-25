//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_SOCKET_UDP_H
#define LEARN_SOCKET_UDP_H

#include "socket.h"
#include "result_code.h"
#include "result.h"

class SocketTcp: public Socket
{
public:
    result_code create_socket() override ;
    result_code fill_addr(int port) override ;
    result_code set_in_addr() override ;
    result_code set_remote_addr(const std::string& server_host) override ;
    result_code connect() override ;
    result accept() override ;
    result_code listen() override ;
    result_code bind() override ;
    std::string recv() override ;
    result_code send(const std::string& msg) override ;
private:
    int m_socket;
    sockaddr_in m_address;
    int client_socket
};


#endif //LEARN_SOCKET_UDP_H
