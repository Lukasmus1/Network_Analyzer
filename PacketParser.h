#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "PacketInfo.h"

class PacketParser
{
public:
    PacketParser(const u_char* packet, std::string sort_by);
    PacketInfo parse_packet(bpf_u_int32 packet_size);
    void update_packet_list(PacketInfo connection_info, std::vector<PacketInfo>* packets);
    ~PacketParser();
private:
    const u_char* _packet;
    std::string _sort_by;
    void port_check(PacketInfo* connection_info);
    void sort_packets(std::vector<PacketInfo>* packets);
};

