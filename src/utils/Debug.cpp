#include "utils/Debug.h"

namespace Debug
{
    bool debug = false;

    void set(bool status)
    {
        debug = status;
    }

    bool status()
    {
        return debug;
    }
}
