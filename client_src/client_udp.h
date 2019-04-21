#ifndef LIBRARY_CLIENT_UDP_H
#define LIBRARY_CLIENT_UDP_H

#include "client.h"
#include <iostream>
#include <mutex>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class ClientUDP: public Client
{
public:
    void init(int server_port, const std::string& server_host) override;

    void send_msg(const std::string& msg);

    std::string get_answer();

private:
    int m_sock;
    sockaddr_in m_serv_addr;

};


#endif //LIBRARY_CLIENT_UDP_H
