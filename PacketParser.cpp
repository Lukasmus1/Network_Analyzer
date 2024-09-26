#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>
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

    u_int16_t type = ntohs(eth_header->ether_type);

    if (type == ETHERTYPE_IP)
    {
        const struct ip* ip_header = (struct ip*)(_packet + sizeof(struct ether_header));

        connection_info.source_ip = inet_ntoa(ip_header->ip_src);
        connection_info.destination_ip = inet_ntoa(ip_header->ip_dst);
        connection_info.protocol = getprotobynumber(ip_header->ip_p)->p_name;
        connection_info.size = packet_size;
        
        port_check(&connection_info);
    }
    else if (type == ETHERTYPE_IPV6)
    {
        const struct ip6_hdr* ip6 = (struct ip6_hdr*)(_packet + sizeof(struct ether_header));
        char src[INET6_ADDRSTRLEN];
        char dst[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &ip6->ip6_src, src, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &ip6->ip6_dst, dst, INET6_ADDRSTRLEN);

        connection_info.source_ip = src;
        connection_info.destination_ip = dst;
        connection_info.protocol = getprotobynumber(ip6->ip6_nxt)->p_name;
        connection_info.size = packet_size;
        
        port_check(&connection_info);
    }
    else if (type == ETHERTYPE_ARP)
    {
        const struct ether_arp* arp = (struct ether_arp*)(_packet + sizeof(struct ether_header));
        connection_info.source_ip = inet_ntoa(*(struct in_addr*)arp->arp_spa);
        connection_info.destination_ip = inet_ntoa(*(struct in_addr*)arp->arp_tpa);
        connection_info.protocol = "arp";
        connection_info.size = packet_size;
        connection_info.source_port = 0;
        connection_info.destination_port = 0;
    }
    else
    {
        connection_info.protocol = "unknown";
    }

    return connection_info;
}

void PacketParser::port_check(PacketInfo* connection_info)
{
    if (connection_info->protocol == "tcp")
    {
        const struct tcphdr* tcp = (struct tcphdr*)(_packet + sizeof(struct ether_header) + sizeof(struct ip));
        connection_info->source_port = ntohs(tcp->source);
        connection_info->destination_port = ntohs(tcp->dest);

    }
    else if (connection_info->protocol == "udp")
    {
        const struct udphdr* udp = (struct udphdr*)(_packet + sizeof(struct ether_header) + sizeof(struct ip));
        connection_info->source_port = ntohs(udp->source);
        connection_info->destination_port = ntohs(udp->dest);
    }
}