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
    bpf_u_int32 rx;
    bpf_u_int32 tx;
};