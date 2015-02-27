#ifndef PIDGET_HPP
#define PIDGET_HPP

#include <string>

int pidGetPid(std::string name);
std::string pidGetStatus(int pid, int &ok);
std::string pidGetStatusError(int ok);

#endif //PIDGET_HPP
