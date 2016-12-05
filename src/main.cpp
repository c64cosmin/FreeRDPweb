#include <iostream>
#include <fstream>
#include "log.hpp"
#include "server.hpp"
#ifdef _WIN32
#include "nt_service.hpp"
#endif

using namespace freerdpweb;

int parseCLI(int argc, char** argv);
int main(int argc, char** argv){
    log::create();

#ifdef _WIN32
    NTService::init();
#endif
    int result = parseCLI(argc, argv);

    log::destroy();

    return result;
}

int parseCLI(int argc, char** argv){
    if (argc == 2){
        std::string arg(argv[1]);
        if (arg.compare("-h") == 0 || arg.compare("--help") == 0){
            log::console << "-h [--help]           Show this message and exit." << std::endl;
            log::console << "-v [--version]        Show version and exit." << std::endl;
            log::console << "-c [--config] path    Set config file location; must be full path." << std::endl;
            log::console << "-i [--install] path   Install the service and set config file location; must be full path." << std::endl;
            log::console << "-u [--uninstall]      Remove the service." << std::endl;
            log::console << "-s [--start]          Start the service." << std::endl;
            log::console << "-x [--stop]           Stop the service." << std::endl;
            log::console << "-r [--run]            Run the server, path file must be set" << std::endl;
            return 0;
        }
        else if (arg.compare("-v") == 0 || arg.compare("--version") == 0){
            log::console << VERSION_STRING << std::endl;
            return 0;
        }
        else if (arg.compare("-c") == 0 || arg.compare("--config") == 0 ||
            arg.compare("-i") == 0 || arg.compare("--install") == 0){
            log::console << "Path to config file not specified!" << std::endl;
            return 1;
        }
        else if (arg.compare("-u") == 0 || arg.compare("--uninstall") == 0){
            Service::instance()->uninstall();
            return 0;
        }
        else if (arg.compare("-s") == 0 || arg.compare("--start") == 0){
            Service::instance()->start();;
            return 0;
        }
        else if (arg.compare("-x") == 0 || arg.compare("--stop") == 0){
            Service::instance()->stop();
            return 0;
        }
        else if (arg.compare("-r") == 0 || arg.compare("--run") == 0){
            auto server = freerdpweb::Server::instance();
            return server->start();
        }
    }
    else if (argc == 3){
        std::string arg(argv[1]);
        std::string path(argv[2]);
        std::ifstream file;
        file.open(path);
        //the config file exists
        if (file.good()){
            if (arg.compare("-c") == 0 || arg.compare("--config") == 0){
                Service::instance()->setConfigFile(path);
                return 0;
            }
            else if (arg.compare("-i") == 0 || arg.compare("--install") == 0){
                Service::instance()->install(path);
                return 0;
            }
        }
    }

#ifdef _WIN32
    Service::instance()->run();
#endif
    log::console << "Use \"" << argv[0] << " --help\" for more information." << std::endl;

    return 0;
}
