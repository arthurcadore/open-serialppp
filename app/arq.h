#ifndef ARQ_H
#define ARQ_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "frame.h"

// Classe para a camada ARQ (Automatic Repeat reQuest)
class ARQ : public Subcamada
{
private:
    int sequenceNumber = 0;
    std::vector<Frame> buffer;
    int frameCount = 0;
    
public: 

    // Construtor
    ARQ (int fd, long tout) : Subcamada(fd, tout) {}

      void recebe(const Frame * Frame){

      }

      void envia(Frame * Frame){
            // adiciona parâmetros ao quadro
            Frame->addSequenceNumber(this->frameCount);

            // adiciona CRC ao quadro
            Frame->addCRC();

            // adiciona o controle de bit ao quadro
            Frame->addControlBit(controlBit::DATA);

            frameCount++;
      }


    void handle() override {}

    void handle_timeout() override {}

};

#endif // IO_H