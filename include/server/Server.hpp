/*
** Deb PROJECT, 2026
** myServer
** File description:
** Server
*/

#pragma once
//Clients
#include "./Client.hpp"
#include <unistd.h>

//storing pollfds
#include <vector>
#include <unordered_map> //pollfd lookup table

//cli
#include <map>
#include <iomanip>
#include <deque>
#include <charconv>

//signal handling
#include <signal.h>
#include <sys/signalfd.h>

//server config
#include "./Config.hpp"

namespace my
{
    class Server {
        public:
            Server() = delete;
            /** Server constructor
             * @param port port to listen on
             */
            Server(const std::string& port);
            ~Server();

            /** run the server's poll loop, the server runs until receiving a SIGINT */
            void run();

            /** get your corresponding pollfd&
             * @param fd your client's fd
             * @returns a pollfd reference
             * @exception std::out_of_range if fd not found
             */
            pollfd& getPfd(int fd);

            friend std::ostream& operator<<(std::ostream& out, const Server& server);

        private:
            bool _run = true;

            /** Server network info */
            sockaddr_in _addr;

            /** Server config */
            config_t conf;

            /** list of pfd
             * [0] is the signal fd
             * [1] is the cli (std::cin)
             * [2] is the listen socket
             * [n] is a Client
             */
            std::vector<pollfd> _pfds;
            /** pfd lookup table, key = fd, value = index inside _pfds, gives getPfd and complexity of O(1) */
            std::unordered_map<int, size_t> _pfdIndex;
            /** map of clients, key is the client's fd */
            std::unordered_map<int, Client> _clients;

            /** hang up a client based on it's fd */
            void hangUp(int fd) noexcept;


            //===== socket Handling ==============================//
            //in Server.cpp

            /** handle listen socket (new connections)
             * @returns the number of events done (0 or 1)
             */
            int listenSocketHandler() noexcept;

            /** add a new client to the server
             * @param fd the socket fd of the client
             * @param addr the network info of the client (filled by the accept call)
             * @return true if the client was successfully added. false if failed
             * @note this function does not handle any handshake
             */
            bool insertNewClient(int fd, const sockaddr_in& addr) noexcept;

            //from here is file ServerSocketHandler.cpp

            /** handle client sockets (and events)
             * @param n the number of poll events to complete
             * @returns the number of events done
             */
            int socketsHandler(int n) noexcept;

            /** handle client pollin
             * @param ci reference to the client's index inside pfd
             * @note client index will change if something requires the deletion of the client
             * @note (for example reading an input of size 0 is equivalent to a pollhup)
             */
            void clientPollin(size_t& ci, Client& client);

            /** handle client pollout
             * @param ci reference to the client's index inside pfd
             * @note client index will change if something requires the deletion of the client
             */
            void clientPollout(size_t& ci, Client& client);

            //===== cli interface ================================//
            //see also section "cli commands"

            /** handle terminal input
             * @returns the number of events done (0 or 1)
             */
            int sinHandler() noexcept;


            //===== signals ======================================//

            int _sigMask;
            /** makes a signal fd  with the requested flags
             * @returns fd of the file linked to signals
             */
            int makeSignalFd(int signals);
            /** signal handler
             * @returns the number of events done
             */
            int sigHandler() noexcept;


            //===== read ===========================================//

            /** read a certain size into a buffer, shouldn't partial read
             * @param fd fd to read from
             * @param buf buffer to write read data
             * @param size size of the buffer
             * @returns true if it read something of requested size
             */
            bool readAll(int fd, void *buf, size_t size) noexcept;
            /** read everything there is to read inside the fd
             * @param buf string to fill with data
             * @returns size of read data (number of characters read)
             */
            int readAll(int fd, std::string& buf);

            //===== cli commands =================================//

            /** list command
             * @param arg rest of the line
             * @note needs atleast one
             * @note available args:
             * @note - clients (or clients) = list clients
             */
            int listCmd(std::deque<std::string_view>& args);

            /** client command
             * @param arg rest of the line
             * @note needs atleast one
             * @note available args:
             * @note - fd => get info about the corresponding client
             */
            int clientCmd(std::deque<std::string_view>& args);

            /** send command
             * @param arg rest of the line
             * @note needs all in order
             * @note available args:
             * @note - fd => get info about the corresponding client
             * @note - text => text to send
             */
            int sendCmd(std::deque<std::string_view>& args);
    };
} // namespace my
