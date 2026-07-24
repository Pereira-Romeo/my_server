/*
** Deb PROJECT, 2026
** myhttp
** File description:
** Client
*/

#pragma once
#include "./ClientType.hpp"
#include "Error.hpp"

//Networking
#include <poll.h>       //poll & pollfd
#include <functional>   //Server functions callback
#include <netinet/in.h> //sockaddr_in struct
#include <arpa/inet.h>  //print network info

namespace myhttp
{
    /** Client class used in the server
     */
    class Client {
        public:
            Client() = delete;
            /** Client class constructor
             * @param info network information linked to the client
             * @param fd linked fd
             * @param getPfd server callback function to grab the pfd structure correspondinf to the clien'ts fd
             */
            Client(sockaddr_in info, int fd, std::function<pollfd& (int)> getPfd);

            /** client's type */
            ClientType type = ClientType::none;


            //===== Misc =========================================//

            /** get client's network information */
            sockaddr_in getNetInfo() const noexcept;

            /** get client's fd */
            int getFd() const noexcept;

            friend std::ostream& operator<<(std::ostream& out, const Client& client);
        private:
            //network information
            sockaddr_in _addr;
            //file descriptor assigned to client
            int _fd;
            //server's public method to retrieve your pfd
            std::function<pollfd& (int)> _getPfd;

    };
} // namespace myhttp

std::ostream& operator<<(std::ostream& out, const sockaddr_in& addr);

