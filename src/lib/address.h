//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_ADDRESS_H
#define LEARN_ADDRESS_H

#include<sys/socket.h>


struct address
{
    address(const sockaddr_in& address, int fd):
            m_address(address),
            m_fd(fd)
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
    int m_fd;
};

#endif //LEARN_ADDRESS_H
