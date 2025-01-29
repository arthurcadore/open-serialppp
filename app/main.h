#ifndef __MAIN_H__
#define	__MAIN_H__

#include <iostream>
#include <cstring>
#include "serial.h"
#include "poller.h"
#include "aplicacao.h"
#include "enquadramento.h"

void tx (Serial & rf, char * msg);
void rx (Serial & rf);

int main (int argc, char * argv[]);


#endif    