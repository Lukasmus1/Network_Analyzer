#include <pcap.h>
#include <iostream>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>

#include "PacketCapturing.h"
#include "PacketParser.h"

PacketCapturing::PacketCapturing(std::string interface, std::string sort_by)
{
    _interface = interface;
    _sort_by = sort_by;
}


void PacketCapturing::packet_handler(u_char* user, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    PacketParser* pp = new PacketParser(packet);
    PacketInfo pi = pp->parse_packet(pkthdr->len);
    

    std::cout << "Source IP: " << pi.source_ip << ":" << pi.source_port << "\n" << "Destination IP: " << pi.destination_ip << ":" << pi.destination_port << "\n" << "Protocol: " << pi.protocol << "\nSize: " << pi.size << "\n" << "\n";
}

PacketCapturing::~PacketCapturing() = default;

int PacketCapturing::start_capture()
{
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle = pcap_open_live(_interface.c_str(), BUFSIZ, 1, 1000, errbuf);

    if (handle == nullptr)
    {
        std::cerr << "An error has occured with opening an interface." << "\n";
        return 1;
    }

    pcap_loop(handle, 0, PacketCapturing::packet_handler, nullptr);


    pcap_close(handle);
    return 0;
}

//NCURSES