#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

class Memory
{
public:
    Memory() = default;
    Memory(int start, int len);
    Memory(int start, int len, std::string status);
    Memory(const Memory& obj) = default;

    int start;
    int len;
    std::string status;
};

#endif // MEMORY_H
