#ifndef LEARN_SOCKET_H
#define LEARN_SOCKET_H

#include "result_code.h"
#include "address.h"
#include "result.h"
#include <string>
#include <memory>
#include "message.h"


/*
 * TODO: Интерфейс, на мой взгляд, получился слегка сложным по идее должно хватать функций: bind, accept, connect,
 * recv_msg и send_msg, остальное это уже детали реализации которых в интерфейсе быть не должно, это не то чтобы плохо
 * просто для человека который соберётся ре-использовать этот класс и увидит его впервые будет очень легко запутаться в
 * каком месте и когда вызывать конкретные функции, в то время как указанная выше пятёрка говорит сама за себя.
 */

class Socket
{
public:
    Socket() = default;
    virtual result_code bind_socket(int port) = 0;
    virtual result_code connect_socket(const std::string& host, int port) = 0;
    virtual result<std::shared_ptr<Socket>> accept_socket() = 0;
    virtual message recv_msg() = 0;
    virtual address get_address() = 0;
    virtual result_code send_msg(message& msg) = 0;
    virtual ~Socket() = default;
};


#endif //LEARN_SOCKET_H
