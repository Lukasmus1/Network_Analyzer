#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h> 
#include <netinet/udp.h>


#include "PacketParser.h"
#include "PacketInfo.h"

PacketParser::PacketParser(const u_char* packet)
{
    _packet = packet;
}

PacketInfo PacketParser::parse_packet(bpf_u_int32 packet_size)
{
    PacketInfo connection_info;

    const struct ether_header* eth_header = (struct ether_header*)_packet;

    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP)
    {
        const struct ip* ip_header = (struct ip*)(_packet + sizeof(struct ether_header));

        connection_info.source_ip = inet_ntoa(ip_header->ip_src);
        connection_info.destination_ip = inet_ntoa(ip_header->ip_dst);
        connection_info.protocol = getprotobynumber(ip_header->ip_p)->p_name;
        connection_info.size = packet_size;

        if (connection_info.protocol == "tcp")
        {
            const struct tcphdr* tcp = (struct tcphdr*)(_packet + sizeof(struct ether_header) + sizeof(struct ip));
            connection_info.source_port = ntohs(tcp->source);
            connection_info.destination_port = ntohs(tcp->dest);

        }
        else if (connection_info.protocol == "udp")
        {
            const struct udphdr* udp = (struct udphdr*)(_packet + sizeof(struct ether_header) + sizeof(struct ip));
            connection_info.source_port = ntohs(udp->source);
            connection_info.destination_port = ntohs(udp->dest);
        }
    }

    return connection_info;
}
