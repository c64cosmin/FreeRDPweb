#include <boost\program_options.hpp>

using namespace boost::program_options;

int main(int argc, char** argv){
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
    catch (const error &e){
        //do logging
        return -1;
    }

    if (variables.count("help")){
        //do help
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

    //do help
    return 0;
}