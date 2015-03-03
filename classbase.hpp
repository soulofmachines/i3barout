#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP

#include <string>
#include <jsoncpp/json/json.h>

class classBase {
protected:
    int         integer;
    std::string color,
    error,
    output;
public:
    bool        colored,
    json,
    tmux;
    std::string                 colorNormal,
    colorUrgent,
    colorWarning,
    label,
    name;
    void            readConfig(Json::Value &config);
    virtual void    readCustomConfig(Json::Value &config) = 0;
    virtual void    update() = 0;
    void            setColor();
    void setOutput(std::string input);
    std::string     show();
};

#endif //CLASSBASE_HPP
