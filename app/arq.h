#ifndef ARQ_H
#define ARQ_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "frame.h"

enum state {
    OCIOSO,
    ESPERA,
    ENVIANDO, 
    RECEBIDO
};


// Classe para a camada ARQ (Automatic Repeat reQuest)
class ARQ : public Subcamada
{
private:
    std::vector<Frame> buffer;
    bool frameSequence = 0;
    
public: 

    // Construtor
    ARQ (int fd, long tout) : Subcamada(fd, tout) {}

      void recebe(const Frame * frame){
      }

      void envia(Frame * Frame){
            // adiciona parâmetros ao quadro
            // Frame->addSequenceNumber(this->frameSequence);
            
            // // invertendo o bit de controle
            // this->frameSequence = !this->frameSequence;

            // adiciona o controle de bit ao quadro
            Frame->addControlBit(controlBit::DATA);

            // // adiciona o quadro ao buffer
            // this->buffer.push_back(*Frame);

            // envia o primeiro quadro do buffer
            if (inferior)
            {
                inferior->envia(&this->buffer[0]);
            }

      }


    void handle() override {}

    void handle_timeout() override {}

};



            // // verifica se o quadro é um ACK ou DATA
            // if (frame->getControlBit() == controlBit::ACK)
            // {
            //   // verifica se o quadro possui o número de sequência correto
            //   if (frame->getSequenceBit() == this->frameSequence)
            //   {
            //     // remove o quadro do buffer
            //     this->buffer.erase(this->buffer.begin());

            //     std::cout << "ACK recebido corretamente!" << std::endl;
            //   }
            //   // imprime mensagem de erro
            //   else
            //   {
            //     std::cout << "ACK recebido, mas quadro fora de ordem" << std::endl;
            //   }
            // } else {
            //     // Verifica o sequenciamento do quadro recebido
            //     frame->getSequenceBit();
            //     // Envia um ACK para o quadro recebido
            //     Frame ack;
            //     ack.addControlBit(controlBit::ACK);
            //     ack.addReserved(reserved::R);
            //     ack.addSequenceNumber(frame->getSequenceBit());

            //     // Envia o ACK para a camada inferior
            //     inferior->envia(&ack);

            //     // Envia o quadro para a camada superior
            //     superior->recebe(&frame);
            // } 

#endif // IO_H