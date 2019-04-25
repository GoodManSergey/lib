//
// Created by kovalev_s on 25.04.19.
//

#include "socket_tcp.h"

result_code SocketTcp::create_socket_fd()
{
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout<<"Create socket FD error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::SOCKET_ERROR;
}

result_code SocketTcp::fill_addr(int port)
{
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(port);

    return result_code::OK;
}

result_code SocketTcp::set_in_addr()
{
    m_address.sin_addr.s_addr = INADDR_ANY;
    return result_code::OK;
}

result_code SocketTcp::set_remote_addr(const std::string& host)
{
    if(inet_pton(AF_INET, host.c_str(), &m_address.sin_addr)<=0)
    {
        std::cout<<"addr init"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::OK;
}

result_code SocketTcp::connect_socket()
{
    if (connect(m_socket, (sockaddr *)&m_address, sizeof(m_address)) < 0)
    {
        std::cout<<"connect"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::OK;
}

result<address> SocketTcp::accept_socket()
{
    int client_socket;
    int addr_size = sizeof(m_address);
    if ((client_socket = accept(m_socket, (sockaddr*)&m_address, (socklen_t*)&addr_size)) < 0)
    {
        std::cout<<"Client connect error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return std::move(address(std::move(client_socket)));
}

result_code SocketTcp::listen_socket()
{
    if (listen(m_socket, 1) < 0)
    {
        std::cout<<"listen error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::OK;
}

result_code SocketTcp::bind_socket()
{
    int addr_size = sizeof(m_address);
    if (bind(m_socket, (sockaddr*)&m_address, (socklen_t)addr_size) < 0)
    {
        std::cout<<"bind error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return result_code::OK;
}

std::string SocketTcp::recv_msg(address& socket_addr)
{
    int readval = 0;
    int buffer_size = 1024;
    char buffer[1024]{0};
    readval = read(socket_addr.m_fd, buffer, buffer_size);

    return std::move(buffer);
}

result_code SocketTcp::send_msg(std::string&& msg, address& socket_addr)
{
    int left;

    while (msg.length() != 0)
    {
        left = send(socket_addr.m_fd, msg.c_str(), msg.length(), 0);
        if(left < 0)
        {
            return result_code::SOCKET_ERROR;
        }
        else
        {
            msg.erase(0, left);
        }
    }

    return result_code::OK;
}

protocol SocketTcp::return_protocol()
{
    return protocol::TCP;
}