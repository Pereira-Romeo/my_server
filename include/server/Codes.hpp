/*
** Deb PROJECT, 2026
** myhttp
** File description:
** Codes
*/

#pragma once

namespace myhttp
{

enum class code {
    OK = 200, //OK
    BadReq = 400, //Bad Request
    Forbid = 403, //Forbidden
    NotFound = 404, //Not Found
    LengthReq = 411, //Length Required
};

//add codeToString wich returns the status line description of the code
//but maybe have ability to custom desc code ?

} // namespace myhttp

