#pragma once
#include <iostream>

#include "PacketInfo.h"

class PacketParser
{
public:
    PacketParser(const u_char* packet);
    PacketInfo parse_packet(bpf_u_int32 packet_size);
    ~PacketParser();
private:
    const u_char* _packet;
    void port_check(PacketInfo* connection_info);
};

