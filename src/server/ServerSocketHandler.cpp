/*
** EPITECH PROJECT, 2026
** myhttp
** File description:
** ServerSocketHandler
*/

#include "server/Server.hpp"
#include "ColorsShortcut.hpp"

namespace myhttp
{

int Server::socketsHandler(int n) noexcept
{
    int nDone = 0;

    std::cout << "starting socketsHandler loop" << "\n";
    for (size_t i = 3; i < _pfds.size() && nDone < n; i++) {
        if (_pfds[i].revents != 0) {
            nDone++;
            try {
                std::cout << "    Client " << _clients.at(_pfds[i].fd) << " has one event." << "\n";
                if (_pfds[i].revents & POLLIN) {
                    clientPollin(i, _clients.at(_pfds[i].fd));
                } else if (_pfds[i].revents & POLLOUT) {
                    clientPollout(i, _clients.at(_pfds[i].fd));
                } else if (_pfds[i].revents & POLLHUP) {
                    std::cout << CSINFOL "Client " << _clients.at(_pfds[i].fd) << " closed its connexion." << "\n";
                    hangUp(_pfds[i].fd);
                    i--;
                }
                continue;
            } catch (std::out_of_range& e) {
                std::cerr << CSWARNL "fd(" << _pfds[i].fd << ") is missing it's client information.";
            } catch (std::exception& e) {
                std::cerr << CSWARNL "fd(" << _pfds[i].fd << ") caused an unrecoverable issue '" << e.what() << "'.";
            } catch (...) {
                std::cerr << CSWARNL "fd(" << _pfds[i].fd << ") caused an unknown issue.";
            }
            std::cerr << " Killing corrupted fd(" << _pfds[i].fd << ")..." << std::endl;
            hangUp(_pfds[i].fd);
            i--;
        }
    }
    std::cout << "socketsHandler loop done" << std::endl;
    return nDone;
}

void Server::clientPollin(size_t& ci, Client& client)
{
    std::string in(client.getInput()); //retrieve buffered input

    try {
        if (readAll(client.fd(), in) == 0) {
            std::cout << CSINFOL "Client " << client << " closed its connexion." << "\n";
            hangUp(client.fd());
            ci--;
            return;
        }
    } catch (...) {
        //add counter inside client to mark it everytime it fails a read ?
        std::cout << CSWARNL "    Client " << client << " encountered an issue while reading a pollin." << "\n";
        return;
    }

    std::cout << "    Client " << client << " sent '" << in << "'." << "\n";

    // //deal with commands until none remaining
    // while (in.size() > 0) {
    //     std::string nextCmd;
    //     size_t eoc = in.find_first_of('\n');

    //     if (eoc != std::string::npos) {
    //         //this append naturally strips the \n
    //         nextCmd.append(in.begin(), in.begin() + eoc);
    //         in.erase(in.begin(), in.begin() + eoc + 1);
    //     } else { //command is partially written, buffer and wait next pollin
    //         cli.addInput(in);
    //         break;
    //     }

    //     if (cli._type == Client::none) {
    //         newConnectionHandler(i, cli, nextCmd);
    //     } else {
    //         if (clientPollin(i, cli, nextCmd) == false) {
    //             cli.addInput(nextCmd + '\n' + in);
    //             break;
    //         }
    //     }
    // }
}

void Server::clientPollout(size_t& ci, Client& client)
{
    std::string out = client.getOutput();

    if (out.size() > 1) {
        ssize_t val = write(client.fd(), out.c_str(), out.size());
        if (val == -1) {
            client.pushOutput(out);
            std::cerr << CSWARNL "write error with client(" << client << ")." << std::endl;
        } else if ((size_t) val < out.size() - 1) {
            out.erase(out.begin(), out.begin() + val);
            client.pushOutput(out);
        }
    }
}

} // namespace myhttp
