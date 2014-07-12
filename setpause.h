#include <time.h>

#ifndef SETPAUSE_H_

using namespace std;

int setpause (int value) {
struct timespec timeout = {value};
struct timespec timenout;
nanosleep (&timeout, &timenout);
return 0;
}

#endif
