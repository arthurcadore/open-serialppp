#ifndef ENQ_H
#define ENQ_H

#include "serial.h"
#include "subcamada.h"

class Enquadramento: public Subcamada {
 public:   
    // construtor 
    Enquadramento(Serial & porta_serial, double tout): Subcamada(porta_serial.get(), tout), dev(porta), n_bytes(0) {
    }
    
    void handle() {
      // lê um octeto da serial, e o armazena no buffer
      // Encaminha o buffer para camada superior, se ele tiver 8 octetos
      char octeto;
      
      dev.read(&octeto, 1);
      buffer[n_bytes] = octeto;
      n_bytes++;
      if (n_bytes == 8) {
        // envia o conteúdo do buffer para a camada superior (upper)
        upper->recebe(buffer, n_bytes);
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
      porta.write(&dados, len);
      porta.write(&flag, 1);
    }
    
    void recebe() {
      // nada faz, mas precisa definir se em Subcaada for um método abstrato ...
    }
 private:
   char buffer[MaxSize];
   int n_bytes;
   Serial & dev;
 };
 
 #endif