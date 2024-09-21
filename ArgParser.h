#pragma once

#include <iostream>
#include <vector>

class ArgParser
{
public:
    ArgParser(int argc, char* argv[], std::string* interface, std::string* sort_by);
    ~ArgParser();
    int parse();

private:
    std::string check_arg(int i);
    int _argc;
    std::vector<std::string> _argv;
    std::string* _interface;
    std::string* _sort_by;
};
