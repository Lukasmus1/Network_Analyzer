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
pcap_t* PacketCapturing::_handle;
Output* PacketCapturing::out;

PacketCapturing::PacketCapturing(std::string interface, std::string sort_by)
{
    _interface = interface;
    _sort_by = sort_by;
    _handle = nullptr;
    out = nullptr;
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
    delete pp;
}

PacketCapturing::~PacketCapturing()
{
    if (_handle != nullptr)
    {
        pcap_breakloop(_handle);
        pcap_close(_handle);
    }
    if (out != nullptr)
    {
        delete out;
        out = nullptr;
    }
}

int PacketCapturing::start_capture()
{   
    char err[PCAP_ERRBUF_SIZE];

    _handle = pcap_open_live(_interface.c_str(), BUFSIZ, 1, 1000, err);

    if (_handle == nullptr)
    {
        std::cerr << "An error has occured with opening an interface." << "\n";
        return 1;
    }
    
    out = new Output(&_packets);

    pcap_loop(_handle, 0, PacketCapturing::packet_handler, nullptr);

    if (_handle != nullptr)
    {
        pcap_close(_handle);
    }
    return 0;
}