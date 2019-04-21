#include "client_udp.h"


void ClientUDP::init(int server_port, const std::string& server_host)
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
};

void ClientUDP::send_msg(const std::string& msg)
{
    sendto(m_sock, msg.c_str(), msg.length(), MSG_CONFIRM, (sockaddr*)&m_serv_addr, sizeof(m_serv_addr));
};

std::string ClientUDP::get_answer()
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
};