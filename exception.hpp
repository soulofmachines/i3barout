#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

class errorExc : public std::exception {
private:
    const char* message;
public:
    errorExc(const char* why) : message(why) {}
    const char* what() const throw() {
        return message;
    }
};

#endif //PIDGET_HPP
