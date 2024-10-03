// Description: Header file for PacketCapturing.cpp
// Author: Lukáš Píšek (xpisek02)
// File: PacketCapturing.h

#pragma once 
#include <string>
#include <pcap.h>
#include <iostream>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <vector>

#include "PacketInfo.h"
#include "Output.h"

class PacketCapturing
{
public:
    PacketCapturing(std::string interface, std::string sort_by);
    ~PacketCapturing();
    int start_capture();

private:
    static void packet_handler(u_char* user, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    static void signal_handling(int signal);
    std::string _interface;
    static std::string _sort_by;
    static Output* out;
    static std::vector<PacketInfo> _packets;
    static pcap_t* _handle;
};