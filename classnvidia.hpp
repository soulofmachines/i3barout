#ifndef CLASSNVIDIA_HPP
#define CLASSNVIDIA_HPP

#include "classbase.hpp"

class classNvidia : public classBase {
private:
    std::string exec,
    module,
    reader,
    status;
    FILE* fd;
    int pid;
    char buffer[32];
public:
    classNvidia();
    void readConfig(Json::Value &config);
    void update();
};

#endif //CLASSNVIDIA_HPP
