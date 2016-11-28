#include "service.hpp"

namespace freerdpweb{
    Service* Service::instance(){
        return _instance;
    }

    void Service::start(){
        issueServiceCommand(SC_START);
    }
    void Service::stop(){
        issueServiceCommand(SC_STOP);
    }

    void Service::install(std::string pathConfigFile){
        setConfigFile(pathConfigFile);
        issueServiceCommand(SC_INSTALL);
    }
    void Service::uninstall(){
        setConfigFile("");
        issueServiceCommand(SC_UNINSTALL);
    }

    bool Service::installed(){
        return isInstalled;
    }
    bool Service::running(){
        return isRunning;
    }

    Service* Service::_instance = NULL;
}