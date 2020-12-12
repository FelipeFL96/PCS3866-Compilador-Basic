#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <string>

namespace syntax {

class BStatement {
    int index;
};

class Assign {
    std::string identifier;
};

class Number {
public:
    bool negative = 0;
    int integer = 0;
    int fraction = 0;
    bool exponent_negative = 0;
    int exponent = 1;

    double generate() {
        double num = 0.0;
        num = integer;

        double frac = fraction;
        while (frac > 1) {
            frac /= 10;
        }
        num = frac + num;

        if (negative)
            num *= -1;

        if (exponent_negative) {
            for (int i = 0; i < exponent; i++) {
                num /= 10;
            }
        }
        else {
            for (int i = 0; i < exponent; i++) {
                num *= 10;
            }
        }
        return num;
    }
};

} // namespace syntax

#endif // SYNTAX_HPP