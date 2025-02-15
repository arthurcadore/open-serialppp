#include "application.h"
#include "framing.h"
#include "../libs/poller.h"
#include "arq.h"

int main(int argc, char *argv[])
{
    // Verifica se o nome da porta serial foi fornecido
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <porta_serial>" << std::endl;
        return 1;
    }

    Serial porta(argv[1], B9600);

    // Instancia a subcamada da aplicação
    Application application(0, 0); // FD e timeout não são usados aqui

    ARQ arq(0, 0); // FD e timeout não são usados aqui

    // Instancia a subcamada do enquadramento
    Framing framing(porta, 0); // FD e timeout não são usados aqui

    // imprime a mensagem inicial
    application.initialize();


    arq.conecta(&application);
    framing.conecta(&arq);

    // Cria o poller e registra as subcamadas
    Poller sched;
    sched.adiciona(&application);
    // sched.adiciona(&arq);
    sched.adiciona(&framing);
    
    // Executa o protocolo (loop principal)
    sched.despache();

    return 0;
}