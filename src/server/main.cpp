#include "../lib/storage.h"
#include "../lib/library.h"
#include "../lib/file_storage.h"
#include "../lib/parser.h"
#include "../lib/xml_parser.h"
#include <memory>
#include <csignal>
#include <iostream>
#include "../lib/socket.h"
#include "../lib/socket_tcp.h"
#include "server.h"
#include "../lib/result_code.h"


std::unique_ptr<Server> serv;

void signal_handler(int signal)
{
    std::cout<<"terminate"<<std::endl;
    serv->stop();
}

int main()
{
    std::unique_ptr<Library> lib;
    std::unique_ptr<Socket> sock;
    auto parser = std::make_unique<XmlParser>();
    auto storage = std::make_unique<FileStorage>(std::move(parser), "FileStore.xml");
    lib = std::make_unique<Library>(std::move(storage));
    sock = std::make_unique<SocketTcp>();
    auto sock_bind_res = sock->bind_socket(8080);
    if (sock_bind_res != result_code::OK)
    {
        std::cout<<"sock init error"<<std::endl;
        exit(-1);
    }

    serv = std::make_unique<Server>(std::move(lib), std::move(sock));
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    serv->run();
    
    return 0;
}
