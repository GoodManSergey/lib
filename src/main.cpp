#include"storage.h"
#include"library.h"
#include"server_tcp.h"
#include"file_storage.h"
#include"parser.h"
#include"xml_parser.h"
#include<memory>
#include<csignal>
#include <iostream>


std::unique_ptr<Library> lib;
std::unique_ptr<Server> serv;

void signal_handler(int signal)
{
    std::cout<<"terminate"<<std::endl;
    serv->stop();
}

int main()
{
    auto parser = std::make_unique<XmlParser>();
    auto storage = std::make_unique<FileStorage>(std::move(parser), "FileStore.xml");
    lib = std::make_unique<Library>(std::move(storage));
    serv = std::make_unique<ServerTCP>(std::move(lib));
    signal(SIGTERM, signal_handler);
    serv->init_socket(8080);
    serv->run();
    
    return 0;
}
