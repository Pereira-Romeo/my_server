/*
** Deb PROJECT, 2026
** myServer
** File description:
** Server
*/

#include "server/Server.hpp"
#include "ColorsShortcut.hpp"

namespace my
{
//================= Exceptions ===================================//



//================= Constructor / Destructor =====================//


static
uint16_t readNVerifyPort(const std::string& port)
{
    int p;
    try {
        p = std::stoi(port);
    } catch (...) {
        throw my::Error("Port should be a number between 1 and 65 535.", false);
    }
    if (p < 1 || p > 65535) {
        throw my::Error("Port should be a number between 1 and 65 535.", false);
    }
    return static_cast<uint16_t>(p);
}

Server::Server(const std::string& port)
{
    //signal handling
    pollfd sigfd = {makeSignalFd(SIGINT), POLLIN, 0};
    _pfds.push_back(sigfd);


    //cli interface
    pollfd sin = {STDIN_FILENO, POLLIN, 0};
    _pfds.push_back(sin);

    //network
    uint16_t _port = readNVerifyPort(port);

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;

    pollfd listenSocket = {0, POLLIN, 0};
    if ((listenSocket.fd = socket(_addr.sin_family, SOCK_STREAM, 0)) == -1)
        throw my::Error(std::ostringstream("Couldn't open listening socket ") << _addr << "." , true);
    _pfds.push_back(listenSocket);

    if (bind(listenSocket.fd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
        throw my::Error(std::ostringstream("Couldn't bind listening socket to requested port ") << _addr << ".", true);

    if (listen(listenSocket.fd, 10) == -1)
        throw my::Error(std::ostringstream("Couldn't setup server for listening") << _addr << ".", true);

    std::cout << CSTS << " " CSINFOL "Server " << *this << " done creating." << std::endl;
}

Server::~Server()
{
    std::cout << CSTS << " " CSWARNL "Shutting down server..." << std::endl;
    for (pollfd pfd : _pfds) {
        close(pfd.fd);
    }
}

//================= run ==========================================//

void Server::run()
{
    while (_run) {
        int n = poll(_pfds.data(), _pfds.size(), -1); //number of events

        if (n > 0 && _run)
            n -= sigHandler();
        if (n > 0 && _run)
            n -= sinHandler();
        if (n > 0 && _run)
            n -= listenSocketHandler();
        if (n > 0 && _run)
            n -= socketsHandler(n);
    }
}

//================= socket Handling ==============================//

int Server::listenSocketHandler() noexcept
{
    if (!(_pfds[2].revents & POLLIN)) {
        return 0;
    }

    sockaddr_in newClientAddr;
    socklen_t scklen = sizeof(newClientAddr);
    int newSock = accept(_pfds[2].fd, (sockaddr *)&newClientAddr, &scklen);

    if (newSock == -1) {
        std::cerr << CSWARNL "Failed to accept incoming connection." << std::endl;
    } else {
        if (insertNewClient(newSock, newClientAddr)) { //&& debug lvl = info
            std::cout << CSINFOL "accepted new client: " << _clients.at(newSock) << std::endl;
            //handshake
        } else {
            close(newSock);
        }
    }
    return 1;
}

bool Server::insertNewClient(int fd, const sockaddr_in& addr) noexcept
{
    int stage = 0;

    try {
        _clients.emplace(std::piecewise_construct, std::forward_as_tuple(fd),
            std::forward_as_tuple(
                addr, fd, [this](int __fd) -> pollfd& {return getPfd(__fd);})
        ); //ugly line so only one client ever gets created :)
        stage = 1;
        _pfdIndex[fd] = _pfds.size();
        stage = 2;
        _pfds.push_back({fd, POLLIN | POLLHUP, 0});
        return true;

    } catch (std::exception& e) {
        std::cerr << CSWARNL "new client insertion failed! Reason: " << e.what() << "." << std::endl;
    } catch (...) {
        std::cerr << CSWARNL "new client insertion failed! Unknown reason." << std::endl;
    }
    //error recovery
    switch (stage) {
        case 2:
            _pfdIndex.erase(fd);
            [[fallthrough]]; //if stage 2 then stage 1 was reached
        case 1:
            _clients.erase(fd);
        default:
        break;
    }
    return false;
}


//================= pfd management / Client management ===========//

pollfd& Server::getPfd(int fd)
{
    return _pfds.at(_pfdIndex.at(fd));
}

void Server::hangUp(int fd) noexcept
{
    try {
        size_t index = _pfdIndex.at(fd); //get target index
        //update swapped item's index & move target to the back
        if (index != _pfds.size() - 1) {
            _pfdIndex[_pfds.back().fd] = index;
            std::swap(_pfds.at(index), _pfds.back());
        }
        //delete target
        _pfdIndex.erase(fd);
        _clients.erase(fd);
        close(fd);
        _pfds.pop_back();
    } catch (std::out_of_range& e) {
    }
}


//================= signals ======================================//

int Server::makeSignalFd(int signals)
{
    _sigMask = signals;
    sigset_t ss;
    if (sigemptyset(&ss) == -1 || sigaddset(&ss, signals) == -1)
        throw my::Error("Couldn't setup signalfd.", true);

    //block signals from reaching the program directly
    if (sigprocmask(SIG_SETMASK, &ss, NULL))
        throw my::Error("Couldn't setup signalfd.", true);

    int _sigIntFd = signalfd(-1, &ss, 0);
    if (_sigIntFd == -1)
        throw my::Error("Couldn't setup signalfd.", true);
    return _sigIntFd;
}

int Server::sigHandler() noexcept
{
    if (!(_pfds[0].revents & POLLIN)) {
        return 0;
    }

    struct signalfd_siginfo ssi;
    if (!readAll(_pfds[0].fd, &ssi, sizeof(ssi))) {
        std::cerr << "Error while trying to read from signalFd, reconstructing signalFd..." << std::endl;
        try {
            pollfd _sigpfd = {makeSignalFd(_sigMask), POLLIN, 0};
            close(_pfds[0].fd);
            _pfds[0] = _sigpfd;
            std::cerr << "Successfully reconstructed signalFd." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failure to reconstruct signalFd," << e.what() << ", skipping..." << std::endl;
        }
    } else {
        if (ssi.ssi_signo == SIGINT) {
            _run = false;
        }
    }
    return 1;
}


//================= read =========================================//

bool Server::readAll(int fd, void *buf, size_t size) noexcept
{
    size_t total = 0;
    uint8_t *ptr = static_cast<uint8_t *>(buf);

    while (total < size) {
        int val = read(fd, ptr + total, size - total);
        if (val == -1 && errno == EINTR)
            continue; // interrupted by signal, retry
        if (val == -1 || val == 0)
            return false;
        total += val;
    }
    return true;
}

int Server::readAll(int fd, std::string& buf)
{
    int total = 0;
    char buffer[4096] = {};
    int val = sizeof(buffer);

    while (val == (int)sizeof(buffer)) {
        val = read(fd, buffer, sizeof(buffer));
        if (val == -1 && errno == EINTR)
            continue;
        if (val == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            break;
        if (val <= 0)
            break;
        buf.append(buffer, val);
        total += val;
    }
    return total;
}


//================= operators ====================================//

std::ostream& operator<<(std::ostream& out, const my::Server& server)
{
    out << "{"
    << &server << ", "
    << server._addr << ", "
    << "pfds: "  << server._pfds.size() << ", "
    << "clients: " << server._clients.size()
    << "}";
    return out;
}

} // namespace my
