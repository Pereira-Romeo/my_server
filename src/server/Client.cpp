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
}


//================= buffer management ============================//

std::string Client::getInput()
{
    std::string tmp(_inBuffer);
    _inBuffer.clear();
    return tmp;
}

void Client::saveInput(std::string in)
{
    _inBuffer.append(in);
}

void Client::addOutput(std::string out)
{
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find_first_not_of(" \t\r") == std::string::npos)
            continue;
        _outBuffer.append(line + '\n');
    }
    polloutActivator();
}

void Client::addOutput(std::ostringstream out)
{
    addOutput(out.str());
}

void Client::pushOutput(std::string out)
{
    out.append(_outBuffer);
    _outBuffer = out;
}

std::string Client::getOutput()
{
    std::string out("");
    size_t eolc = _outBuffer.find_last_of('\n');

    if (eolc != std::string::npos) {
        out.append(_outBuffer.begin(), _outBuffer.begin() + eolc + 1);
        _outBuffer.erase(_outBuffer.begin(), _outBuffer.begin() + eolc + 1);
    }
    polloutActivator();
    return out;
}

void Client::polloutActivator()
{
    if (_outBuffer.find('\n') != std::string::npos)
        _getPfd(_fd).events |= POLLOUT;
    else
        _getPfd(_fd).events &= ~POLLOUT;
}


//================= Misc =========================================//

sockaddr_in Client::getNetInfo() const noexcept
{
    return _addr;
}

int Client::fd() const noexcept
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
