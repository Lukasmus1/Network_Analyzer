// Description: Class that handles outputting packet information to the console using ncurses. It uses threads for updating the screen.
// Author: Lukáš Píšek (xpisek02)
// File: Output.cpp 

#include <ncurses.h>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>

#include "Output.h"


Output::Output(std::vector<PacketInfo>* packets)
{
    _packets = packets;

    //Ncurses initialization
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    //Starting threads
    start_thread_screen_size();
    start_thread_output();
}

Output::~Output()
{
    //Stopping loops in threads
    _runThreadOutput = false;
    _runThreadScreenSize = false;

    //Joining threads back to main thread
    if (_threadOutput.joinable())
    {
        _threadOutput.join();
    }
    if (_threadScreenSize.joinable())
    {
        _threadScreenSize.join();
    }

    //Ncurses deinitialization
    endwin();
}

//Method for outputting connection information to the console
void Output::update_output()
{
    while (_runThreadOutput)
    {
        for(std::size_t i = 0; i < _packets->size(); i++)
        {
            //Clear the line before printing current connection info
            move(i + 2, 0);
            clrtoeol();

            //Formatting space between columns
            int distanceX = _maxX / 7;
            
            //Printing connection info
            PacketInfo packet = _packets->at(i);

            //If the port is 0, print only the IP
            if (packet.source_port == 0)
            {
                mvprintw(i + 2, 0, "%s", packet.source_ip.c_str());
            }
            else
            {
                mvprintw(i + 2, 0, "%s:%d", packet.source_ip.c_str(), packet.source_port);
            }

            //Might be a redundant check, but just in case
            if (packet.destination_port == 0)
            {
                mvprintw(i + 2, distanceX * 2, "%s", packet.destination_ip.c_str());
            }
            else
            {
                mvprintw(i + 2, distanceX * 2, "%s:%d", packet.destination_ip.c_str(), packet.destination_port);
            }
        
            mvprintw(i + 2, distanceX * 4, "%s", packet.protocol.c_str());
            mvprintw(i + 2, distanceX * 5, "%s", format_speed(packet.rx).c_str());
            mvprintw(i + 2, distanceX * 6, "%s", format_speed(packet.tx).c_str());
            
            //Refresh the speed and packet count
            _packets->at(i).rx = 0;
            _packets->at(i).tx = 0;
            _packets->at(i).packet_count = 0;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

//Method for formatting the raw speed to a more readable format (Kb, Mb, Gb)
std::string Output::format_speed(bpf_u_int32 speed)
{
    //Using stringstream for limiting the speed to one decimal place
    std::stringstream ss; 
    ss << std::fixed << std::setprecision(1);
    
    if (speed < 1024)
    {
        return std::to_string(speed) + "b";
    }
    else if (speed < 1024 * 1024)
    {
        float speedK = (float)speed / 1024;
        ss << speedK;
        return ss.str() + "Kb";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        float speedM = (float)speed / (1024 * 1024);
        ss << speedM;
        return ss.str() + "Mb";
    }
    else
    {
        float speedG = (float)speed / (1024 * 1024 * 1024);
        ss << speedG;
        return ss.str() + "Gb";
    }
}

//Method for updating the header of the console
void Output::update_header()
{
    //Clear header
    move(0, 0);
    clrtoeol();

    //Clear header spacing
    move(1, 0);
    clrtoeol();

    //Print header
    int distanceX = _maxX / 7;
    mvprintw(0, 0, "Source IP:port");
    mvprintw(0, distanceX * 2, "Destination IP:port");
    mvprintw(0, distanceX * 4, "Protocol");
    mvprintw(0, distanceX * 5, "Rx/s");
    mvprintw(0, distanceX * 6, "Tx/s");
    refresh();
}

//Method for starting the thread to output connection information to the console
void Output::start_thread_output()
{
    _runThreadOutput = true;
    _threadOutput = std::thread(&Output::update_output, this);
}

//Method for getting the screen size to make the console responsive
void Output::update_screen_size()
{
    while (_runThreadScreenSize)
    {
        getmaxyx(stdscr, _maxY, _maxX);
        update_header();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

//Method for starting the thread to get the screen size
void Output::start_thread_screen_size()
{
    _runThreadScreenSize = true;
    _threadScreenSize = std::thread(&Output::update_screen_size, this);
}