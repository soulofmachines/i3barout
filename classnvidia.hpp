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
    int pid,
    ok;
    char buffer[32];
public:
    classNvidia();
    void readCustomConfig(yajl_val &config);
    void update();
};

#endif //CLASSNVIDIA_HPP
