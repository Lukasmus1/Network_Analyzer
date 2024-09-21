#pragma once 
#include <string>

class PacketCapturing
{
public:
    PacketCapturing(std::string interface, std::string sort_by);
    ~PacketCapturing();
    int start_capture();

private:
    std::string _interface;
    std::string _sort_by;
    static void packet_handler(u_char* user, const struct pcap_pkthdr* pkthdr, const u_char* packet);
};