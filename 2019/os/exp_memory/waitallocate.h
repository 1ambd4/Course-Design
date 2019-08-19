#ifndef WAITALLOCATE_H
#define WAITALLOCATE_H


class WaitAllocate
{
public:
    WaitAllocate() = default;
    WaitAllocate(int no, int need);
    WaitAllocate(const WaitAllocate& obj) = default;

    int no;
    int need;
};

#endif // WAITALLOCATE_H
