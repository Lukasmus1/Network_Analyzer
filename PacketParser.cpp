// Description: Class that parses a given packet and updates the list of connections.
// Author: Lukáš Píšek (xpisek02)
// File: PacketParser.cpp

#include "PacketParser.h"
#include "PacketInfo.h"

PacketParser::PacketParser(const u_char* packet, std::string sort_by, Output* out)
{
    _packet = packet;
    _sort_by = sort_by;
    _out = out;
}

//Not allocating anything here, so the destructor can be default
PacketParser::~PacketParser() = default;

//Method for parsing packet and its information
PacketInfo PacketParser::parse_packet(bpf_u_int32 packet_size)
{
    PacketInfo connection_info;

    const struct ether_header* eth_header = (struct ether_header*)_packet;

    u_int16_t type = ntohs(eth_header->ether_type);

    //Checking if the packet is IPv4 or IPv6, other packets are ignored
    if (type == ETHERTYPE_IP)
    {
        //Getting IP header
        const struct ip* ip_header = (struct ip*)(_packet + sizeof(struct ether_header));

        //Parsing information from the packet
        connection_info.source_ip = inet_ntoa(ip_header->ip_src);
        connection_info.destination_ip = inet_ntoa(ip_header->ip_dst);
        connection_info.protocol = getprotobynumber(ip_header->ip_p)->p_name;
        connection_info.size = packet_size;
        
        //If the packet is UDP or TCP, get the port numbers
        port_check(&connection_info);
    }
    else if (type == ETHERTYPE_IPV6)
    {
        //Getting IPv6 header
        const struct ip6_hdr* ip6 = (struct ip6_hdr*)(_packet + sizeof(struct ether_header));
        
        //Getting IPv6 addresses
        char src[INET6_ADDRSTRLEN];
        char dst[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &ip6->ip6_src, src, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &ip6->ip6_dst, dst, INET6_ADDRSTRLEN);

        connection_info.source_ip = "[" + std::string(src) + "]";
        connection_info.destination_ip = "[" + std::string(dst) + "]";
        connection_info.protocol = getprotobynumber(ip6->ip6_nxt)->p_name;
        connection_info.size = packet_size;
        
        //If the packet is UDP or TCP, get the port numbers
        port_check(&connection_info);
    }
    else
    {
        connection_info.protocol = "unknown";
    }

    return connection_info;
}

//Method for getting the port numbers from the packet
void PacketParser::port_check(PacketInfo* connection_info)
{
    //Only TCP and UDP packets have port numbers
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
    else
    {
        //0 is a wildcard for no port
        connection_info->source_port = 0;
        connection_info->destination_port = 0;
    }
}

//Method for updating the list of connections
void PacketParser::update_packet_list(PacketInfo connection_info, std::vector<PacketInfo>* packets)
{
    //Locking the mutex, so I can't modify the packets while they are being updated
    std::unique_lock<std::mutex> lock(_out->get_mutex());

    //If the list is empty, add the connection
    if (packets->size() == 0)
    {
        packets->push_back(connection_info);
        packets->at(0).rx += connection_info.size;
        packets->at(0).packet_count++;
        sort_packets(packets);

        //Unlocking the mutex
        lock.unlock();
        return;
    }
    else
    {
        //Checking if the connection is already in the list
        for (PacketInfo& packet : *packets)
        {
            //If the connection is already in the list, update Rx
            if (packet.source_ip == connection_info.source_ip && packet.destination_ip == connection_info.destination_ip && 
                packet.source_port == connection_info.source_port && packet.destination_port == connection_info.destination_port && 
                packet.protocol == connection_info.protocol)
            {
                packet.rx += connection_info.size;
                packet.packet_count++;
                sort_packets(packets);
                
                //If the list is longer than 10, remove the last element
                if (packets->size() > 10)   
                {
                    packets->pop_back();
                }

                //Unlocking the mutex
                lock.unlock();
                return;
            }
            //If the connection is in the list, but the source and destination are switched, update Tx
            else if (packet.source_ip == connection_info.destination_ip && packet.destination_ip == connection_info.source_ip && 
                    packet.source_port == connection_info.destination_port && packet.destination_port == connection_info.source_port && 
                    packet.protocol == connection_info.protocol)
            {
                packet.tx += connection_info.size;
                packet.packet_count++;
                sort_packets(packets);

                //If the list is longer than 10, remove the last element
                if (packets->size() > 10)
                {
                    packets->pop_back();
                }

                //Unlocking the mutex
                lock.unlock();
                return;
            }
        }

        //If the connection is not in the list, add a new one
        packets->push_back(connection_info);
        packets->at(packets->size() - 1).rx += connection_info.size;
        packets->at(packets->size() - 1).packet_count++;
        sort_packets(packets);

        //If the list is longer than 10, remove the last element
        if (packets->size() > 10)
        {
            packets->pop_back();
        }

        //Unlocking the mutex
        lock.unlock();
    }
}

void PacketParser::sort_packets(std::vector<PacketInfo>* packets)
{
    //ArgParser makes sure that _sort_by is either "b" or "p"
    if (_sort_by == "p")
    {
        //Sort by packet count
        std::sort(packets->begin(), packets->end(), [](PacketInfo a, PacketInfo b) { return a.packet_count > b.packet_count; });
    }
    else
    {
        //Sort by Rx + Tx (byte count)
        std::sort(packets->begin(), packets->end(), [](PacketInfo a, PacketInfo b) { return a.rx + a.tx > b.rx + b.tx; });
    }
}