/*
** Deb PROJECT, 2026
** myhttp
** File description:
** ServerCliHandler
*/

#include "server/Server.hpp"
#include "ColorsShortcut.hpp"

static inline
bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

static
std::deque<std::string_view> tokenize(std::string_view line)
{
    std::deque<std::string_view> tokens;
    size_t i = 0, n = line.size();

    while (i < n) {
        // skip whitespace
        for (; i < n && isSpace(line[i]); i++) {};
        if (i >= n) break;

        if (line[i] == '"' || line[i] == '\'') {
            char q = line[i];
            size_t start = ++i; // skip opening quote
            while (i < n && line[i] != q) ++i;
            tokens.push_back(line.substr(start, i - start));
            if (i < n) ++i; // skip closing quote
        } else {
            size_t start = i;
            while (i < n && !isSpace(line[i])) ++i;
            tokens.push_back(line.substr(start, i - start));
        }
    }

    return tokens;
}

namespace myhttp
{
//================= cli interface ================================//

typedef struct cliCmd_s {
    std::function<int (std::deque<std::string_view>&)> func;
    std::string description;
} cliCmd_t;


int Server::sinHandler() noexcept
{
    if (!(_pfds[1].revents & POLLIN)) {
        return 0;
    }

    try {
        /** list of available commands, corresponding function and description
         * @note std::less<> allows comparisons with string_view, just look up "heterogeneous look up in containers c++"
         */
        static const std::map<std::string, cliCmd_t, std::less<>> cmds = {
            {"status", {[this](std::deque<std::string_view>&) -> int {std::cout << *this << std::endl; return 0;}, "get information about the server's state."}},
            {"reload", {[this](std::deque<std::string_view>&) -> int {std::cout << "not implemented yet" << std::endl; return 0;}, "reload server's config file."}},
            {"list", {[this](std::deque<std::string_view>& args) -> int {return this->listCmd(args);}, "list something (clients)."}},
            {"client", {[this](std::deque<std::string_view>& args) -> int {return this->clientCmd(args);}, "get information about one or more clients."}},
            {"send", {[this](std::deque<std::string_view>& args) -> int {return this->sendCmd(args);}, "send information to a client."}}
            //command to hangup on specific clients (through fd)? (if fd index in pfds < 3 refuse the command) (flag to chose not to notify the client that we're hanging up ? add a bool markedForTermination so sending a string checks this and if true hangs up)
        };

        std::string line;
        line.reserve(256);
        std::getline(std::cin, line);

        std::deque<std::string_view> args = tokenize(line);
        if (args.size() == 0) {
            return 1;
        }
        std::string_view cmd = args.front();
        args.pop_front();

        if (cmd == "help") {
            for (auto& _cmd : cmds) {
                std::cout << CSBOLD << std::left << std::setw(12) << _cmd.first << std::setw(0) << CSRESET << _cmd.second.description << "\n";
            }
            std::cout << std::endl;
        } else if (cmd.size() > 0) {
            auto it = cmds.find(cmd);
            if (it == cmds.end()) {
                std::cerr << "Unknown command '" << cmd  << "'." << std::endl;
            } else {
                int ret = it->second.func(args);
                std::cout << CSINFO "$?" CSRB ": " CSRESET << ret << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << CSWARNL "sinHandler failure: " << e.what() << "." << std::endl;
    } catch (...) {
        std::cerr << CSWARNL "sinHandler failure: reason unkown." << std::endl;
    }
    return 1;
}


//================= cli commands =================================//

int Server::listCmd(std::deque<std::string_view>& args)
{
    if (args.size() < 1) {
        std::cout << "list: need atleast 1 argument, can be 'client'." << std::endl;
        return 1;
    }

    for (std::string_view elt : args) {
        if (elt == "client" || elt == "clients") {
            for (auto& client : _clients) {
                std::cout << client.second << "\n";
            }
        } else {
            std::cout << "list: Unknow argument '" << elt << "'." << "\n";
        }
        std::cout << std::endl;
    }
    return 0;
}

int Server::clientCmd(std::deque<std::string_view>& args)
{
    if (args.size() < 1) {
        std::cout << "client: need atleast 1 argument, can be a client's fd (int)." << std::endl;
        return 1;
    }

    for (std::string_view elt : args) {
        int fd = 0;
        auto result = std::from_chars(elt.data(), elt.data() + elt.size(), fd);
        if (result.ec != std::errc{} || result.ptr != elt.data() + elt.size()) {
            std::cout << "client: argument '" << elt << "' is not a valid client fd." << "\n";
        } else if (_clients.find(fd) != _clients.end()) {
            std::cout << _clients.at(fd) << "\n";
        } else {
            std::cout << "client: " << elt << " is not a client's fd." << "\n";
        }
        std::cout << std::endl;
    }
    return 0;
}

int Server::sendCmd(std::deque<std::string_view>& args)
{
    if (args.size() != 2) {
        std::cout << "send: need 2 arguments, must be a client's fd (int) followed by the text to send." << std::endl;
        return 1;
    }

    int fd = 0;
    auto result = std::from_chars(args[0].data(), args[0].data() + args[0].size(), fd);
    if (result.ec != std::errc{} || result.ptr != args[0].data() + args[0].size()) {
        std::cout << "client: argument '" << args[0] << "' is not a valid client fd." << "\n";
    } else if (_clients.find(fd) != _clients.end()) {
        _clients.at(fd).addOutput(static_cast<std::string>(args[1]));
    } else {
        std::cout << "client: " << args[0] << " is not a client's fd." << "\n";
    }
    std::cout << std::endl;
    return 0;
}


} // namespace myhttp
