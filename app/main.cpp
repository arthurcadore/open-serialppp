#include "application.h"
#include "framing.h"
#include "../libs/poller.h"

int main(int argc, char *argv[]) {
    // Verifica se o nome da porta serial foi fornecido
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <porta_serial>" << std::endl;
        return 1;
    }

    // Instancia a subcamada do enquadramento
    Framing framing(argv[1], 9600, 0, 0); // FD e timeout não são usados aqui

    // Instancia a subcamada da aplicação
    Application application(0, 0); // FD e timeout não são usados aqui

    // Conecta as subcamadas: aplicação acima de enquadramento, enquadramento acima de IO
    framing.conecta(&application);

    // Cria o poller e registra as subcamadas
    Poller sched;
    sched.adiciona(&application);
    // sched.adiciona(&framing);

    // Executa o protocolo (loop principal)
    sched.despache();

    return 0;
}