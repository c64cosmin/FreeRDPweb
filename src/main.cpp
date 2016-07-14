#include "log.hpp"
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
        ("config,c", "Path to config file.")
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
        //do version
        return 0;
    }

    if (variables.count("config")){
        //do serving
        return 0;
    }

    log::console << "Config file is mandatory" << std::endl;
    log::console << argv[0] << " -c <path to config>" << std::endl;
    log::console << argv[0] << " --help" << std::endl;

    return 0;
}
