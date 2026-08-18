/*
** Deb PROJECT, 2026
** myhhtp
** File description:
** ClientType
*/

#pragma once
#include <cstdint>
#include <iostream>

namespace my
{
    /** enum precising a client's type
     * You should modify this enum to you heart's content (just don't remove none or count)
     * it is used by the server to know what protocol should it attempt to read first from a client
     */
    enum class ClientType : uint8_t {
        //no specific type
        none,
        /** the last transmission was of HTTP protocol
         * therefore the first start-line recognition tool called should be the one of an HTTP protocol
         */
        http,

        //not a type, used to always have correct number of possible types
        count
    };

    std::ostream& operator<<(std::ostream& out, const ClientType& type);
} // namespace my
