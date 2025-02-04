#ifndef IO_H
#define IO_H

#include <iostream>
#include <cstring>
#include "../libs/serial.h"

enum baudrate {
    B_9600 = 9600
};

class Io {
    private: 
        Serial Tx;
        Serial Rx;

        std::vector<char> uint8_tToChar (std::vector<uint8_t> buffer);
        std::vector<uint8_t> charToUint8_t (std::vector<char> buffer);

    public:
        Io (char * TxPort, char * RxPort, int baudrate) {
            // Verifique o baudrate
            if (baudrate != B_9600) {
                throw std::runtime_error("Baudrate not supported");
            }
            Tx = Serial(TxPort, B9600);
            Rx = Serial(RxPort, B9600);
        }

        // Transmite um buffer de uint8_t pela interface serial
        void tx (std::vector<uint8_t> buffer);

        // Recebe um buffer de uint8_t da interface serial
        std::vector<uint8_t> rx ();
};

#endif // IO_H