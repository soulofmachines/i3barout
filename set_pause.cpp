#include "set_pause.hpp"
#include <unistd.h>

using namespace std;

void set_pause (unsigned int value) {
    sleep (value);
}
