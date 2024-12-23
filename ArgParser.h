﻿// Description: Header file for ArgParser.cpp
// Author: Lukáš Píšek (xpisek02)
// File: ArgParser.h

#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

class ArgParser
{
public:
    ArgParser(int argc, char* argv[], std::string* interface, std::string* sort_by, std::string* time);
    ~ArgParser();
    int parse();

private:
    std::string check_arg(int i);
    int _argc;
    std::vector<std::string> _argv;
    std::string* _interface;
    std::string* _sort_by;
    std::string* _time;
};
