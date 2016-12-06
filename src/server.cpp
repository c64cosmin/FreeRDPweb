#include "server.hpp"
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

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
            std::this_thread::sleep_for(1000ms);
        }
        return 0;
    }
}
