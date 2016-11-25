#include "service.hpp"
#include "log.hpp"
#include "server.hpp"

#ifdef _WIN32
#include <Windows.h>
#define REG_PATHKEY "SOFTWARE\\Cloudbase Solutions\\freerdp-web"
#define REG_SUBKEY "configfile"
#endif

namespace freerdpweb{
    Service::Service() :
        isInstalled(false),
        isRunning(false){
        issueServiceCommand(SC_QUERY);
    }

    Service::~Service(){
    }

    bool Service::installed(){
        return this->isInstalled;
    }

    bool Service::running(){
        return this->isRunning;
    }

    void Service::start(){
        issueServiceCommand(SC_START);
    }

    void Service::stop(){
        issueServiceCommand(SC_STOP);
    }

    void Service::install(std::string pathConfigFile){
        Service::setConfigFile(pathConfigFile);
        issueServiceCommand(SC_INSTALL);
    }

    void Service::uninstall(){
        Service::setConfigFile("");
        issueServiceCommand(SC_UNINSTALL);
    }

    void Service::issueServiceCommand(ServiceCommand command){

#ifdef _WIN32
        SC_HANDLE hSCM = NULL;
        SC_HANDLE hService = NULL;
        SERVICE_STATUS sStatus;
        DWORD dwDesiredAccess;

        switch (command){
        case SC_QUERY:
            dwDesiredAccess = GENERIC_READ;
            break;
        case SC_START:
            dwDesiredAccess = SERVICE_START;
            break;
        case SC_STOP:
            dwDesiredAccess = SERVICE_STOP;
            break;
        case SC_INSTALL:
            dwDesiredAccess = SERVICE_ALL_ACCESS;
            break;
        case SC_UNINSTALL:
            dwDesiredAccess = DELETE;
        }

        hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!hSCM){
            log::error << "Cannot open SCM; Error Code : " << GetLastError() << std::endl;
        }
        else{
            //create the service
            if (command == SC_INSTALL){
                char pathToBinary[MAX_PATH];
                if (GetModuleFileName(NULL, pathToBinary, MAX_PATH) == 0){
                    log::error << "Cannot get the path to this binary; Error Code : " << GetLastError() << std::endl;
                }
                else{
                    if (this->installed()){
                        log::console << "Service is already installed." << std::endl;
                        log::console << "Use \"--config path\" to change the config file." << std::endl;
                    }
                    else{
                        hService = CreateService(hSCM, SERVICE_NAME, SERVICE_NAME,
                                                 dwDesiredAccess,
                                                 SERVICE_WIN32_OWN_PROCESS,
                                                 SERVICE_AUTO_START,
                                                 SERVICE_ERROR_IGNORE,
                                                 pathToBinary,
                                                 NULL,
                                                 NULL,
                                                 "Eventlog\0",//Eventlog is the only dependency, we need double null-termination
                                                 NULL,
                                                 NULL
                                                 );
                    }
                }
            }
            //open the service
            else{
                hService = OpenService(hSCM, SERVICE_NAME, dwDesiredAccess);
            }

            if (!hService) {
                CloseServiceHandle(hSCM);
                log::error << "Cannot acquire handle for the service; Error Code : " << GetLastError() << std::endl;
            }
            else{
                this->isInstalled = true;
                int result;
                switch (command){
                case SC_QUERY:
                    result = QueryServiceStatus(hService, &sStatus);
                    if (result == 0){
                        log::error << "Cannot query the service; Error Code : " << GetLastError() << std::endl;
                    }
                    //the service is running only when it is not stopped;
                    this->isRunning = sStatus.dwCurrentState != SERVICE_STOPPED;
                    break;
                case SC_START:
                    result = StartService(hService, 0, NULL);
                    if (result == 0){
                        log::error << "Cannot start the service; Error Code : " << GetLastError() << std::endl;
                    }
                    else{
                        log::console << "Service " << SERVICE_NAME << " started." << std::endl;
                    }
                    break;
                case SC_STOP:
                    result = ControlService(hService, SERVICE_CONTROL_STOP, &sStatus);
                    if (result == 0){
                        log::error << "Cannot stop the service; Error Code : " << GetLastError() << std::endl;
                    }
                    else{
                        log::console << "Service " << SERVICE_NAME << " stopped." << std::endl;
                    }
                    break;
                case SC_INSTALL:
                    log::console << "Service " << SERVICE_NAME << " installed." << std::endl;
                    break;
                case SC_UNINSTALL:
                    result = DeleteService(hService);
                    if (result == 0){
                        log::error << "Cannot delete the service; Error Code : " << GetLastError() << std::endl;
                    }
                    else{
                        log::console << "Service " << SERVICE_NAME << " removed." << std::endl;
                    }
                    break;
                }
            }
        }

        if (hSCM) CloseServiceHandle(hSCM);
        if (hService) CloseServiceHandle(hService);
#else
#endif
    }

    std::string Service::pathConfigFile = "";

    std::string Service::getConfigFile(){
#ifdef _WIN32
        if (Service::pathConfigFile.empty()){
            HKEY hRegistryKey;
            LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATHKEY, 0, KEY_READ, &hRegistryKey);
            if (result == ERROR_SUCCESS){
                char path[MAX_PATH];
                LONG len = MAX_PATH;
                result = RegQueryValue(hRegistryKey, REG_SUBKEY, path, &len);
                if (result == ERROR_SUCCESS){
                    Service::pathConfigFile = std::string(path);
                }
                else{
                    log::error << "Error when reading from a registry key; Error Code : " << result << std::endl;
                }
            }
            else if (result == ERROR_FILE_NOT_FOUND || result == ERROR_NO_MATCH){
                log::error << "Looks like freerdp-web was not installed properly; Issue \"freerdp-web install <full-path to config file>\" to properly install." << std::endl;
            }
            else{
                log::error << "Error when opening a registry key; Error Code : " << result << std::endl;
            }
        }
#else
#endif
        return Service::pathConfigFile;
    }

    void Service::setConfigFile(std::string pathConfigFile){
#ifdef _WIN32
        HKEY hRegistryKey = NULL;

        if (!pathConfigFile.empty()){
            LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATHKEY, 0, KEY_SET_VALUE, &hRegistryKey);

            if (result == ERROR_FILE_NOT_FOUND || result == ERROR_NO_MATCH){
                result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_PATHKEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRegistryKey, NULL);
                if (result != ERROR_SUCCESS){
                    log::error << "Error when creating a registry key; Error Code : " << result << std::endl;
                }
            }
            
            if (result == ERROR_SUCCESS){
                result = RegSetValue(hRegistryKey, REG_SUBKEY, REG_SZ, pathConfigFile.c_str(), 0);
                if (result == ERROR_SUCCESS){
                    Service::pathConfigFile = pathConfigFile;
                    log::console << "Config file path changed to " << pathConfigFile << std::endl;
                }
                else{
                    log::error << "Error when writing a registry key; Error Code : " << result << std::endl;
                }
            }
            else{
                log::error << "Error when opening a registry key; Error Code : " << result << std::endl;
            }
        }
        else{
            LONG result = RegDeleteTree(HKEY_LOCAL_MACHINE, REG_PATHKEY);
            if (result == ERROR_SUCCESS){
                Service::pathConfigFile = pathConfigFile;
            }
            else{
                log::error << "Error when deleting a registry key; Error Code : " << result << std::endl;
            }
        }

        if (hRegistryKey){
            RegCloseKey(hRegistryKey);
        }
#else
#endif
    }
}

