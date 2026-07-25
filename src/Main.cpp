/*
** Deb PROJECT, 2026
** myhttp
** File description:
** Main
*/

#include "server/Server.hpp"
#include "ColorsShortcut.hpp"

int main(int ac, char **av)
{
        try {
        //arg handling
        // my::ArgHandler args(argc, argv);
        // if (args.help())
        //     return helper(0);
        // std::deque<std::string> flag_args;
        // std::deque<std::string> teams;
        // args.find("-p", true, 1, flag_args);
        // args.find("-x", true, 1, flag_args);
        // args.find("-y", true, 1, flag_args);
        // args.find("-c", true, 1, flag_args);
        // args.find("-f", true, 1, flag_args);
        // args.find("-n", true, -1, teams);
        // std::cout << "width: " << flag_args.at(1) << std::endl;
        // std::cout << "height: " << flag_args.at(2) << std::endl;
        // std::cout << "clientsNb: " << flag_args.at(3) << std::endl;
        // std::cout << "freq: " << flag_args.at(4) << std::endl;

        // std::cout << "teams are:" << std::endl;
        // for (size_t i = 0; i < teams.size(); i++) {
        //     std::cout << "- " + teams[i] << std::endl;
        // }

        // args.tryThrowUnrecognized();

        //main
        myhttp::Server server(av[1]);
        server.run();
        return 0;
    } catch (my::Error& e) {
        std::cerr << CSERR "Fatal error" CSRB ": " CSRESET  << e.what() << std::endl;
        return e.value();
    } catch (std::exception& e) {
        std::cerr << CSERR "Fatal error" CSRB ": " CSRESET << e.what() << "\nLast registered errno(" << errno << "): " << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    } catch (...) {
        std::cerr << CSERR "Caught error" CSRB ": " CSRESET << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    }
    return 0;
}
