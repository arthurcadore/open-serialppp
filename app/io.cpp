#include "io.h"

std::vector<char> Io::uint8_tToChar (std::vector<uint8_t> buffer) {
    std::vector<char> charBuffer;
    for (auto & c: buffer) {
        charBuffer.push_back((char)c);
    }
    return charBuffer;
}

std::vector<uint8_t> Io::charToUint8_t (std::vector<char> buffer) {
    std::vector<uint8_t> uint8_tBuffer;
    for (auto & c: buffer) {
        uint8_tBuffer.push_back((uint8_t)c);
    }
    return uint8_tBuffer;
}

void Io::tx (std::vector<uint8_t> buffer) {
    // Converte o buffer de uint8_t para char
    std::vector<char> charBuffer = uint8_tToChar(buffer);

    // Envia a mensagem
    Tx.write(charBuffer);
}

std::vector<uint8_t> Io::rx () {
    // Recebe até 32 octetos
    std::vector<char> buffer = Rx.read(32);

    std::vector<uint8_t> uint8_tBuffer = charToUint8_t(buffer);
    
    return uint8_tBuffer;
}