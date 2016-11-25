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
            Service();
            ~Service();

            void start();
            void stop();

            void install(std::string pathConfigFile);
            void uninstall();

            bool installed();
            bool running();

            static std::string getConfigFile();
            static void setConfigFile(std::string pathConfigFile);
        private:
            static std::string pathConfigFile;

            bool isInstalled;
            bool isRunning;

            void issueServiceCommand(ServiceCommand command);
    };
}
#endif //_SERVICE_HPP_