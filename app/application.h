#ifndef IO_H
#define IO_H

#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>  
#include <cstdint>

#include "../libs/callback.h"

// Function to dump the buffer to the output stream
void dump(const std::vector<uint8_t> & buffer, std::ostream & out);

// Class for reading from the keyboard and writing to the screen
class KeyboardRead : public Callback {
public:
    KeyboardRead(): Callback(0,0) {
        disable_timeout();
    }

    void handle() {
        std::string newLine;
        std::getline(std::cin, newLine);

        if (newLine == "exit") {
            exit(0);
        }

        std::cout << "You entered: " << newLine << std::endl;
    }

    void handle_timeout() {}
};

#endif