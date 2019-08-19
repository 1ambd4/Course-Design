#include "process.h"

Process::Process(std::string name, int time, int priv)
{
    this->name = name;
    this->time = time;
    this->priv = priv;
    this->state = 'R';
}
