#include "aplicacao.h"

void handle_timeout() {
  // nada faz, mas precisa definir se em Callback for um método abstrato ...
}

void envia(char * dados, int len) {
  // nada faz, mas precisa definir se em Subcamada for um método abstrato ...
}

void recebe(char * dados, int len) {
  // mostra na tela os dados recebidos da subcamada inferior
  cout << "RX: ";
  cout.write(dados, len);
  cout << endl;
}