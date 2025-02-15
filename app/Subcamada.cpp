#include <stdexcept>
#include "Subcamada.h"

Subcamada::Subcamada(int fd, long tout) : Callback(fd, tout), superior(nullptr), inferior(nullptr) {

}

Subcamada::Subcamada(long tout) : Callback(tout), superior(nullptr), inferior(nullptr) {

}

void Subcamada::conecta(Subcamada *acima) {
    if (acima == nullptr) throw std::runtime_error("camada superior não pode ser nullptr !");
    superior = acima;
    acima->inferior = this;
}

Subcamada::~Subcamada() {
    if (superior) {
        superior->inferior = nullptr;
    }
    if (inferior) {
        inferior->superior = nullptr;
    }
}