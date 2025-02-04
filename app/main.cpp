#include "main.h"

using namespace std;


int main (int argc, char * argv[]) {
    
  Serial serial(argv[1], B9600);

  Aplicacao app;
  Enquadramento enq(serial, 9600);
  
  enq.conecta(&app);

  Poller sched;
  sched.adiciona(&enq);
  sched.adiciona(&app);

  sched.despache();
}