//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_ADDRESS_H
#define LEARN_ADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>


struct address
{
    address() = default;

    address(const sockaddr_in& address):
            m_address(address)
    {}

    sockaddr_in m_address;

};

#endif //LEARN_ADDRESS_H
