// Description: Header file for Output.cpp
// Author: Lukáš Píšek (xpisek02)
// File: Output.h

#pragma once
#include <vector>
#include <thread>
#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <condition_variable>

#include "PacketInfo.h"

class Output
{
public:
    Output(std::vector<PacketInfo>* packets, std::string time);
    std::mutex& get_mutex() { return _packetsLock; }
    ~Output();
private:
    void start_thread_output();
    void update_output();
    void start_thread_screen_size();
    void update_screen_size();
    void update_header();
    std::string _time;
    std::string format_speed(float speed);
    std::vector<PacketInfo>* _packets;
    int _maxX, _maxY;
    bool _runThreadOutput, _runThreadScreenSize;
    std::thread _threadOutput, _threadScreenSize;
    std::mutex _packetsLock;
    std::condition_variable _condVar;
    std::mutex _cvMutex;
};