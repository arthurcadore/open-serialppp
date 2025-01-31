#include "main.h"
#include <iomanip>  

using namespace std;

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

int main (int argc, char * argv[]) {
    Framing framing;
    string message = argv[1];

    vector<uint8_t> packet = framing.serialize(message);

    dump(packet, std::cout);
    std::cout << std::endl;

    string messageDeserialized = framing.deserialize(packet);

    std::cout << messageDeserialized << std::endl;

    return 0;
}