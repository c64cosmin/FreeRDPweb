#include "log.hpp"

namespace freerdpweb{
    logger* logger::get(){
        if (logger::instance == NULL){
            logger::instance = new logger();
        }
        return logger::instance;
    }

    void logger::log(std::string message, logging_scope scope){
#ifdef _WIN32
        WORD wType;
        DWORD dwEventID;
        switch (scope){
        case LOG_SCOPE_CONSOLE:
            std::cout << message << std::endl;
            return;

        case LOG_SCOPE_ERROR:
            wType = EVENTLOG_ERROR_TYPE;
            dwEventID = EVENT_CATEGORY_ERROR;
            break;

        case LOG_SCOPE_DEBUG:
            wType = EVENTLOG_INFORMATION_TYPE;
            dwEventID = EVENT_CATEGORY_DEBUG;
            break;

        case LOG_SCOPE_INFO:
            wType = EVENTLOG_INFORMATION_TYPE;
            dwEventID = EVENT_CATEGORY_DEBUG;
            break;
        }

        LPCSTR msg = message.c_str();

        ReportEventA(hEventLog,
            wType,
            0,
            dwEventID,
            NULL,
            1,
            0,//no event specific data
            &msg,
            NULL);
#else
        std::string type("");
        int priority;
        switch (scope){
        case LOG_SCOPE_CONSOLE:
            std::cout << message << std::endl;
            return;

        case LOG_SCOPE_ERROR:
            type = "Error:";
            priority = LOG_ERR;
            break;

        case LOG_SCOPE_DEBUG:
            type = "Debug:";
            priority = LOG_DEBUG;
            break;

        case LOG_SCOPE_INFO:
            priority = LOG_INFO;
            break;
        }

        syslog(priority, "%s%s", type.c_str(), message.c_str());
#endif
    }

    void logger::destroy(){
#ifdef _WIN32
        DeregisterEventSource(logger::instance->hEventLog);
#endif
        delete logger::instance;
        logger::instance = NULL;
    }

    logger::logger(){
#ifdef _WIN32
        hEventLog = RegisterEventSourceA(NULL, "freerdp-web");
#endif
    }

    logger* logger::instance = NULL;


    logbuf::logbuf(enum logging_scope scope) :
        scope(scope),
        message("")
    {
    }

    int logbuf::overflow(int c = EOF){
        //did we encounter a new line, then send the message
        if (c == '\n'){
            if (!message.empty()){
                logger::get()->log(message, scope);
            }
            message.clear();
        }
        else{
            message += c;
        }
        return c;
    }


    void log::create(){
        logger::get();

        log::console.rdbuf(new logbuf(LOG_SCOPE_CONSOLE));
        log::error.rdbuf(new logbuf(LOG_SCOPE_ERROR));
        log::debug.rdbuf(new logbuf(LOG_SCOPE_DEBUG));
        log::info.rdbuf(new logbuf(LOG_SCOPE_INFO));
    }

    void log::destroy(){
        logger::destroy();

        delete log::console.rdbuf();
        delete log::error.rdbuf();
        delete log::debug.rdbuf();
        delete log::info.rdbuf();
    }

    std::ostream log::console(NULL);
    std::ostream log::error(NULL);
    std::ostream log::debug(NULL);
    std::ostream log::info(NULL);
}
