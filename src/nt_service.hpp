#ifdef _WIN32
#ifndef _NT_SERVICE_HPP_
#define _NT_SERVICE_HPP_

#include "service.hpp"

namespace freerdpweb{
    class NTService : public Service{
        public:
            static void init();
            virtual void run();

            virtual std::string getConfigFile();
            virtual void setConfigFile(std::string pathConfigFile);
        protected:
            virtual void issueServiceCommand(ServiceCommand command);
        private:
            NTService();
    };
}

#endif //_NT_SERVICE_HPP_
#endif