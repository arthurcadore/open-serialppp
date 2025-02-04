#include "main.h"
#include "io.h" // Inclua o arquivo de cabeçalho para a classe Io

int main (int argc, char * argv[]) {
    // Verifique se os argumentos foram passados corretamente
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <TxPort> <RxPort> <baudrate>" << std::endl;
        return 1;
    }

    // Leia os argumentos e crie ponteiros char para as portas seriais Tx e Rx
    char * TxPort = argv[1];
    char * RxPort = argv[2];
    int baudrate = std::stoi(argv[3]); // Converta o baudrate para inteiro

    // Crie um objeto Io
    Io io(TxPort, RxPort, baudrate);

    Poller sched;
    TxCallback txCb;
    RxCallback rxCb;

    sched.adiciona(&txCb);
    sched.adiciona(&rxCb);
    sched.despache();

    return 0;
}