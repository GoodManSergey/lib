#include"storage.h"
#include"library.h"
#include"server_tcp.h"
#include"file_storage.h"
#include"parser.h"
#include"xml_parser.h"
#include<memory>


int main()
{
    std::unique_ptr<Library> lib {new Library(std::unique_ptr<Storage>(new FileStorage(std::unique_ptr<Parser>(new XmlParser()), "FileStore.xml")))};
    ServerTCP serv(std::move(lib));
    serv.init_socket(8080);
    serv.run();
    
    return 0;
}
