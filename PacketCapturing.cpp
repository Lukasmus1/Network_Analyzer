#include <pcap.h>
#include <iostream>

#include "PacketCapturing.h"

PacketCapturing::PacketCapturing(std::string interface, std::string sort_by)
{
    _interface = interface;
    _sort_by = sort_by;
}

void PacketCapturing::packet_handler(u_char* user, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    std::cout << "Packet captured\n";
}

PacketCapturing::~PacketCapturing() = default;

int PacketCapturing::start_capture()
{
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle = pcap_open_live(_interface.c_str(), BUFSIZ, 1, 1000, errbuf);

    std::cout << _interface << "\n";

    if (handle == nullptr)
    {
        std::cerr << "Problem with opening the interface\n";
        return 1;
    }

    pcap_loop(handle, 0, PacketCapturing::packet_handler, nullptr);


    pcap_close(handle);
    return 0;
}

//NCURSES