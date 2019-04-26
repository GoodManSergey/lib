//
// Created by kovalev_s on 25.04.19.
//

#include "socket_udp.h"


result_code SocketUdp::create_socket_fd()
{
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        std::cout<<"Create socket FD error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::SOCKET_ERROR;
}

result_code SocketUdp::fill_addr(int port)
{
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(port);

    return result_code::OK;
}

result_code SocketUdp::set_in_addr()
{
    m_address.sin_addr.s_addr = INADDR_ANY;
    return result_code::OK;
}

result_code SocketUdp::set_remote_addr(const std::string& host)
{
    m_address.sin_addr.s_addr = inet_addr(host.c_str());
    return result_code::OK;
}

result_code SocketUdp::connect_socket()
{
    return result_code::OK;
}

result<std::shared_ptr<Socket>> SocketUdp::accept_socket()
{
    return shared_from_this();
}

result_code SocketUdp::listen_socket()
{
    return result_code::OK;
}

result_code SocketUdp::bind_socket()
{
    int addr_size = sizeof(m_address);
    if (bind(m_socket, (sockaddr*)&m_address, (socklen_t)addr_size) < 0)
    {
        std::cout<<"bind error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::OK;
}

message SocketUdp::recv_msg()
{
    int readval = 0;
    int buffer_size = 1024;
    char buffer[1024]{0};
    address socket_addr;
    int size_of_addr = sizeof(socket_addr.m_address);

    readval = recvfrom(m_socket, (char *)buffer, buffer_size, MSG_WAITALL, (sockaddr *)&socket_addr.m_address,
            (socklen_t*)&size_of_addr);

    message msg(std::move(buffer), std::move(socket_addr));
    return std::move(msg);
}

result_code SocketUdp::send_msg(message&& msg)
{
    if (!msg.m_address)
    {
        return result_code::SOCKET_ERROR;
    }
    int size_of_addr = sizeof(msg.m_address);
    sendto(m_socket, msg.m_data.c_str(), msg.m_data.length(), 0, (sockaddr *)&msg.m_address, size_of_addr);
    return result_code::OK;
}

protocol SocketUdp::return_protocol()
{
    return protocol::UDP;
}

SocketUdp::~SocketUdp()
{
    close(m_socket);
}
