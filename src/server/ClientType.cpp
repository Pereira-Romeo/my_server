/*
** Deb PROJECT, 2026
** myServer
** File description:
** ClientType
*/

#include "server/ClientType.hpp"

namespace my
{
std::ostream& operator<<(std::ostream& out, const ClientType& type)
{
    static const std::string _toStr[(uint8_t) ClientType::count] = {
        "none",
        "http"
    };

    out << _toStr[(uint8_t) type];
    return out;
}
} // namespace my
