#include "main.h"

using namespace std;

void tx (Serial & rf, char * msg) {
  // esta é a mensagem a ser transmitida
  vector<char> buffer(msg, msg + strlen(msg));

  // envia a mensagem
  int n = rf.write(buffer);

  cout << "Enviou " << n << " bytes" << endl;
}

void rx (Serial & rf) {
  // recebe até 32 octetos
  auto buffer = rf.read(32);

  cout << "Recebeu " << buffer.size() << " bytes: ";

  // mostra na tela os caracteres recebidos
  cout.write(buffer.data(), buffer.size());

  cout << endl;
}

int main (int argc, char * argv[]) {
    
  Serial Tx(argv[1], B9600);
  Serial Rx(argv[1], B9600);

    while(true){
        char msg[100];
        cout << "Digite a mensagem a ser enviada: ";
        cin.getline(msg, 100);
        tx(Tx, msg);
        rx(Rx);
    }
}