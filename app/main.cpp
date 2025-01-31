#include "main.h"

int main (int argc, char * argv[]) {
    Poller sched;
    KeyboardRead keyboard;

    sched.adiciona(&keyboard);
    sched.despache();

    return 0;
}