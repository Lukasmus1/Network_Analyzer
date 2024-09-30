// Description: Struct for storing information about a packet / connection.
// Author: Lukáš Píšek (xpisek02)
// File: PacketInfo.h

#pragma once
#include <string>
#include <pcap.h>

struct PacketInfo
{
    std::string source_ip;
    u_int16_t source_port;
    
    std::string destination_ip;
    u_int16_t destination_port;

    std::string protocol;
    u_int32_t packet_count; 
    bpf_u_int32 size;
    bpf_u_int32 rx;
    bpf_u_int32 tx;
};