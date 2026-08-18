/*
** Deb PROJECT, 2026
** myServer
** File description:
** Client
*/

#pragma once
#include "http/Basics.hpp"
#include "./ClientType.hpp"
#include "Error.hpp"

//Networking
#include <poll.h>       //poll & pollfd
#include <functional>   //Server functions callback
#include <netinet/in.h> //sockaddr_in struct
#include <arpa/inet.h>  //print network info

namespace my
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


            //===== client input handling ========================//

            //===== buffer management ============================//

            /** get client's saved input
             * @note the input buffer should be used to buffer incomplete commands
             * @note so when you receive something from the client, the real input of the client is:
             * @note _inBuffer + received text
             * @returns string
             */
            std::string getInput();

            /** save to the back of client's input
             * @note the input buffer should be used to buffer incomplete commands
             * @note so when you receive something from the client, the real input of the client is:
             * @note _inBuffer + received text
             */
            void saveInput(std::string in);

            /** add output to output buffer
             * @note make sure to add the ending \\n
             */
            void addOutput(std::string out);

            /** add output to output buffer
             * @note make sure to add the ending \\n
             */
            void addOutput(std::ostringstream out);

            /** add output to the front of output buffer
             * use this to add back an output that was partially written
             */
            void pushOutput(std::string out);

            /** used to retrieve current output until the last \\n
             * @returns string
             */
            std::string getOutput();


            //===== Misc =========================================//

            /** get client's network information */
            sockaddr_in getNetInfo() const noexcept;

            /** get client's fd */
            int fd() const noexcept;

            friend std::ostream& operator<<(std::ostream& out, const Client& client);
        private:
            //network information
            sockaddr_in _addr;
            //file descriptor assigned to client
            int _fd;
            //server's public method to retrieve your pfd
            std::function<pollfd& (int)> _getPfd;

            /** output buffer */
            std::string _outBuffer;

            /** input buffer
             * @note the input buffer should be used to buffer incomplete commands
             * @note so when you receive something from the client, the real input of the client is:
             * @note _inBuffer + received text
             */
            std::string _inBuffer;

            /** activates client's pollout if there is a complete output
             * @note a complete output is any output ending by \\n
             */
            void polloutActivator();
    };
} // namespace my

std::ostream& operator<<(std::ostream& out, const sockaddr_in& addr);
