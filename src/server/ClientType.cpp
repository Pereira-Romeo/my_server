/*
** Deb PROJECT, 2026
** myhttp
** File description:
** ClientType
*/

#include "server/ClientType.hpp"

namespace myhttp
{
std::ostream& operator<<(std::ostream& out, const ClientType& type)
{
    static const std::string _toStr[(uint8_t) ClientType::count] = {
        "none"
    };

    out << _toStr[(uint8_t) type];
    return out;
}
} // namespace myhttp

