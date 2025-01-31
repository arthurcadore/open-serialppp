#ifndef __FRAMING_H__
#define	__FRAMING_H__

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdint>
#include "../libs/crc16.h"

// define the characters that will be used for framing
#define FRAME_DELEMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Class for framing messages into packets to be sent over the serial interface
class Framing {
    private:
    std::vector<uint8_t> packet;
    std::string message;
    int packetSize;

    std::string calculateCRC(std::string message);

    std::string removeCRC(std::string message); 

    public:
    // Constructor
    Framing () {
        packet.clear();
        message.clear();
    }

    std::vector<uint8_t> serialize( std::string message ); 

    std::string deserialize( std::vector<uint8_t> packet );

    std::string packetToString();

    int getPacketSize( std::vector<uint8_t> packet );
};


#endif
