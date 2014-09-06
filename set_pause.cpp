#include "set_pause.hpp"
#include <time.h>

using namespace std;

void set_pause (unsigned int value) {
    struct timespec timeout = {value};
    struct timespec timenout;
    nanosleep (&timeout, &timenout);
}
