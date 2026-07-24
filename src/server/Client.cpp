/*
** Deb PROJECT, 2026
** myhttp
** File description:
** Client
*/

#include "server/Client.hpp"

namespace myhttp
{
//================= Exceptions ===================================//



//================= Constructor / Destructor =====================//

Client::Client(sockaddr_in info, int fd, std::function<pollfd& (int)> getPfd)
: _addr(info), _fd(fd), _getPfd(getPfd)
{
    std::cout << "Client " << *this << " created." << std::endl;
}


//================= Misc =========================================//

sockaddr_in Client::getNetInfo() const noexcept
{
    return _addr;
}

int Client::getFd() const noexcept
{
    return _fd;
}


//================= operators ====================================//

std::ostream& operator<<(std::ostream& out, const myhttp::Client& client)
{
    out << "{"
    << &client << ", "
    << client._addr << ", "
    << "fd: "  << client._fd << ", "
    << "type: "  << client.type
    << "}";
    return out;
}

} // namespace myhttp

std::ostream& operator<<(std::ostream& out, const sockaddr_in& addr)
{
    out << "{"
    << "family: " << addr.sin_family << ", "
    << "port: " << ntohs(addr.sin_port) << ", "
    << "ipv4: " << inet_ntoa(addr.sin_addr) << "}";
    return out;
}