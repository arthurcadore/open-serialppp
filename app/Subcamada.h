#ifndef SUBCAMADA_H
#define SUBCAMADA_H

#include <vector>
#include "../libs/callback.h"

using std::vector;

class Subcamada: public Callback {
public:
    Subcamada(int fd, long tout);
    ~Subcamada();
    void conecta(Subcamada * acima);
    virtual void envia(const vector<char> & quadro) = 0;
    virtual void recebe(const vector<char> & quadro) = 0;
protected:
    Subcamada * superior, * inferior;
};

#endif // SUBCAMADA_H
