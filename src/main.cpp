#include"storage.h"
#include"library.h"
#include"server_tcp.h"
#include"file_storage.h"
#include"parser.h"
#include"xml_parser.h"
#include<memory>
#include<csignal>
#include <iostream>


std::unique_ptr<Library> lib {new Library(std::unique_ptr<Storage>(new FileStorage(std::unique_ptr<Parser>(new XmlParser()), "FileStore.xml")))};
ServerTCP serv(std::move(lib));

void signal_handler(int signal)
{
    std::cout<<"terminate"<<std::endl;
    serv.stop();
}

int main()
{
    signal(SIGTERM, signal_handler);
    serv.init_socket(8080);
    serv.run();
    
    return 0;
}
