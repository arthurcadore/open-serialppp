#include "io.h"


std::vector<char> io::uint8_tToChar (std::vector<uint8_t> buffer) {
    std::vector<char> charBuffer;
    for (auto & c: buffer) {
        charBuffer.push_back((char)c);
    }
    return charBuffer;
}

std::vector<uint8_t> io::charToUint8_t (std::vector<char> buffer) {
    std::vector<uint8_t> uint8_tBuffer;
    for (auto & c: buffer) {
        uint8_tBuffer.push_back((uint8_t)c);
    }
    return uint8_tBuffer;
}

void io::tx (vector<uint8_t> buffer) {

    //change the uint8_t buffer to char buffer
    std::vector<char> charBuffer = uint8_tToChar(buffer);

    // send the message
    Tx.write(charBuffer);
}

std::vector<uint8_t> io::rx () {
    // receive up to 32 octets
    std::vector<char> buffer = Rx.read(32);

    std::vector<uint8_t> uint8_tBuffer = charToUint8_t(buffer);
    
    return uint8_tBuffer;
}





















// void tx (Serial & rf, char * msg) {
//   // esta é a mensagem a ser transmitida
//   std::vector<char> buffer(msg, msg + strlen(msg));

//   // envia a mensagem
//   int n = rf.write(buffer);

//   std::cout << "Enviou " << n << " bytes" << std::endl;
// }

// void rx (Serial & rf) {
//   // recebe até 32 octetos
//   auto buffer = rf.read(32);

//   std::cout << "Recebeu " << buffer.size() << " bytes: ";

//   // mostra na tela os caracteres recebidos
//   std::cout.write(buffer.data(), buffer.size());

//   std::cout << std::endl;
// }

// int main (int argc, char * argv[]) {
    
//   Serial Tx(argv[1], B9600);
//   Serial Rx(argv[1], B9600);

//     while(true){
//         char msg[100];
//         std::cout << "Digite a mensagem a ser enviada: ";
//         std::cin.getline(msg, 100);
//         tx(Tx, msg);
//         rx(Rx);
//     }
// }