#include "io.h"

using namespace std;

void Io::envia(const vector<char>& quadro) {
    // Envia os dados pela porta serial
    serial.write(quadro);
}

void Io::recebe(const vector<char>& quadro) {
    // Envia os dados para a camada superior (Framing)
    if (superior) {
        superior->recebe(quadro);
    }
}

