#ifndef SUBCAMADA_H
#define SUBCAMADA_H

#include <vector>
#include "../libs/callback.h"
#include "frame.h"

using std::vector;

class Subcamada: public Callback {
public:
    Subcamada(int fd, long tout);
    Subcamada(long tout);
    ~Subcamada();
    void conecta(Subcamada * acima);
    virtual void envia(Frame * quadro) = 0;
    virtual void recebe(Frame * quadro) = 0;
protected:
    Subcamada * superior, * inferior;
};

#endif // SUBCAMADA_H
