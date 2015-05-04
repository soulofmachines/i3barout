#ifndef CLASSRFKILL_HPP
#define CLASSRFKILL_HPP

#include "classbase.hpp"
#include <dirent.h>

class classRfkill : public classBase {
private:
    std::string device,
    on,
    off,
    name,
    dname,
    path;
    int state;
public:
    classRfkill();
    void readCustomConfig(yajl_val &config);
    void update();
};

#endif //CLASSRFKILL_HPP
