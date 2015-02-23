#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP

#include <string>
#include <jsoncpp/json/json.h>

class classBase {
protected:
    bool        colored,
                json,
                tmux;
    int         integer;
    std::string color,
                colorNormal,
                colorUrgent,
                colorWarning,
                error,
                name,
                output;
public:
    virtual void    readConfig(Json::Value &config) = 0;
    virtual void    update() = 0;
    std::string     show();
};

#endif //CLASSBASE_HPP
