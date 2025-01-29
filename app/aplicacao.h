#ifndef APLICACAO_H
#define APLICACAO_H

#include <iostream>
#include <string>
#include "subcamada.h"

using std::cin;
using std::cout;
using std::string;

class Aplicacao: public Subcamada {
 public:   
    // construtor 
    Aplicacao(): Subcamada(0) {
    }
    
    void handle() {
      // lê uma linha do teclado
      string dados;
      
      getline(cin, dados);
      
      // envia para a subcamada inferior (lower)
      lower->envia(dados.c_str(), dados.size());
    }
    
    void handle_timeout();
    
    void envia(char * dados, int len);
    
    void recebe(char * dados, int len);
    
 };
 
 #endif