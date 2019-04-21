#include "client_tcp.h"


void ClientTCP::init(int server_port, const std::string& server_host)
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
        std::cout<<"connect"<<std::endl;
        assert(false);
    }
};

void ClientTCP::read_write() {
    int readval = 0;
    int buffer_size = 1024;
    char buffer[1024]{0};
    std::string msg = "";

    while (true) {
        m_send_queue_mutex.lock();
        if (m_send_queue.size() > 0) {
            std::string send_msg = std::move(m_send_queue.front());
            m_send_queue.pop();
            send(m_sock, std::move(send_msg.c_str()), send_msg.length(), 0);
        }
        m_send_queue_mutex.unlock();


        readval = read(m_sock, buffer, buffer_size);
        if (readval == 0) {
            sleep(1);
        }

        for (int i = 0; i < readval; i++) {
            if (buffer[i] == '\v') {
                if (msg != "ping") {
                    m_response_mutex.lock();
                    m_has_response = true;
                    m_response = std::move(msg);
                    m_response_mutex.unlock();
                } else {
                    std::string send_msg("pong");
                    send(m_sock, std::move(send_msg.c_str()), send_msg.length(), 0);
                }
                msg = "";
            }

            msg += buffer[i];
        }
    }
};