#ifndef ARQ_H
#define ARQ_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "frame.h"
#include <iostream>

enum State {
    OCIOSO,
    ESPERA,
};

class Event {
    public:
    enum Type {
        TIMEOUT,
        RECEIVED,
    };

    Type type;
    Frame frame;

    Event(Type t, Frame f) : type(t), frame(f) {}
};

// Classe para a camada ARQ (Automatic Repeat reQuest)
class ARQ : public Subcamada
{
private:
    std::vector<Frame> buffer;
    bool frameSequence = 0;
    State estado = OCIOSO;  // Estado inicial

    
public: 

    // Construtor
    ARQ (long tout) : Subcamada(tout) {
        this->enable_timeout();
    }

      void recebe(Frame *frame) {
        fsm(Event(Event::RECEIVED, *frame));
      }


      void envia(Frame * frame){
        if (estado == OCIOSO) {
            frame->addControlBit(controlBit::DATA);
            frame->addSequenceNumber(this->frameSequence);

            // Adiciona ao buffer e envia
            buffer.push_back(*frame);
            if (inferior) {
                inferior->envia(&this->buffer[0]);
            }
            estado = ESPERA;
        } else {
             buffer.push_back(*frame);
            std::cout << "Mensagem enfilerada\n";
        }
      };


    void handle() override {
    }


    void handle_timeout() override {
        fsm(Event(Event::TIMEOUT, Frame()));
    }

      void fsm(Event e) {
        switch (estado) {
            case OCIOSO:
                if (e.type == Event::RECEIVED) {
                    if (e.frame.getControlBit() == controlBit::ACK) {
                        std::cout << "ACK inesperado, ignorando...\n";
                    } else {
                        Frame ack;
                        ack.addControlBit(controlBit::ACK);
                        ack.addSequenceNumber(e.frame.getSequenceBit());

                        // Envia ACK
                        inferior->envia(&ack);
                        // Passa o quadro para a camada superior
                        superior->recebe(&e.frame);
                    }
                }
                break;

            case ESPERA:
                if (e.type == Event::RECEIVED) {
                    if (e.frame.getControlBit() == controlBit::ACK) {
                        if (e.frame.getSequenceBit() == this->frameSequence) {
                            // Remove o quadro confirmado do buffer
                            this->buffer.erase(this->buffer.begin());
                            std::cout << "ACK correto recebido!\n";
                            estado = OCIOSO;
                        } else {
                            std::cout << "ACK fora de ordem, ignorado.\n";
                        }
                    }
                } else if (e.type == Event::TIMEOUT) {
                    std::cout << "Timeout! Reenviando quadro...\n";
                    if (!buffer.empty()) {
                        inferior->envia(&buffer[0]);
                    }
                }
                break;

            default:
                std::cout << "Estado desconhecido.\n";
                break;
        }
    }

};
#endif // IO_H