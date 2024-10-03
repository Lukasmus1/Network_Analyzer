// Description: Header file for PacketParser.cpp
// Author: Lukáš Píšek (xpisek02)
// File: PacketParser.h

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <pcap.h>
#include <algorithm>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>
#include <netinet/tcp.h> 
#include <netinet/udp.h>
#include <mutex>

#include "PacketInfo.h"
#include "Output.h"

class PacketParser
{
public:
    PacketParser(const u_char* packet, std::string sort_by, Output* out);
    PacketInfo parse_packet(bpf_u_int32 packet_size);
    void update_packet_list(PacketInfo connection_info, std::vector<PacketInfo>* packets);
    ~PacketParser();
private:
    const u_char* _packet;
    std::string _sort_by;
    void port_check(PacketInfo* connection_info);
    void sort_packets(std::vector<PacketInfo>* packets);
    Output* _out;
};

