#include "ChildProcess.h"
#include <boost/filesystem.hpp>
#include <iostream>

ChildProcess::ChildProcess(std::string childProcessName)
    : processName{ childProcessName }
{
}

void ChildProcess::wait()
{
    if (processPtr)
    {
        processPtr->wait();
    }
}

void ChildProcess::terminate()
{
    if (processPtr)
    {
        processPtr->terminate();
    }
}

void ChildProcess::assureRunning()
{
    if (processPtr && !processPtr->running())
    {
        std::cout << "Process '" << processName << "' terminated!" << std::endl;
        throw std::runtime_error{ "not running" };
    }
}

ChildProcess::~ChildProcess()
{
    terminate();
}