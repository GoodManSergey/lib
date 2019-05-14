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
    /*
     * TODO зачем передавать в сервер неинициализированный сокет? Пусть создаёт его сам (например в методе init_socket).
     */
    sock = std::make_unique<SocketTcp>();
    serv = std::make_unique<Server>(std::move(lib), std::move(sock));
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    serv->init_socket(8080);
    serv->run();
    
    return 0;
}
