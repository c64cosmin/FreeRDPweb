#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include<string>

namespace freerdpweb{
    class Server{
    public:
        Server();
        ~Server();

        int start();   
    };
}

#endif //_SERVER_HPP_