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
    flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1)
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

    for (int i = 0; i < 10; i++)
    {
        if (connect(m_socket, (sockaddr *) &m_address, sizeof(m_address)) >= 0)
        {
            return result_code::OK;
        }
    }

    std::cout << "connect error" << std::endl;
    return result_code::SOCKET_ERROR;
}

result<std::shared_ptr<Socket>> SocketTcp::accept_socket()
{
    int client_socket;
    int addr_size = sizeof(m_address);
    if ((client_socket = accept(m_socket, (sockaddr*)&m_address, (socklen_t*)&addr_size)) < 0)
    {
        //std::cout<<"Client connect error"<<std::endl;
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

/*
 * TODO не могу объяснить что именно не так, но стилистически какой-то сложный для восприятия метод получился, хотя вроде бы всё должно работать
 * (для примера я попробовал написать внизу этот же метод, мне кажется он вышел более прозрачным)
 * В твоей же реализации отсутствует одна важная достаточно вещь -- m_msg у тебя может увеличиваться бесконечно,
 * что может привести к выеданию приложением всей RAM если из сети мы будем получать какие-то левые данные (а сеть она такая, вдруг наше приложение будут атаковать злые хацкеры (уу-у-у))
 * Кроме того, ты вообще не смотришь на то что вернул read(), соотвественно никак не обрабатываешь ошибки возвращаемые им, это не очень правильно.
 */
message SocketTcp::recv_msg()
{
    std::cout<<m_buffer<<std::endl;
    std::cout<<"buffer iter: "<<m_buffer_iter<<std::endl;
    std::cout<<"buffer len: "<<strlen(m_buffer)<<std::endl;
    if (m_buffer_iter >= strlen(m_buffer))
    {
        m_buffer_iter = 0;
        int readval = 0;
        memset(m_buffer, 0, m_buffer_size);
        readval = recv(m_socket, m_buffer, m_buffer_size, MSG_DONTWAIT);
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
    memset(m_buffer, 0, sizeof(m_buffer));
    return std::move(std::string(""));
}


/*


std::deque<message> m_Messages;
std::vector<char> m_RecvBuffer(512);
size_t m_RecvBufferOffset = 0;

std::optional<message> SocketTcp::recv_msg_2() {
    if (!m_Messages.empty())
    {
        message msg = m_Messages.front();
        m_Messages.pop_front();
        return msg;
    }

    char buffer[512];
    int rc = read(m_socket, buffer, 512);
    if (rc <= 0) {
        // Обработка ошибок
    }

    size_t old_size = m_RecvBuffer.size();
    size_t desired_size = rc + m_RecvBufferOffset;
    if (desired_size > 1024 * 1024)
    {
        // нельзя увеличивать буффер бесконечно, нужно выбрать опеределённый (достаточно большой) лимит на размер
        // сообщения иначе из вне достаточно легко будет уронить приложение.
    }

    if (desired_size > old_size)
    {
        m_RecvBuffer.resize(rc + m_RecvBufferOffset);
    }

    memcpy(m_RecvBuffer.data() + m_RecvBufferOffset, buffer, rc);

    size_t offset = 0;
    for (size_t i = offset; i < m_RecvBuffer.size(); i++)
    {
        if (m_RecvBuffer[i] == '\v')
        {
            std::string message_string(m_RecvBuffer.begin() + offset, m_RecvBuffer.begin() + i + offset);
            m_Messages.push_back(message(message_string));

            offset = i;
        }
    }

    if (offset != 0)
    {
        int remains = m_RecvBuffer.size() - offset;
        memcpy(m_RecvBuffer.data(), m_RecvBuffer.data() + offset, remains);
        m_RecvBuffer.resize(remains);
    }

    if (!m_Messages.empty())
    {
        message msg = m_Messages.front();
        m_Messages.pop_front();
        return msg;
    }
    else
    {
        return std::optional<message>();
    }
}
*/

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