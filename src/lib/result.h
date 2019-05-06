#pragma once
#include"result_code.h"

template<typename T>
struct result
{
    result(result_code code):
        m_code(code)
    {}

    result(const T& object):
        m_code(result_code::OK),
        m_object(object)
    {}

    operator bool()
    {
        return m_code == result_code::OK;
    }

    result_code m_code;
    T m_object;
};
