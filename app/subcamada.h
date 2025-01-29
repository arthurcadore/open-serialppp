#ifndef SUBCAMADA_H
#define	SUBCAMADA_H

#include <vector>
#include "callback.h"

using std::vector;

class Subcamada: public Callback {
public:
    Subcamada(int fd, long tout);
    ~Subcamada();
    void conecta(Subcamada * acima);
    virtual void envia(const vector<char> & quadro);
    virtual void recebe(const vector<char> & quadro);
protected:
    Subcamada * superior, * inferior;
};


#endif //ENQ_V0_SUBCAMADA_H
