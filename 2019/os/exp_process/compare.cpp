#include "compare.h"

bool Compare::operator()(const Process &obja, const Process &objb)
{
    return obja.priv != objb.priv ? obja.priv < objb.priv : obja.time < objb.time;
}
