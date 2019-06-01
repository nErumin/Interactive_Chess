#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <thread>
#include <chrono>
#include "ChildProcess.h"

namespace
{
    constexpr const char RelayProcessName[] = "RobotNetworkRelay.exe";
    constexpr const char DetectorProcessName[] = "ChessPieceDetector.exe";
    constexpr const char SystemProcessName[] = "ChessVisualizer.exe";
}

void waitEnter()
{
    std::cin.clear();

    std::string responseLine;
    std::getline(std::cin, responseLine);
}

void waitRobotConnected()
{
    std::cout << "Press 'ENTER' if you connect the robot to your system..." << std::endl;
    waitEnter();
}

std::string getPortName()
{
    std::cout << "What is your serial port name?" << std::endl;

    std::string portName;
    std::getline(std::cin, portName);

    if (std::cin.fail())
    {
        throw std::logic_error{ "You cannot enter a wrong serial port." };
    }

    return portName;
}

int main()
{
    ChildProcess relayProcess{ RelayProcessName };
    ChildProcess detectorProcess{ DetectorProcessName };
    ChildProcess chessSystemProcess{ SystemProcessName };

    try
    {
        waitRobotConnected();
        
        relayProcess.run(getPortName());
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1500 });

        detectorProcess.run();
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1500 });
        
        chessSystemProcess.run();
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1500 });
        
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{ 500 });

            chessSystemProcess.assureRunning();
            detectorProcess.assureRunning();
            relayProcess.assureRunning();
        }
    }
    catch (const std::logic_error& error)
    {
        std::cout << error.what() << std::endl;
        std::cout << "Please execute the launcher again later..." << std::endl;
    }
    catch (const std::runtime_error&)
    {
        // Pass
    }

    chessSystemProcess.terminate();
    detectorProcess.terminate();
    relayProcess.terminate();

    std::cout << "Press 'ENTER' to finish the launcher..." << std::endl;
    waitEnter();
    return 0;
}