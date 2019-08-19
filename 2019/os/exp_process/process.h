#ifndef PROCESS_SCHEDULING_H
#define PROCESS_SCHEDULING_H

#include <iostream>
#include <queue>

class Process
{
public:
    Process() = default;
    Process(std::string name, int time, int priv);
    Process(const Process& obj) = default;

    std::string name;
    int time;
    int priv;
    char state;
};

#endif // PROCESS_SCHEDULING_H
