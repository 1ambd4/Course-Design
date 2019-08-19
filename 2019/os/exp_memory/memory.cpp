#include "memory.h"

Memory::Memory(int start, int len)
{
    this->start = start;
    this->len = len;
    this->status = "F";
}

Memory::Memory(int start, int len, std::string status)
{
    this->start = start;
    this->len = len;
    this->status = status;
}
