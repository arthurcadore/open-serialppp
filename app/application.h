#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "frame.h"

// Classe para a camada Application
class Application : public Subcamada
{
public:
    // Construtor
    Application(int fd, long tout) : Subcamada(fd, tout) {}

    // Método para enviar dados (implementação da Subcamada)
    void envia(Frame * Frame) override
        {
        // Envia os dados para a camada inferior (Framing)
        if (inferior)
        {
            inferior->envia(Frame);
        }
    }

    // Método para receber dados (implementação da Subcamada)
    void recebe(Frame * Frame) override
    {
        // Exibe os dados recebidos da camada inferior (Framing)
        std::cout << "Application received: " <<  Frame->getData() << std::endl;
    }

    // Método para ler dados do terminal e enviar para a camada inferior
    void handle() override
    {
        std::string input;

        // Recebe os dados do terminal
        std::getline(std::cin, input);

        if (input == "exit")
        {
            exit(0); // Encerra o programa se o usuário digitar "exit"
        }

        // instancia um novo quadro
        Frame frame(input);
        
        frame.addIdProto(idProto::MSG);

        // adiciona reservado ao quadro
        frame.addReserved(reserved::R);

        // Envia os dados para a camada inferior (Framing)
        envia(&frame);

        std::cout << "Open-SerialPPP: ";
    }

    // Método chamado em caso de timeout (não utilizado aqui)
    void handle_timeout() override {}

    void initialize()
    {
        std::cout << "Digite uma mensagem para enviar ou 'exit' para sair:" << std::endl;
    }
};

#endif // APPLICATION_H