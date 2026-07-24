/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Error
*/

#pragma once
#include <cerrno>
#include <cstring>
#include <exception>
#include <string>
#include <sstream>

namespace my {
    class Error: public std::exception {
        public:
            /** Error class constructor
             * @param msg error message
             * @param addErrno set to true if you want to append the errno message to your message
             */
            Error(std::string msg, bool addErrno):_msg(msg)
                {if (errno != 0 && addErrno) {_msg.append(" | last recorded errno: " + std::string(std::strerror(errno)));};};
            /** Error class constructor
             * @param oss ostringstream, essentially a string but easier to use :)
             * @param addErrno set to true if you want to append the errno message to your message
             */
            Error(std::ostringstream oss, bool addErrno):Error(oss.str(), addErrno) {};
            /** Error class constructor
             * @param msg error message
             * @param val value you want this error to give
             * @param addErrno set to true if you want to append the errno message to your message
             */
            Error(std::string msg, int val, bool addErrno):Error(msg, addErrno) {_val = val;};
            /** Error class constructor
             * @param oss ostringstream, essentially a string but easier to use :)
             * @param val value you want this error to give
             * @param addErrno set to true if you want to append the errno message to your message
             */
            Error(std::ostringstream oss, int val, bool addErrno):Error(oss.str(), val, addErrno) {};

            std::string what() {return _msg;};
            int value() {return _val;};

        protected:
            std::string _msg = "";
            int _val = 84;
    };
}
