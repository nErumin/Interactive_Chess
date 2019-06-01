#ifndef _LAUNCHER_CHILD_PROCESS_H_
#define _LAUNCHER_CHILD_PROCESS_H_

#include <string>
#include <memory>
#include <boost/process.hpp>

class ChildProcess
{
public:
    ChildProcess(std::string childProcessName);

    template <typename... ProcessArgs>
    void run(const ProcessArgs&... args);
    void wait();
    void terminate();
    void assureRunning();
    
    ~ChildProcess();
private:
    std::unique_ptr<boost::process::child> processPtr;
    std::string processName;
};

template <typename... ProcessArgs>
void ChildProcess::run(const ProcessArgs&... args)
{
    if (!boost::filesystem::exists(processName))
    {
        using namespace std;
        throw std::logic_error{ "You don't have '"s + processName + "' in the same directory." };
    }

    std::cout << "Execute the '" << processName << "' process..." << std::endl;

    processPtr = std::make_unique<boost::process::child>(processName, args...);
}

#endif