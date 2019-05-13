//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_ADDRESS_H
#define LEARN_ADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>


struct address
{
    address(const sockaddr_in& address, int fd):
            m_address(address),
            m_fd(fd)
    {}

    address()
    {}

    address(const sockaddr_in& address):
            m_address(address),
            m_fd(0)
    {}

    address(int fd):
            m_address({}),
            m_fd(fd)
    {}

    sockaddr_in m_address;
    /* TODO зачем здесь поле m_fd? */
    int m_fd;
};

#endif //LEARN_ADDRESS_H
