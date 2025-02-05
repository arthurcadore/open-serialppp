#include "main.h"

using namespace std;


int main (int argc, char * argv[]) {
    Framing framing;
    string message = argv[1];

    vector<uint8_t> packet = framing.serialize(message);

    cout << "Message: " << message << endl;
    // cout << "Packet: " << framing.packetToString() << endl;  
    
    string deserialized = framing.deserialize(packet);

    cout << "Deserialized: " << deserialized << endl;


    return 0;
}