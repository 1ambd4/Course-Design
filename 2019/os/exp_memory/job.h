#ifndef JOB_H
#define JOB_H


class Job
{
public:
    Job() = default;
    Job(int no, int start, int len);
    Job(const Job& obj) = default;

    int no;
    int start;
    int len;
};

#endif // JOB_H
