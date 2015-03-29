#ifndef PID_HPP
#define PID_HPP

#include <string>
#include <vector>

class classInput {
public:
    std::string name,
    exec1,
    exec2,
    exec3;
};

void inputProtocol(std::vector<classInput> inputV);
void inputStop();

#endif //PID_HPP
