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
             * @param _errno set this to errno if you want the errno message to be added to your message, defaults to 0 (no message added)
             * @param val value you want this error to give, defaults to 84
             */
            Error(std::string msg, int _errno = 0, int val = 84):_msg(msg), _val(val)
                {if (_errno != 0) {_msg.append(" | last recorded errno: " + std::string(std::strerror(_errno)));};};
            /** Error class constructor
             * @param oss error message (as a stream)
             * @param _errno set this to errno if you want the errno message to be added to your message, defaults to 0 (no message added)
             * @param val value you want this error to give, defaults to 84
             */
            Error(std::ostringstream oss, int _errno = 0, int val = 84):Error(oss.str(), _errno, val) {};

            const char* what() const noexcept override {return _msg.c_str();};
            int value() const {return _val;};

        protected:
            std::string _msg;
            int _val;
    };
}
