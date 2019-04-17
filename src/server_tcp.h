#pragma once
#include"server.h"
#include"memory"
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>


class ServerTCP: public Server
{
    public:
    ServerTCP(std::unique_ptr<Library> lib);

    void init_socket(int port);

    void run();
    
    ~ServerTCP() = default;
};
