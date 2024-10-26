// Description: Main class of the program. It parses arguments and starts packet capturing.
// Author: Lukáš Píšek (xpisek02)
// File: MainClass.cpp

#include <iostream>
#include <cstring>
#include <pcap.h>
#include <csignal>

#include "ArgParser.h"
#include "PacketCapturing.h"

ArgParser* arg_parser;
PacketCapturing* packet_capturing;

//Handle the SIGINT signal (CTRL+C)
void signal_handling(int signal)
{
    if (signal == SIGINT)
    {
        if (arg_parser != nullptr)
        {
            delete arg_parser;
        }
        if (packet_capturing != nullptr)
        {
            delete packet_capturing;
        }
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    std::signal(SIGINT, signal_handling);
    std::string interface;
    std::string sort_by;
    std::string time;

    arg_parser = new ArgParser(argc, argv, &interface, &sort_by, &time);

    //If user inputs wrong arguments
    if (arg_parser->parse() == 1)
    {
        delete arg_parser;
        return 1;
    }
    
    packet_capturing = new PacketCapturing(interface, sort_by, time);

    //Usually if the interface name is wrong
    if (packet_capturing->start_capture() == 1)
    {
        delete arg_parser;
        delete packet_capturing;
        return 1;
    }

    delete arg_parser;
    delete packet_capturing;
    return 0;
}

