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
    bpf_u_int32 size;
    u_int32_t packet_count_rx;
    bpf_u_int32 speed_rx;
    u_int32_t packet_count_tx;
    bpf_u_int32 speed_tx;
};