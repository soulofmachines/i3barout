#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP

#include <string>
#include <yajl/yajl_tree.h>
#include <yajl/yajl_gen.h>

class classBase {
protected:
    int         integer,
    iconMax,
    iconNum,
    urgent;
    bool        urgentLow;
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
    name;
    void            readConfig(yajl_val &config);
    virtual void    readCustomConfig(yajl_val &config) = 0;
    virtual void    update() = 0;
    void            setColor();
    void setOutput(std::string input);
    void setIcon();
    std::string     show();
    void jsonAdd(yajl_gen &jsonOutput);
    std::string     showTerm();
    std::string     showTmux();
};

#endif //CLASSBASE_HPP
