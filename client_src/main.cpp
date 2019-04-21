#include "client_tcp.h"
#include <thread>


int main()
{
    ClientTCP client;

    client.init(8080, "127.0.0.1");

    std::thread read_write(&ClientTCP::read_write, &client);

    bool exit = false;
    while (!exit)
    {
        std::cout<<"1.Add author"<<std::endl;
        std::cout<<"2.Get author"<<std::endl;
        std::cout<<"3.Change author"<<std::endl;
        std::cout<<"4.Delete author"<<std::endl;
        std::cout<<"5.Add book"<<std::endl;
        std::cout<<"6.Get book"<<std::endl;
        std::cout<<"7.Change book"<<std::endl;
        std::cout<<"8.Delete book"<<std::endl;
        std::cout<<"9.Get all author books"<<std::endl;

        int choise = 0;

        std::cin>>choise;

        switch(choise)
        {
            case 1:
            {
                std::string name;
                std::cout<<"Enter author name"<<std::endl;
                std::cin>>name;
                std::string msg = client.add_author_msg(name);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 2:
            {
                int id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>id;
                std::string msg = client.get_author_msg(id);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 3:
            {
                int id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>id;
                std::string name;
                std::cout<<"Enter author name"<<std::endl;
                std::cin>>name;
                std::string msg = client.change_author_msg(id, name);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 4:
            {
                int id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>id;
                std::string msg = client.delete_author_msg(id);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 5:
            {
                int author_id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>author_id;
                std::string title;
                std::cout<<"Enter book title"<<std::endl;
                std::cin>>title;
                std::string msg = client.add_book_msg(author_id, title);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 6:
            {
                int id;
                std::cout<<"Enter book id"<<std::endl;
                std::cin>>id;
                std::string msg = client.get_book_msg(id);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 7:
            {
                int id;
                std::cout<<"Enter book id"<<std::endl;
                std::cin>>id;
                int author_id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>author_id;
                std::string title;
                std::cout<<"Enter book title"<<std::endl;
                std::cin>>title;
                std::string msg = client.change_book_msg(id, author_id, title);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 8:
            {
                int id;
                std::cout<<"Enter book id"<<std::endl;
                std::cin>>id;
                std::string msg = client.delete_book_msg(id);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            case 9:
            {
                int id;
                std::cout<<"Enter author id"<<std::endl;
                std::cin>>id;
                std::string msg = client.get_all_author_books_msg(id);
                client.send_to_queue(std::move(msg));
                std::string answer = client.get_response();
                std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
                break;
            }
            default:
                std::cout<<"bad choise"<<std::endl;
                exit = true;
                break;

        }
    }
    read_write.join();
    return 0;
}

