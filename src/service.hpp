#ifndef _SERVICE_HPP_
#define _SERVICE_HPP_

#include <string>

#define SERVICE_NAME "freerdp-web"

namespace freerdpweb{
    enum ServiceCommand{
        SC_QUERY,
        SC_START,
        SC_STOP,
        SC_INSTALL,
        SC_UNINSTALL
    };
    class Service{
        public:
            static Service* instance();

            void start();
            void stop();

            void install(std::string pathConfigFile);
            void uninstall();

            virtual bool installed();
            virtual bool running();

            virtual void run() = 0;

            virtual std::string getConfigFile() = 0;
            virtual void setConfigFile(std::string pathConfigFile) = 0;
        protected:
            static Service* _instance;

            std::string pathConfigFile;

            bool isInstalled;
            bool isRunning;

            virtual void issueServiceCommand(ServiceCommand command) = 0;
    };
}
#endif //_SERVICE_HPP_