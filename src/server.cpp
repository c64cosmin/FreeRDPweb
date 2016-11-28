#include "server.hpp"
#include "service.hpp"
#include <Windows.h>

namespace freerdpweb{
    Server::Server(){

    }

    Server::~Server(){

    }

    int Server::start(){
        while (Service::instance()->running()){
            Sleep(10);
        }
        return 0;
    }
}
