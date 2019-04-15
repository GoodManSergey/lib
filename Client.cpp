#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<cstring>
#include<arpa/inet.h>
#include<iostream>
#include<string>
#include<assert.h>
#include"jsoncpp/json/json.h"


class Client
{	
	public:
	virtual void init(int server_port, const std::string& server_host) = 0;
	virtual std::string get_answer() = 0;
	virtual void send_msg(const std::string& msg) = 0;
	
	std::string json_to_string(const Json::Value& json)
	{
		Json::FastWriter fastWriter;
        std::string output = fastWriter.write(json);

        return std::move(output + "\v");
	}
	
	std::string add_author_msg(const std::string& name)
	{
		Json::Value root;
		Json::Value data;
		data["name"] = name;
		root["command"] = std::move("add_author");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string get_author_msg(int id)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		root["command"] = std::move("get_author_by_id");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string change_author_msg(int id, const std::string& name)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		data["name"] = name;
		root["command"] = std::move("change_author");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string delete_author_msg(int id)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		root["command"] = std::move("delete_author_by_id");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string add_book_msg(int author_id, const std::string& title)
	{
		Json::Value root;
		Json::Value data;
		data["author_id"] = author_id;
		data["title"] = title;
		root["command"] = std::move("add_book");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string get_book_msg(int id)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		root["command"] = std::move("get_book");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string change_book_msg(int id, int author_id, const std::string& title)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		data["author_id"] = author_id;
		data["title"] = title;
		root["command"] = std::move("change_book");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string delete_book_msg(int id)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		root["command"] = std::move("delete_book");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
	std::string get_all_author_books_msg(int id)
	{
		Json::Value root;
		Json::Value data;
		data["id"] = id;
		root["command"] = std::move("get_all_author_books");
		root["data"] = std::move(data);
		
		return std::move(json_to_string(root));
	}
	
};


class ClientTCP: public Client
{
	public:
	void init(int server_port, const std::string& server_host)
	{
		if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{	
    		std::cout<<"sock init"<<std::endl;
        	assert(false);
    	}
    	
    	memset(&m_serv_addr, '0', sizeof(m_serv_addr));
    	
    	m_serv_addr.sin_family = AF_INET;
    	m_serv_addr.sin_port = htons(server_port);
    	
    	if(inet_pton(AF_INET, server_host.c_str(), &m_serv_addr.sin_addr)<=0)
    	{	
    		std::cout<<"addr init"<<std::endl;
        	assert(false);
    	}
    	
    	if (connect(m_sock, (sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0)
    	{
    		std::cout<<"aconnect"<<std::endl;
        	assert(false);
    	}
	}
	
	void send_msg(const std::string& msg)
	{
		send(m_sock, msg.c_str(), msg.length(), 0);
	}
	
	std::string get_answer()
	{
		int readval = 0;
        int buffer_size = 1024;
        char buffer[1024]{0};
        std::string msg = "";

        while (true)
        {
        	sleep(0.1);
            readval = read(m_sock, buffer, buffer_size);

            for (int i=0; i<readval; i++)
            {	
                if (buffer[i] == '\v')
                {
                    return std::move(msg);
                }

                msg += buffer[i];
            }
        }
	}
	
	private:
	int m_sock;
	sockaddr_in m_serv_addr;
	
};


class ClientUDP: public Client
{
	public:
	void init(int server_port, const std::string& server_host)
	{
		if ((m_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{	
    		std::cout<<"sock init"<<std::endl;
        	assert(false);
    	}
    	
    	memset(&m_serv_addr, '0', sizeof(m_serv_addr));
    	
    	m_serv_addr.sin_family = AF_INET;
    	m_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	m_serv_addr.sin_port = htons(server_port);
    	//m_serv_addr.sin_addr.s_addr = INADDR_ANY;
	}
	
	void send_msg(const std::string& msg)
	{
		sendto(m_sock, msg.c_str(), msg.length(), MSG_CONFIRM, (sockaddr*)&m_serv_addr, sizeof(m_serv_addr));
	}
	
	std::string get_answer()
	{
		int readval = 0;
		socklen_t len_from;
        int buffer_size = 1024;
        char buffer[1024]{0};
        std::string msg = "";

        while (true)
        {
            readval = recvfrom(m_sock, (char *)buffer, buffer_size, MSG_WAITALL, (sockaddr *)&m_serv_addr, &len_from);

            for (int i=0; i<readval; i++)
            {	
                if (buffer[i] == '\v')
                {
                    return std::move(msg);
                }

                msg += buffer[i];
            }
        }
	}
	
	private:
	int m_sock;
	sockaddr_in m_serv_addr;
};


int main()
{
    ClientUDP client;
    
    client.init(8080, "127.0.0.1");
    
    while (true)
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
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		case 2:
    		{
    			int id;
    			std::cout<<"Enter author id"<<std::endl;
    			std::cin>>id;
    			std::string msg = client.get_author_msg(id);
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
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
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		case 4:
    		{
    			int id;
    			std::cout<<"Enter author id"<<std::endl;
    			std::cin>>id;
    			std::string msg = client.delete_author_msg(id);
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
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
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		case 6:
    		{
    			int id;
    			std::cout<<"Enter book id"<<std::endl;
    			std::cin>>id;
    			std::string msg = client.get_book_msg(id);
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
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
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		case 8:
    		{
    			int id;
    			std::cout<<"Enter book id"<<std::endl;
    			std::cin>>id;
    			std::string msg = client.delete_book_msg(id);
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		case 9:
    		{
    			int id;
    			std::cout<<"Enter author id"<<std::endl;
    			std::cin>>id;
    			std::string msg = client.get_all_author_books_msg(id);
    			client.send_msg(msg);
    			std::string answer = client.get_answer();
    			std::cout<<"answer from server:"<<std::endl<<answer<<std::endl;
    			break;
    		}
    		default:
    			std::cout<<"bad choise"<<std::endl;
    	}
    }
    return 0;
}
