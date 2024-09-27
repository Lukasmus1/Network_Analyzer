#include <pcap.h>
#include <iostream>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <vector>

#include "PacketCapturing.h"
#include "PacketParser.h"
#include "Output.h"

std::string PacketCapturing::_sort_by;
std::vector<PacketInfo> PacketCapturing::_packets;

PacketCapturing::PacketCapturing(std::string interface, std::string sort_by)
{
    _interface = interface;
    _sort_by = sort_by;
}


void PacketCapturing::packet_handler(u_char*, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{    
    PacketParser* pp = new PacketParser(packet, _sort_by);
    PacketInfo pi = pp->parse_packet(pkthdr->len);
    if (pi.protocol == "unknown")
    {
        delete pp;
        return;
    }

    pp->update_packet_list(pi, &_packets);

}

PacketCapturing::~PacketCapturing() = default;

int PacketCapturing::start_capture()
{
    out = new Output(&_packets);

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