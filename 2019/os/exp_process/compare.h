#ifndef COMPARE_H
#define COMPARE_H

#include "process.h"


class Compare
{
public:
    Compare() = default;
    ~Compare() = default;

    Compare(const Compare& obj) = default;

    bool operator() (const Process& obja, const Process& objb);
};

#endif // COMPARE_H
