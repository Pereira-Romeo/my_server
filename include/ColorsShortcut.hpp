/*
** Deb PROJECT, 2026
** my namespace
** File description:
** colors everywhere :D
*/

#pragma once
#include <chrono>

#define CSBOLD "\033[1m" //bold text
#define CSPURPLE "\033[38;5;165m" //sexiest purple known to man
#define CSRED "\033[38;5;196m" //reddest red
#define CSFUSCHIA "\033[38;5;201m" //sexy pinkish purple
#define CSBLUE1 "\033[38;5;33m" //deep dark blu
#define CSBLUE2 "\033[38;5;45m" //brightest true blu
#define CSCYAN "\033[38;5;51m" //Cyan
#define CSTEAL "\033[38;5;49m" //is it blu ? is it green ?
#define CSGREEN1 "\033[38;5;34m" //deep dark green
#define CSGREEN2 "\033[38;5;46m" //brightest green

#define CSRESET "\033[0m" //no more color :(

#define CSINFO CSBOLD CSBLUE1 //used for "INFO" something
#define CSWARN CSBOLD CSPURPLE // used for "WARN" something
#define CSERR CSBOLD CSRED // used for "ERR" something
#define CSRB CSRESET CSBOLD //reset colors and set bold

#define CSINFOL CSINFO "INFO" CSRB ": " CSRESET //INFO: (in colors) used to display information
#define CSWARNL CSWARN "WARN" CSRB ": " CSRESET //WARN: (in colors) used to display warnings / recovered errors
#define CSERRL CSERR "ERR" CSRB ": " CSRESET //ERR: (in colors) used to display unrecoverale errors (doesn't necessarly mean program wide fatal error)
#define CSFATERR CSERR "FATAL ERROR" CSRB ": " CSRESET //FATAL ERROR: (in colors) used to display unrecoverale, program wide errors

inline std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << '['
    << std::put_time(std::localtime(&time), "%H:%M:%S")
    << '.' << std::setfill('0') << std::setw(3) << ms.count()
    << ']';
  return oss.str();
}

#define CSTS timestamp() //TIME STAMP: [HH:MM:SS.MS0]

