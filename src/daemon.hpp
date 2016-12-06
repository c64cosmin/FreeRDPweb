#ifndef _DAEMON_HPP_
#define _DAEMON_HPP_

#include "service.hpp"

namespace freerdpweb{
    class Daemon : public Service{
        public:
            static void init();

            virtual std::string getConfigFile();
            virtual void setConfigFile(std::string pathConfigFile);
        protected:
            virtual void issueServiceCommand(ServiceCommand command);
        private:
            Daemon();

            unsigned int getProcessPID(std::string procname);
    };
}

#endif //_DAEMON_HPP_