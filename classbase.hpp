#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP

#include <string>
#include <yajl/yajl_tree.h>

class classBase {
protected:
//    Json::Value jsonOutput;
//    Json::FastWriter writer;
    yajl_val jsonOutput;
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
    void            readConfig(yajl_val &config);
    virtual void    readCustomConfig(yajl_val &config) = 0;
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
