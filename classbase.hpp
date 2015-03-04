#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP

#include <string>
#include <jsoncpp/json/json.h>

class classBase {
protected:
    Json::Value jsonOutput;
    Json::FastWriter writer;
    int         integer,
    iconNum,
    urgent;
    bool        urgentAbove;
    std::string color,
    error,
    icon,
    iconName,
    iconExt,
    label,
    output;
public:
    bool        colored,
    json,
    tmux;
    std::string                 colorNormal,
    colorUrgent,
    colorWarning,
    separator,
    name;
    void            readConfig(Json::Value &config);
    virtual void    readCustomConfig(Json::Value &config) = 0;
    virtual void    update() = 0;
    void            setColor();
    void setOutput(std::string input);
    void setIcon();
    std::string     show();
    std::string     showJson();
    std::string     showTerm();
    std::string     showTmux();
};

#endif //CLASSBASE_HPP
