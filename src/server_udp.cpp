#include"server_udp.h"


ServerUDP::ServerUDP(std::unique_ptr<Library> lib):
    	Server(std::move(lib))
    {};

void ServerUDP::init_socket(int port)
    {
    	if ((m_server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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
    };

void ServerUDP::run()
    {
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));

        int addr_len = sizeof(m_address);

        int readval = 0;
        socklen_t len_from = sizeof(client_addr);
        int buffer_size = 1024;
        char buffer[1024]{0};
        std::string msg = "";

        while (true)
        {
            readval = recvfrom(m_server_fd, (char *)buffer, buffer_size, MSG_WAITALL, (sockaddr *)&client_addr, &len_from);
            std::cout << "port = " << ntohs(client_addr.sin_port) << std::endl;
            for (int i=0; i<readval; i++)
            {
            	std::cout<<buffer[i];
                if (buffer[i] == '\v')
                {
                    std::string resp = proc_msg(msg);
                    if (resp.length()>254)
                    {
                    	resp = "too large\v";
                    }
                    std::cout<<resp<<std::endl;
                    sendto(m_server_fd, resp.c_str(), resp.length(), 0, (sockaddr *)&client_addr, sizeof(client_addr));
                    std::cout<<std::strerror(errno)<<std::endl;
                    msg = "";
                    continue;
                }

                msg += buffer[i];
            }
        }
    };
