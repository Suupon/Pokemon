#ifndef INTERAGIR_HPP
#define INTERAGIR_HPP

#include <string>

class Interagir {
public:
    virtual ~Interagir() = default;
    virtual std::string interagir() const = 0;
};

#endif // INTERAGIR_HPP 