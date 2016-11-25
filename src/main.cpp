#include "log.hpp"
//#include "server.hpp"
#include "service.hpp"
#include <boost\program_options.hpp>

using namespace boost::program_options;
using namespace freerdpweb;

int parseCLI(int argc, char** argv);
int main(int argc, char** argv){
    log::create();

    int result = parseCLI(argc, argv);

    log::destroy();

    return result;
}

int parseCLI(int argc, char** argv){
    options_description description("Program options");
    description.add_options()
        ("help,h", "Show this message and exit.")
        ("version,v", "Show version information and exit.")
        ("config,c", value<std::string>(), "Set the config file.")
        ("install", value<std::string>(), "Install the service.")
        ("uninstall", "Remove the service.")
        ("start", "Start the service.")
        ("stop", "Stop the service.")
        ("restart", "Restart the service")
        ;

    variables_map variables;
    try{
        store(parse_command_line(argc, argv, description), variables);
        notify(variables);
    }
    catch (error &e){
        log::console << "Wrong parameters, use:" << std::endl << argv[0] << " help" << std::endl;
        return -1;
    }

    if (variables.count("help")){
        log::console << description << std::endl;
        return 0;
    }

    if (variables.count("version")){
        log::console << "Version: " << VERSION_STRING << std::endl;
        return 0;
    }

    if (variables.count("config")){
        Service s;
        s.setConfigFile(variables["config"].as<std::string>());
    }

    if (variables.count("install")){
        std::string path = variables["install"].as<std::string>();
        if (path.empty()){
            log::console << "Specify the full path for the config file";
        }
        else{
            Service s;
            s.install(path);
        }
    }

    if (variables.count("uninstall")){
        Service s;
        s.uninstall();
    }

    log::console << "Config file is mandatory" << std::endl;
    log::console << argv[0] << " -c <path to config>" << std::endl;
    log::console << argv[0] << " --help" << std::endl;

    return 0;
}
