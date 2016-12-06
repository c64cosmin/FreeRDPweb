#ifndef _WIN32
#include "daemon.hpp"
#include "log.hpp"
#include "server.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
 
#define CONFFILEPATH "/etc/freerdp-web"
#define CONFFILENAME "configfile"

namespace freerdpweb{
    void Daemon::init(){
        Service::_instance = new Daemon();
    }
    Daemon::Daemon(){
        isInstalled = false;
        isRunning = false;
        issueServiceCommand(SC_QUERY);
    }

    void Daemon::issueServiceCommand(ServiceCommand command){
        switch (command){
            case SC_QUERY:
            {
                auto pid = getProcessPID(SERVICE_NAME);
                //the process is running only if there is a pid for it
                this->isRunning = pid != 0;
                break;
            }
            case SC_START:
            {
                if(!this->isRunning){
                    pid_t pid = fork();
                    if(pid == 0){
                        int nfd = open("/dev/null", O_RDWR);
                        dup2(nfd, 0);
                        dup2(nfd, 1);
                        dup2(nfd, 2);
                        close(nfd);
                        if(setsid() != 0){
                            log::error << "Cannot set session group leader; errno set to " << errno << std::endl;
                        }
                        //start the server
                        auto server = freerdpweb::Server::instance();
                        int result = server->start();
                    }
                    else if(pid == -1){
                        log::error << "fork() returned and error; errno set to " << errno << std::endl;
                        log::console << "Cannot start the service; errno set to " << errno << std::endl;
                    }
                    else{
                        log::console << "Service " << SERVICE_NAME << " started." << std::endl;
                    }
                }
                break;
            }
            case SC_STOP:
            {
                auto pid = getProcessPID(SERVICE_NAME);
                int result = kill((pid_t)pid, SIGTERM);
                if (result != 0){
                    log::error << "Cannot stop the service; errno set to " << errno << std::endl;
                }
                else{
                    //wait until the process stops
                    bool stillAlive = true;
                    while (stillAlive){
                        result = kill((pid_t)pid, 0);
                        if (errno == ESRCH){
                            log::console << "Service " << SERVICE_NAME << " stopped." << std::endl;
                            stillAlive = false;
                        }
                        else{
                            log::error << "Error when checking if PID is still valid; errno set to " << errno << std::endl;
                            break;
                        }
                    }
                }
                break;
            }
            case SC_INSTALL:
            {
                log::console << "Service " << SERVICE_NAME << " installed." << std::endl;
                break;
            }
            case SC_UNINSTALL:
            {
                log::console << "Service " << SERVICE_NAME << " removed." << std::endl;
                break;
            }
        }
    }

    std::string Daemon::getConfigFile(){
        std::ifstream file(CONFFILEPATH);
        if (file.good()){
            file >> Daemon::pathConfigFile;
        }
        else{
            log::error << "Looks like freerdp-web was not installed properly; Issue \"freerdp-web install <full-path to config file>\" to properly install." << std::endl;
        }
        file.close();
        return Daemon::pathConfigFile;
    }

    void Daemon::setConfigFile(std::string pathConfigFile){
        std::string fullpath = std::string(CONFFILEPATH) + "/" + std::string(CONFFILENAME);
        if (!pathConfigFile.empty()){
            DIR* confDir = opendir(CONFFILEPATH);
            if (confDir){
                closedir(confDir);
            }
            else if (errno == ENOENT){
                int result = mkdir(CONFFILEPATH, S_IRWXU | S_IRWXG | S_IRWXO);
                if (result != 0){
                    log::error << "Error when creating directory \"" << CONFFILEPATH << "\"; errno set to " << errno << std::endl;
                    return;
                }
            }
            else{
                log::error << "Error when checking if " << CONFFILEPATH << " exists; errno set to " << errno << std::endl;
                return;
            }
            std::ofstream file(fullpath);
            if (file.good()){
                file << pathConfigFile;
            }
            file.close();
        }
        else{
            int result;
            result = remove(fullpath.c_str());
            if (result != 0){
                log::error << "Error when deleting \"" << fullpath << "\"; errno set to " << errno << std::endl;
            }
            result = remove(CONFFILEPATH);
            if (result != 0){
                log::error << "Error when deleting \"" << CONFFILEPATH << "\"; errno set to " << errno << std::endl;
            }
        }
    }

    unsigned int Daemon::getProcessPID(std::string procname){
        unsigned int selfPID = (unsigned int)getpid();
        DIR* procDir;
        procDir = opendir("/proc");
        if (procDir == NULL){
            log::error << "There was an error opening \"/proc\"; errno set to " << errno << std::endl;
        }
        else{
            dirent* dir;
            char* endptr;
            while ((dir = readdir(procDir)) != NULL){
                long lpid = strtol(dir->d_name, &endptr, 10);
                // /proc/pid files are entirely numerical, then *endptr must be null character,
                // else it isn't a /proc/pid file, so we skip ahead
                if (*endptr == '\0') {
                    std::string rname;

                    std::string statFilename = "/proc/";
                    statFilename += dir->d_name;
                    statFilename += "/stat";

                    std::ifstream statFile(statFilename);

                    if (statFile.good()){
                        unsigned int rpid;
                        statFile >> rpid;
                        statFile >> rname;
                        //remove the surrounding parentheses
                        rname = rname.substr(1, rname.length()-2);

                        if (procname.compare(rname) == 0 && rpid != selfPID){
                            return rpid;
                        }
                    }

                    statFile.close();
                }
            }

            if (closedir(procDir) != 0){
                log::error << "There was an error closing \"/proc\"; errno set to " << errno << std::endl;
            }
        }
        
        //default is 0, means the process is not running
        return 0;
    }
}
#endif
