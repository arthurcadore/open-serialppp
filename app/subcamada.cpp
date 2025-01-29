#include <stdexcept>
#include "subcamada.h"

Subcamada::Subcamada(int fd, long tout) : Callback(fd, tout), superior(nullptr), inferior(nullptr) {

}

void Subcamada::conecta(Subcamada *acima) {
    if (acima == nullptr) throw std::runtime_error("camada superior não pode ser nullptr !");
    superior = acima;
    acima->inferior = this;
}
