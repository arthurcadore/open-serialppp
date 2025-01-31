#include "application.h"

void dump(const std::vector<uint8_t> & buffer, std::ostream & out) {
    int n = 0;

    out << std::hex << std::setprecision(2);
    for (auto & c: buffer) {
        int x = (unsigned char)c;
        out << x << " ";
        n++;
        if ((n % 20) == 0) out << std::endl;
    }
}