#include "framing.h"

/*
Flags: 

7E: Start of frame (frame delimiter)
7D: Escape character (next character will be escaped)

Each character in the message that will be escaped will be XORed with 0x20, so the character 0x7E will be escaped as 0x7D 0x5E for example. 
*/

std::vector<uint8_t> Framing::serialize( std::string message ) {

    // calculate the CRC16 for the message
    std::string messageWithCRC = calculateCRC(message);

    packet.clear();
    packet.push_back(FRAME_DELEMITER); // Start of frame


    for (int i = 0; i < messageWithCRC.length(); i++) {
        if (messageWithCRC[i] == FRAME_DELEMITER) {
            packet.push_back(ESCAPE_CHARACTER);
            packet.push_back(0x5E);
        } else if (messageWithCRC[i] == ESCAPE_CHARACTER) {
            packet.push_back(ESCAPE_CHARACTER);
            packet.push_back(0x5D);
        }
        else {
            packet.push_back(messageWithCRC[i]);
        }
    }

    packet.push_back(FRAME_DELEMITER); // End of frame

    return packet;
}

std::string Framing::deserialize(std::vector<uint8_t> packet){
    message.clear();

    // find the start of the frame in the packet, if it not found return an empty string
    int i = 0;
    while (packet[i] != FRAME_DELEMITER) {
        if (i == packet.size()) {
            throw std::runtime_error("Start of frame not found");
        }
        i++;
    }

    i++; // skip the start of frame

    // while until the end of the frame or 7E is found in the packet
    // if the end of the frame is not found return an error; 

    while (packet[i] != FRAME_DELEMITER) {
        if (i == packet.size()) {
            throw std::runtime_error("End of frame not found");
        }

        if (packet[i] == ESCAPE_CHARACTER) {
            i++; // skip the escape character
            if (packet[i] == 0x5E) {
                message.push_back(FRAME_DELEMITER);

            } else if (packet[i] == 0x5D) {
                message.push_back(ESCAPE_CHARACTER);
            }
        } else {
            message.push_back(packet[i]);
        }
        i++;
    }

    // remove the CRC from the message
    message = removeCRC(message);

    return message;
}

std::string Framing::packetToString() {
    std::string packetString;
    for (int i = 0; i < packet.size(); i++) {
        packetString.push_back(packet[i]);
    }
    return packetString;
}

std::string Framing::calculateCRC(std::string message) {
    auto crc = make_crc16(message);
    
    // generate the CRC16 for the packet
    crc.generate_into(message);

    return message;
}

std::string Framing::removeCRC(std::string message) {

    auto crc = make_crc16(message);
    
    // check the CRC of the message, if it is correct remove the CRC from the message
    // if not, return an error

    if (crc.check()) {
        message.pop_back();
        message.pop_back();
    } else {
        throw std::runtime_error("CRC16 check failed");
    }

    return message;
}
