#include "classbase.hpp"

std::string classBase::show() {
    update();
    if (!error.empty())
        return error;
    return output;
}
