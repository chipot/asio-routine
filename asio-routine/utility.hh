#pragma once
#ifndef UTILITY_SL2TQCCM
#define UTILITY_SL2TQCCM

#include "asio-routine/asiofwd.hh"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <string>

namespace tnet {

ip::address
operator "" _addr(char const *addr, size_t len);
  
} /* tnet */


namespace std {

template<typename T>
struct move_on_copy
{
    mutable T value;

    move_on_copy(T&& t)
        : value(std::move(t))
    {}

    move_on_copy(move_on_copy const& other)
        : value(std::move(other.value))
    {}

    move_on_copy(move_on_copy&& other)
        : value(std::move(other.value))
    {}

    move_on_copy& operator=(move_on_copy const& other)
    {
        value = std::move(other.value);
        return *this;
    }

    move_on_copy& operator=(move_on_copy&& other)
    {
        value = std::move(other.value);
        return *this;
    }
};

std::string
to_string(udp::socket const &addr);

std::string
to_string(tcp::socket const &addr);
  
} /* std */

#endif /* end of include guard: UTILITY_SL2TQCCM */
