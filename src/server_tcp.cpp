#include"server_tcp.h"


ServerTCP::ServerTCP(std::unique_ptr<Library> lib):
        Server(std::move(lib))
    {};

void ServerTCP::init_socket(int port)
    {
    	if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            std::cout<<"Create socket FD error"<<std::endl;
            assert(false);
        }

        m_address.sin_family = AF_INET;
        m_address.sin_addr.s_addr = INADDR_ANY;
        m_address.sin_port = htons(port);

        if (bind(m_server_fd, (sockaddr*)&m_address, sizeof(m_address)) < 0)
        {
            std::cout<<"bind error"<<std::endl;
            assert(false);
        }

        if (listen(m_server_fd, 1) < 0)
        {
            std::cout<<"listen error"<<std::endl;
            assert(false);
        }
    };

void ServerTCP::run()
    {
        int client_socket;
        int addr_len = sizeof(m_address);
        if ((client_socket = accept(m_server_fd, (sockaddr*)&m_address, (socklen_t*)&addr_len)) < 0)
        {
            std::cout<<"Client connect error"<<std::endl;
            assert(false);
        }

        int readval = 0;
        int buffer_size = 1024;
        char buffer[1024]{0};
        std::string msg = "";

        while (true)
        {
            readval = read(client_socket, buffer, buffer_size);
            if (readval == 0)
            {
                sleep(0.1);
            }
            
            for (int i=0; i<readval; i++)
            {   
                std::cout<<buffer[i];
                if (buffer[i] == '\v')
                {
                    std::string resp = proc_msg(msg);
                    std::cout<<"resp:"<<std::endl<<resp<<std::endl;
                    send(client_socket, resp.c_str(), resp.length(), 0);
                    msg = "";
                    continue;
                }

                msg += buffer[i];
            }
        }
    };
