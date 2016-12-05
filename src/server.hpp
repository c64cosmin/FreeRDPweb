#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include<string>
#include<mutex>

namespace freerdpweb{
    class Server{
    public:
        static Server* instance();

        void setRunning(bool state);
        bool running();

        int start();
    private:
        static Server* _instance;
        Server();

        std::mutex running_lock;
        bool isRunning;
    };
}

#endif //_SERVER_HPP_