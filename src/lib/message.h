//
// Created by kovalev_s on 25.04.19.
//

#ifndef LEARN_MESSAGE_H
#define LEARN_MESSAGE_H

#include <experimental/optional>
#include <string>
#include "address.h"

struct message
{
    message(const std::string& msg):
    m_data(msg)
    {}

    message(const std::string& data, const address& sock_address):
    m_data(data),
    m_address(sock_address)
    {}

    std::string m_data;
    /*
     * TODO Зачем expermental? optional уже достаточное количество времени есть и просто в std::
     */
    std::experimental::optional<address> m_address;
};


#endif //LEARN_MESSAGE_H
