#ifndef _LOG_HPP_
#define _LOG_HPP_

#ifdef _WIN32
#include <Windows.h>
#else
#include <syslog.h>
#endif

#include <iostream>
#include <ostream>
#include <streambuf>
#include <string>

//define Event Categories
#ifdef _WIN32
#define EVENT_CATEGORY_ERROR 0xC0000000
#define EVENT_CATEGORY_DEBUG 0x40000000
#endif

namespace freerdpweb{
    enum logging_scope{
        LOG_SCOPE_CONSOLE,
        LOG_SCOPE_ERROR,
        LOG_SCOPE_DEBUG,
        LOG_SCOPE_INFO
    };


    class logger{
    public:
        static logger* get();
        void log(std::string message, logging_scope scope);

        static void destroy();
    private:
        logger();
        static logger* instance;
#ifdef _WIN32
        HANDLE hEventLog;
#endif
    };

    class logbuf : public std::streambuf{
        public:
            logbuf(enum logging_scope scope);
        protected:
            virtual int overflow(int c);
        private:
            enum logging_scope scope;
            std::string message;
    };


    class log{
        public:
            static void create();
            static void destroy();

            static std::ostream console;
            static std::ostream error;
            static std::ostream debug;
            static std::ostream info;
    };
}
#endif //_LOG_HPP_
