#include "server.hpp"
#include <Windows.h>

namespace freerdpweb{
    Server* Server::instance(){
        if (Server::_instance == NULL){
            Server::_instance = new Server();
        }
        return Server::_instance;
    }

    Server* Server::_instance = NULL;

    Server::Server(){

    }

    void Server::setRunning(bool state){
        running_lock.lock();
        this->isRunning = state;
        running_lock.unlock();
    }

    bool Server::running(){
        running_lock.lock();
        bool r = this->isRunning;
        running_lock.unlock();
        return r;
    }

    int Server::start(){
        setRunning(true);
        while (this->running()){
            Sleep(10);
        }
        return 0;
    }
}
