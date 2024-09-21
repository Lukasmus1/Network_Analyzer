#include <iostream>
#include <cstring>
#include <pcap.h>

#include "ArgParser.h"
#include "PacketCapturing.h"

int main(int argc, char* argv[])
{
    std::string interface;
    std::string sort_by;


    ArgParser* arg_parser = new ArgParser(argc, argv, &interface, &sort_by);

    if (arg_parser->parse() == 1)
    {
        delete arg_parser;
        return 1;
    }
    
    std::cout << "Interface: " << interface << "\n" << "Sort by: " << sort_by << "\n";

    PacketCapturing* packet_capturing = new PacketCapturing(interface, sort_by);

    if (packet_capturing->start_capture() == 1)
    {
        delete arg_parser;
        delete packet_capturing;
        return 1;
    }

    delete arg_parser;
    return 0;
}
