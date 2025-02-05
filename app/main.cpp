#include "application.h"
#include "framing.h"
#include "io.h"
#include "../libs/poller.h"

int main(int argc, char *argv[]) {
    // Verifica se o nome da porta serial foi fornecido
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <porta_serial>" << std::endl;
        return 1;
    }

    // Cria a porta serial, cujo nome é informado via argv[1]
    Io io(argv[1], 9600); // Substitua B_9600 pela taxa de transmissão desejada

    // Instancia a subcamada do enquadramento
    Framing framing(0, 0); // FD e timeout não são usados aqui

    // Instancia a subcamada da aplicação
    Application application(0, 0); // FD e timeout não são usados aqui

    // Conecta as subcamadas: aplicação acima de enquadramento, enquadramento acima de IO
    application.conecta(&framing);
    framing.conecta(&io);

    // Cria o poller e registra as subcamadas
    Poller sched;
    sched.adiciona(&application);
    sched.adiciona(&framing);
    sched.adiciona(&io);

    // Executa o protocolo (loop principal)
    sched.despache();

    return 0;
}