#ifndef ENQ_H
#define ENQ_H

#include "serial.h"
#include "subcamada.h"

const int MaxSize = 1024; // Define globally or as static const

class Enquadramento: public Subcamada {
 private:
  Serial& porta; // Declare as a member variable
  Subcamada* upper; // Declare as a member variable
  char buffer[MaxSize];
  int n_bytes;
  long timeout;
 public:   
    // construtor 
    Enquadramento(Serial& porta_serial, long tout)
        : Subcamada(porta_serial.get(), tout), porta(porta_serial), timeout(tout){}
        
    void handle() override {
      // lê um octeto da serial, e o armazena no buffer
      // Encaminha o buffer para camada superior, se ele tiver 8 octetos
      char octeto;
      
      porta.read(&octeto, 1);
      buffer[n_bytes] = octeto;
      n_bytes++;
      if (n_bytes == 8) {
        // envia o conteúdo do buffer para a camada superior (upper)
        if (upper) {
            std::vector<char> quadro(buffer, buffer + n_bytes);
            upper->recebe(quadro);

        }
        n_bytes = 0;
      }
    }
    
    void handle_timeout() {
      // Limpa o buffer se ocorrer timeout
      n_bytes = 0;
    }
    
    void envia(char * dados, int len) {
      // Apenas envia os dados pela serial
      // Este método é chamado pela subcamada superior
      static char flag = 0x7e;
      
      porta.write(&flag, 1);
      std::vector<char> buffer(dados, dados + len);
      porta.write(buffer);
      porta.write(&flag, 1);
    }
    
    void recebe() {
      // nada faz, mas precisa definir se em Subcaada for um método abstrato ...
    }

 };
 
 #endif