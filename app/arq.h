#ifndef ARQ_H
#define ARQ_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "frame.h"

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
    ARQ (int fd, long tout) : Subcamada(fd, tout) {
  
    }

      void recebe(Frame * frame){
        switch (estado) {
            case ESPERA:
                if (frame->getControlBit() == controlBit::ACK) {
                    if (frame->getSequenceBit() == this->frameSequence) {
                        // Remove o quadro confirmado do buffer
                        this->buffer.erase(this->buffer.begin());
                        std::cout << "ACK correto recebido!\n";
                        estado = OCIOSO;  // Volta ao estado inicial
                    } else {
                        std::cout << "ACK fora de ordem, ignorado.\n";
                    }
                } else { // Recebeu um quadro de dados
                    Frame ack;
                    ack.addControlBit(controlBit::ACK);
                    ack.addSequenceNumber(frame->getSequenceBit());

                    // Envia ACK
                    inferior->envia(&ack);
                    // Passa o quadro para a camada superior
                    superior->recebe(frame);
                    estado = RECEBIDO;
                }
                break;

            case RECEBIDO:
                // Aguarda próximo quadro
                estado = OCIOSO;
                break;

            default:
                std::cout << "Quadro recebido inesperadamente no estado atual.\n";
                break;
        }
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
            std::cout << "Tentativa de envio em estado inválido.\n";
        }
      };


    void handle() override {
    }


    void handle_timeout() override {
        if (estado == ESPERA) {
            std::cout << "Timeout! Reenviando quadro...\n";
            if (!buffer.empty()) {
                inferior->envia(&buffer[0]);
            }
        }
    }

};


void fsm(Evento e)
#endif // IO_H