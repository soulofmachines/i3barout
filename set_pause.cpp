#include <time.h>

using namespace std;

int set_pause (int value) {
struct timespec timeout = {value};
struct timespec timenout;
nanosleep (&timeout, &timenout);
return 0;
}
