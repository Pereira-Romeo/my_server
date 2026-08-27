/*
** Deb PROJECT, 2026
** myServer
** File description:
** HttpStartLinee
*/

#pragma once
#include <string>
#include "./Codes.hpp"

namespace myhttp
{

//start line of request
typedef struct request_s {
    std::string method;
    std::string path; //URI
    std::string version; // keep full HTTP/1.1 in it, will be needed for modular libs later >:D
} request_t;

//start line of response
typedef struct status_s {//idk what goes in there yet
    std::string version;
    code code;
    std::string codeDesc; //code description like "OK" for 200
} status_t;

/** only really used */
class requestLine {
    public:

    private:
        /** Get, PUT, QUERY... */
        std::string_view _method;
        /** URI */
        std::string_view _path;
        /** HTTP/1.1 */
        std::string_view _version;
};

} // namespace myhttp

