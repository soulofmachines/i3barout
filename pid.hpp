#ifndef PID_HPP
#define PID_HPP

#include <string>

int pidGetPid(std::string name);
std::string pidGetStatus(int pid, int &ok);
std::string pidGetStatusError(int ok);

#endif //PID_HPP
