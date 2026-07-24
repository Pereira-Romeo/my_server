/*
** Deb PROJECT, 2026
** myhhtp
** File description:
** ClientType
*/

#pragma once
#include <cstdint>
#include <iostream>

namespace myhttp
{
    /** enum precising a client's type
     * You should modify this enum to you heart's content (just don't remove none)
     * it is used in the server's broadcast function if you want to broadcast only to a specific kind of clients
     */
    enum class ClientType : uint8_t {
        //no specific type
        none,

        //not a type, used to always have correct number of possible types
        count
    };

    std::ostream& operator<<(std::ostream& out, const ClientType& type);
} // namespace myhttp

