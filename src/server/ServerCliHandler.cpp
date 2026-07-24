/*
** Deb PROJECT, 2026
** myhttp
** File description:
** ServerCliHandler
*/

#include "server/Server.hpp"


namespace myhttp
{
//================= cli interface ================================//

typedef struct cliCmd_s {
    std::function<int (std::deque<std::string>)> func;
    std::string description;
} cliCmd_t;


int Server::sinHandler()
{
    if (!(_pfds[1].revents & POLLIN)) {
        return 0;
    }

    /** list of available commands, corresponding function and description */
    static const std::map<std::string, cliCmd_t> cmds = {
        {"status", {[this](std::deque<std::string>) -> int {std::cout << *this << std::endl; return 0;}, "get information about the server's state."}},
        {"reload", {[this](std::deque<std::string>) -> int {std::cout << "not implemented yet" << std::endl; return 0;}, "reload server's config file."}},
        {"list", {[this](std::deque<std::string> args) -> int {return this->listCmd(args);}, "list something (clients)"}}
    };

    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    std::string arg;
    std::deque<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
        arg.clear();
    }

    if (cmd == "help") {
        for (auto& _cmd : cmds) {
            std::cout << "    \033[1m" << std::left << std::setw(12) << _cmd.first << std::setw(0) << "\033[0m" + _cmd.second.description << "\n";
        }
        std::cout << std::endl;
    } else if (cmd.size() > 0) {
        auto it = cmds.find(cmd);
        if (it == cmds.end()) {
            std::cerr << "Unknown command '" + cmd  + "'." << std::endl;
        } else {
            int ret = it->second.func(args);
            std::cout << "\033[34;1m$?:\033[0m " << ret << std::endl;
        }
    }
    return 1;
}


//================= cli commands =================================//

int Server::listCmd(std::deque<std::string> args)
{
    if (args.size() < 1) {
        std::cout << "list: need atleast 1 argument, can be 'client'." << std::endl;
        return 1;
    }

    for (std::string elt : args) {
        if (elt == "client" || elt == "clients") {
            for (auto& client : _clients) {
                std::cout << client.second << "\n";
            }
        } else {
            std::cout << "list: Unknow argument '" + elt + "'." << "\n";
        }
        std::cout << std::endl;
    }
    return 0;
}


} // namespace myhttp
