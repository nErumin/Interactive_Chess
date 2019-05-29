#include <Server.h>
#include <Address.h>
#include <SerialService.h>
#include <NetworkError.h>
#include <SocketConnection.h>
#include <TransmissionService.h>

#include <iostream>
#include <string>
#include <exception>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " <serial_port>" << std::endl;
        return 1;
    }
    
    try
    {
        Network::Server relayServer{ Network::Address("localhost", 44444) };
        Network::SerialService robotSerialService{ argv[1] };

        auto connection = relayServer.waitClient();
        
        Network::TransmissionService relayTransmission{ connection };
        
        while (true)
        {
            auto receivedMessage = relayTransmission.receive(1024);
            std::cout << "Relay received: " << receivedMessage << std::endl;

            robotSerialService.send(receivedMessage);
            std::cout << "To robot: " << receivedMessage << std::endl;

            auto robotCompletionMessage = robotSerialService.receive(1024);
            std::cout << "From robot: " << receivedMessage << std::endl;

            relayTransmission.send("OK");
            std::cout << "Relay sent: OK" << std::endl;
        }
    }
    catch (const Network::NetworkError & error)
    {
        std::cout << "Cannot start the relay server. Please try again later." << std::endl;
        std::cout << "Cause: " << error.what() << std::endl;
    }
    catch (const std::exception& error)
    {
        std::cout << "Cannot start the relay server. Please try again later." << std::endl;
        std::cout << "Cause: " << error.what() << std::endl;
    }


    return 0;
}