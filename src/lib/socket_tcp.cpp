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

    int flags;
    /*const int opt = 1;
    auto opt_size = sizeof(opt);
    setsockopt(m_socket, SOL_TCP, TCP_NODELAY, &opt, opt_size);
    setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &opt, opt_size);
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, opt_size);*/
    flags = fcntl(m_socket, F_GETFL, 0);
    if (flags != -1)
    {
        std::cout<<"non-block error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);

    return result_code::OK;
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

result_code SocketTcp::connect_socket(const std::string& host, int port)
{
    auto create_res = this->create_socket_fd();
    if (create_res != result_code::OK)
    {
        return create_res;
    }

    auto fill_addr_res = this->fill_addr(port);
    if (fill_addr_res != result_code::OK)
    {
        return fill_addr_res;
    }

    auto set_rm_addr_res = this->set_remote_addr(host);
    if (set_rm_addr_res != result_code::OK)
    {
        return set_rm_addr_res;
    }

    if (connect(m_socket, (sockaddr *)&m_address, sizeof(m_address)) < 0)
    {
        std::cout<<"connect"<<std::endl;
        return result_code::SOCKET_ERROR;
    }

    return result_code::OK;
}

result<std::shared_ptr<Socket>> SocketTcp::accept_socket()
{
    int client_socket;
    int addr_size = sizeof(m_address);
    if ((client_socket = accept(m_socket, (sockaddr*)&m_address, (socklen_t*)&addr_size)) < 0)
    {
        std::cout<<"Client connect error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }
    return std::static_pointer_cast<Socket>(std::make_shared<SocketTcp>(client_socket));
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

result_code SocketTcp::bind_socket(int port)
{
    auto create_res = this->create_socket_fd();
    if(create_res != result_code::OK)
    {
        return create_res;
    }

    auto fill_addr_res = this->fill_addr(port);
    if(fill_addr_res != result_code::OK)
    {
        return fill_addr_res;
    }

    auto set_in_addr_res = this->set_in_addr();
    if (set_in_addr_res != result_code::OK)
    {
        return set_in_addr_res;
    }

    int addr_size = sizeof(m_address);
    if (bind(m_socket, (sockaddr*)&m_address, (socklen_t)addr_size) < 0)
    {
        std::cout<<"bind error"<<std::endl;
        return result_code::SOCKET_ERROR;
    }

    return this->listen_socket();
}

message SocketTcp::recv_msg()
{
    if (m_buffer_iter >= strlen(m_buffer))
    {
        m_buffer_iter = 0;
        int readval = 0;
        readval = read(m_socket, m_buffer, m_buffer_size);
    }

    for (;m_buffer_iter < strlen(m_buffer); m_buffer_iter++)
    {
        if (m_buffer[m_buffer_iter] == '\v')
        {
            std::string buf_msg = std::move(msg);
            msg = "";
            m_buffer_iter++;

            return std::move(buf_msg);
        }
        else
        {
            msg += m_buffer[m_buffer_iter];
        }
    }

    return std::move(std::string(""));
}

result_code SocketTcp::send_msg(message& msg)
{
    int current_pos = 0;
    int rc = 0;
    std::string send_msg = msg.m_data;

    while (send_msg.length() != current_pos)
    {
        rc = send(m_socket, send_msg.c_str() + current_pos, send_msg.length() - current_pos, 0);
        if(rc < 0)
        {
            return result_code::SOCKET_ERROR;
        }
        else
        {
            current_pos += rc;
            if (rc == 0)
            {
                sleep(1);
            }
        }
    }

    return result_code::OK;
}

SocketTcp::~SocketTcp()
{
    close(m_socket);
}

address SocketTcp::get_address()
{
    address sock_addr;
    return sock_addr;
}