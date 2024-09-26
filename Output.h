#pragma once
#include <vector>

#include "PacketInfo.h"

class Output
{
public:
    Output(std::vector<PacketInfo>* packets);
    ~Output();
private:
    void start_thread_output();
    void update_output();
    void start_thread_screen_size();
    void update_screen_size();
    void update_header();
    std::string format_speed(bpf_u_int32 speed);
    std::vector<PacketInfo>* _packets;
    int _maxX, _maxY;
    bool _runThreadOutput, _runThreadScreenSize;
};