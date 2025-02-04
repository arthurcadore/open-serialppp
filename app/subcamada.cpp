#include <stdexcept>
#include "subcamada.h"

Subcamada::Subcamada(int fd, long tout) : Callback(fd, tout), superior(nullptr), inferior(nullptr) {

}

void Subcamada::conecta(Subcamada *acima) {
    if (acima == nullptr) throw std::runtime_error("camada superior não pode ser nullptr !");
    superior = acima;
    acima->inferior = this;
}

void Subcamada::envia(const vector<char>& quadro) override {
    // Implementation for sending data
    if (lower) {
        lower->envia(quadro);
    }
}

void Subcamada::recebe(const vector<char>& quadro) override {
    // Implementation for receiving data
    cout << "Received data: ";
    for (char c : quadro) {
        cout << c;
    }
    cout << endl;
}