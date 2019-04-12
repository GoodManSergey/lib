#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<cstring>
#include<arpa/inet.h>
#include<iostream>
#include<string>


int main()
{
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -1;
    }
    //valread = read( sock , buffer, 1024);
    std::string msg1 = "{\"command\":\"add_author\", \"data\":{\"name\":\"NEW AYTHOR\"}}\v";
	std::string msg2 = "{\"command\":\"get_author_by_id\", \"data\":{\"id\":1}}\v";
	std::string msg3 = "{\"command\":\"change_author\", \"data\":{\"id\":3, \"name\": \"new author name\"}}\v";
	std::string msg4 = "{\"command\":\"delete_author_by_id\", \"data\":{\"id\":3}}\v";
	std::string msg5 = "{\"command\":\"add_book\", \"data\":{\"author_id\":1, \"title\":\"NEW BOOK\"}}\v";
	std::string msg6 = "{\"command\":\"get_book\", \"data\":{\"id\":1}}\v";
	std::string msg7 = "{\"command\":\"change_book\", \"data\":{\"author_id\":1, \"title\":\"book\", \"id\":3}}\v";
	std::string msg8 = "{\"command\":\"delete_book\", \"data\":{\"id\":3}}\v";
	std::string msg9 = "{\"command\":\"get_all_author_books\", \"data\":{\"id\":1}}\v";
    send(sock, msg9.c_str(), msg9.length(), 0);
    int readval = 0;
    int buffer_size = 1024;
    sleep(1);
    readval = read(sock, buffer, buffer_size);
    printf("%s\n",buffer );
    return 0;
}
