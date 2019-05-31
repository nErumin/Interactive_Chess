#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <exception>

namespace
{
    constexpr const char RelayProcessName[] = "RobotNetworkRelay.exe";
}

boost::process::child SpawnRelayProcess()
{
    if (!boost::filesystem::exists(RelayProcessName))
    {
        using namespace std;
        throw std::logic_error{ "You don't have '"s + RelayProcessName + "' in the same directory." };
    }

    std::string responseLine;

    std::cout << "Press 'ENTER' if you connect the robot to your system..." << std::endl;
    std::getline(std::cin, responseLine);

    std::cout << "What is your serial port name?" << std::endl;

    std::string portName;
    std::cin >> portName;

    if (std::cin.fail())
    {
        throw std::logic_error{ "You cannot enter a wrong serial port." };
    }

    std::cout << "Execute the relay server process..." << std::endl;
    return boost::process::child{ RelayProcessName, portName };
}

int main()
{
    try
    {
        auto relayProcess = SpawnRelayProcess();

        relayProcess.wait();

        int relayResultCode = relayProcess.exit_code();
        std::cout << relayResultCode << std::endl;
    }
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        std::cout << "Please execute the launcher again later..." << std::endl;
    }

    return 0;
}