#ifndef INTERAGIR_HPP
#define INTERAGIR_HPP

#include <string>
using namespace std;

class Interagir {
public:
    virtual ~Interagir() = default;
    virtual string interagir() const = 0;
};

#endif // INTERAGIR_HPP 