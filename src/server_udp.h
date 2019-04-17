#pragma once
#include"server.h"
#include"memory"
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<cstring>

class ServerUDP: public Server
{
    public:
    ServerUDP(std::unique_ptr<Library> lib);

    void init_socket(int port);

    void run();

    ~ServerUDP() = default;
};
