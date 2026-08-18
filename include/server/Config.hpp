/*
** Deb PROJECT, 2026
** myServer
** File description:
** Server Config
*/

#pragma once
#include <string>
#include <deque>

//server's config file
#define DEFAULT_CONFIG_PATH "./config/server.conf"


namespace my
{
    /** Server config
     * should be filled through the config file
     * should be editable by reloading the config file
     * or by editing through cli (like editing bash env)
     */
    typedef struct config_s {
        //configPath, if unset use DEFAULT_CONFIG_PATH
        std::string configPath;
        //default path to search for requested URIs
        std::string defaultPath;
        //hosts with custom path for URIs, "host" = "path"
        std::deque<std::string> hosts;
    } config_t;
} // namespace my
