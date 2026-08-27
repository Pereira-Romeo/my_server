/*
** EPITECH PROJECT, 2026
** myServer
** File description:
** http message
*/

#pragma once
#include "./Headers.hpp"

namespace myhttp
{

/** Could be used to build a message but honestly not worth it i think
 * only useful for request really
 */
class message {
    public:

    private:
        /** used to store the raw message received */
        std::string _rawMessage;
        /** header of the message */
        Header _header;
        /** false if the next input is expected to be more info for the header, true if next input is a body or next command */
        bool _headerComplete;
        /** body of the message (string view into _rawMessage) */
        std::string_view _body;
};


} // namespace myhttp

