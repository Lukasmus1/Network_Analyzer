// Description: Class for parsing user inputted arguments. Checks for correct arguments and saves them to predefined variables.
// Author: Lukáš Píšek (xpisek02)
// File: ArgParser.cpp

#include "ArgParser.h"

ArgParser::ArgParser(int argc, char* argv[], std::string* interface, std::string* sort_by, std::string* time)
{
    _argc = argc;
    _argv.assign(argv, argv + argc); 
    _interface = interface;
    _sort_by = sort_by;
    _time = time;
    *_time = "1";
}

//Not allocating anything here, so the destructor can be default
ArgParser::~ArgParser() = default;

//Checking next argument
std::string ArgParser::check_arg(const int i)
{
    if (i + 1 < _argc)
    {
        return _argv[i + 1];
    }
    
    return "ERROR";
}


int ArgParser::parse()
{
    //==========ARGUMENT PARSING==========
    for (int i = 1; i < _argc; i++)
    {
        if (_argv[i] == "-i")
        {
            *_interface = check_arg(i);
            i++; 
        }
        else if (_argv[i] == "-s")
        {
            *_sort_by = check_arg(i);
            i++;
        }
        else if (_argv[i] == "-t")
        {
            *_time = check_arg(i);
            i++;
        }
        else
        {
            std::cerr << "Wrong arguments\n";
            return 1;
        }
    }

    //==========ERROR CHECKING==========
    
    //Interface check
    if (_interface->empty())
    {
        std::cerr << "You must choose an interface!\n";
        return 1;
    }
    
    //If interface or sort_by or time is ERROR, crash.
    if (strcmp(_interface->c_str(), "ERROR") == 0 || strcmp(_sort_by->c_str(), "ERROR") == 0 || strcmp(_time->c_str(), "ERROR") == 0)
    {
        std::cerr << "Wrong arguments\n";
        return 1;
    }
    
    //If sort_by is not empty and not "p" or "b", crash.
    if(strcmp(_sort_by->c_str(), "p") != 0 && strcmp(_sort_by->c_str(), "b") != 0 && !_sort_by->empty())
    {
        std::cerr << "Argument '-s' must be followed by either 'p' or 'b'\n";
        return 1;
    }

    // If time is not a valid number, crash.
    if (!_time->empty())
    {
        try
        {
            float timeValue = std::stof(*_time);
            if (timeValue < 1)
            {
                *_time = "1";
            }
        }
        catch (const std::invalid_argument&)
        {
            std::cerr << "Argument '-t' must be followed by a valid number\n";
            return 1;
        }
        catch (const std::out_of_range&)
        {
            std::cerr << "Argument '-t' is out of range\n";
            return 1;
        }
    }

    return 0;
}
