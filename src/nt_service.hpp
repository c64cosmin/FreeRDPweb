#ifdef _WIN32
#ifndef _NT_SERVICE_HPP_
#define _NT_SERVICE_HPP_

#include "service.hpp"
#include <mutex>

namespace freerdpweb{
    class NTService : public Service{
        public:
            static void init();
            virtual void run();

            //isRunning is used internally to tell if the service need to stop
            virtual bool running();
            void setRunning(bool state);

            virtual std::string getConfigFile();
            virtual void setConfigFile(std::string pathConfigFile);
        protected:
            virtual void issueServiceCommand(ServiceCommand command);
        private:
            NTService();
            ~NTService();

            std::mutex running_lock;
    };
}

#endif //_NT_SERVICE_HPP_
#endif