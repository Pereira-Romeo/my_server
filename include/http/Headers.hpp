/*
** Deb PROJECT, 2026
** myServer
** File description:
** HttpHeaders
*/

#pragma once
#include "./StartLine.hpp"
#include <string>
#include <map>


namespace myhttp
{

typedef struct header_s {
//do i put every category and every category has every field ? probably
} header_t;

//perhaps a header type can be useful to prevent too much parsing
typedef struct body_s {
    std::string body;
    std::map<std::string_view, std::string_view> field;
} body_t;


typedef struct http_message_s { //replace response_t with that in #9
    union start_line {
        request_t req;
        status_t sts;
    };
    // sl_t status;//i don't remember what this was supposed to be, prolly useless
    std::string header;
    std::string body;
    bool ready; //true if response ready to send (once this is set to true, before sending it should automaticly look at the size of body and add field `Content-Length: body.size()`)
} http_mess_t;

class Header {
    public:

    private:
        /** start line of the header,
         * depending on request or response, should look something like:
         * "METHOD /path HTTP/version" or "HTTP/version code description" respectively
        */
        StartLine _sl;
};

} // namespace myhttp
