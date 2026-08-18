/*
** Deb PROJECT, 2026
** myServer
** File description:
** Codes
*/

#pragma once

namespace myhttp
{

enum class code { // : uint64_t or something, don't foget to add later
    OK = 200, //OK
    BadReq = 400, //Bad Request
    Forbid = 403, //Forbidden
    NotFound = 404, //Not Found
    LengthReq = 411, //Length Required
};

//add codeToString wich returns the status line description of the code
//but maybe have ability to custom desc code ?

} // namespace myhttp

