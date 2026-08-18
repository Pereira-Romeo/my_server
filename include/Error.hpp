/*
** Deb PROJECT, 2026
** my namespace
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
             * @param _errno set this to errno if you want the errno message to be added to your message, defaults to 0 (no message added)
             * @param val value you want this error to give, defaults to 84
             */
            Error(std::string msg, int _errno = 0, int val = 84):_msg(msg), _val(val), __errno(_errno)
                {if (_errno != 0) {_msg.append(" | last recorded errno(" + std::to_string(_errno) + "): " + std::string(std::strerror(_errno)));};};
            /** Error class constructor
             * @param oss error message (as a stream)
             * @param _errno set this to errno if you want the errno message to be added to your message, defaults to 0 (no message added)
             * @param val value you want this error to give, defaults to 84
             */
            Error(std::ostringstream oss, int _errno = 0, int val = 84):Error(oss.str(), _errno, val) {};

            /** get the error message */
            const char* what() const noexcept override {return _msg.c_str();};
            /** get the value assigned to that error */
            int value() const {return _val;};
            /** get the errno value that was given at construction of this error */
            int erno() const {return __errno;};

        protected:
            std::string _msg;
            int _val;
            int __errno;
    };
} // namespace my
