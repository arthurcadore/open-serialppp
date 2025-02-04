#ifndef APLICACAO_H
#define APLICACAO_H

#include <iostream>
#include <string>
#include "subcamada.h"

using std::cin;
using std::cout;
using std::string;

class Aplicacao: public Subcamada {
  private:
    Subcamada* lower; // Declare as a member variable
 public:   
    // construtor 
    Aplicacao() : Subcamada(0, 1000), lower(nullptr) {}
    
    void handle() override {
      // lê uma linha do teclado
      string dados;
      
      getline(cin, dados);
      
      // envia para a subcamada inferior (lower)
      if (lower) {
        std::vector<char> quadro(dados.begin(), dados.end());
        lower->envia(quadro);
      }    
    }
    
    void handle_timeout();
    
    void envia(char * dados, int len);
    
    void recebe(char * dados, int len);
    
 };
 
 #endif