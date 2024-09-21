#include <algorithm>
#include <iostream>
#include <cstring>

std::string check_arg(int i, int argc, char* argv[])
{
    if (i + 1 < argc)
    {
        return argv[i + 1];
    }
    
    return "ERROR";
}

int main(int argc, char* argv[])
{
    std::string interface;
    std::string sort_by;

    //==========ARGUMENT PARSING==========
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            interface = check_arg(i, argc, argv);
            i++; 
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            sort_by = check_arg(i, argc, argv);
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
    if (interface.empty())
    {
        std::cerr << "You must choose an interface!\n";
        return 1;
    }
    
    //If interface or sort_by is ERROR, crash.
    if (strcmp(interface.c_str(), "ERROR") == 0 || strcmp(sort_by.c_str(), "ERROR") == 0)
    {
        std::cerr << "Wrong arguments\n";
        return 1;
    }
    
    //If sort_by is not empty and not "p" or "b", crash.
    if(strcmp(sort_by.c_str(), "p") != 0 && strcmp(sort_by.c_str(), "b") != 0 && !sort_by.empty())
    {
        std::cerr << "Argument '-s' must be followed by either 'p' or 'b'\n";
        return 1;
    }

    std::cout << "Interface: " << interface << "\n" << "Sort by: " << sort_by << "\n";
    
    return 0;
}
