#ifndef IO_H
#define IO_H

#include <iostream>
#include <cstring>
#include "../libs/serial.h"

void tx (Serial & rf, char * msg);
void rx (Serial & rf);

enum baudrate {
    B_9600 = 9600
};

class io {
    private: 
        Serial Tx;
        Serial Rx;

        std::vector<char> uint8_tToChar (std::vector<uint8_t> buffer);
        std::vector<uint8_t> charToUint8_t (std::vector<char> buffer);

    public:

        io (char * TxPort, char * RxPort, int baudrate) {
            // check the baudrate
            if (baudrate != B_9600) {
                throw std::runtime_error("Baudrate not supported");
            }
                Tx = Serial(TxPort, B9600);
                Rx = Serial(RxPort, B9600);
        }

        /*
            Transmit a buffer of uint8_t over the serial interface
        */
        void tx (vector<uint8_t> buffer);

        /*
            Receive a buffer of uint8_t from the serial interface
        */
        std::vector<uint8_t> rx ();
};


#endif