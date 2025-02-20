#ifndef FRAMING_H
#define FRAMING_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include <iomanip>  
#include <cstdint>
#include <iostream>
#include "../libs/crc16.h"
#include "../libs/serial.h"

#define FRAME_DELEMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Estados da FSM
enum FramingState {
    WAITING,    // Aguardando início do quadro
    RECEIVING   // Recebendo dados (trata escapes internamente)
};

// Eventos da FSM
class FramingEvent {
public:
    enum Type {
        BYTE_RECEIVED,  // Novo byte recebido
        TIMEOUT         // Timeout (opcional)
    };

    Type type;
    char data;  // Byte recebido

    FramingEvent(Type t, char d) : type(t), data(d) {}
};

class Framing : public Subcamada {
private: 
    Serial & serial;
    std::vector<char> buffer;
    FramingState estado; // Estado atual
    bool escaping = false; // Adicione esta flag
    bool frame_started = false; // Indica se o quadro começou

    void fsm(FramingEvent e); // Máquina de estados

public:
    Framing(Serial & porta, long tout) 
        : Subcamada(porta.get(), tout), serial(porta), estado(WAITING) {
        disable_timeout();  
    }

    void envia(Frame * frame) override;
    void recebe(Frame * frame) override;
    void handle_timeout() override;
    void interpreter(std::vector<char> & quadro);
    void handle() override;
};

#endif // FRAMING_H