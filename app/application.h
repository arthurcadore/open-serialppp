#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "Subcamada.h"

// Classe para a camada Application
class Application : public Subcamada {
public:
    // Construtor
    Application(int fd, long tout) : Subcamada(fd, tout) {}
    

    // Método para enviar dados (implementação da Subcamada)
    void envia(const std::vector<char>& quadro) override {
        // Envia os dados para a camada inferior (Framing)
        if (inferior) {
            inferior->envia(quadro);
        }
    }

    // Método para receber dados (implementação da Subcamada)
    void recebe(const std::vector<char>& quadro) override {
        // Exibe os dados recebidos da camada inferior (Framing)
        std::cout << "Application received: ";
        for (char c : quadro) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

    // Método para ler dados do terminal e enviar para a camada inferior
    void handle() override {
        std::string input;
        std::getline(std::cin, input);

        // imprime os dados que estão sendo enviados
        std::cout << "Application sending: " << input << "\n" << std::endl;

        if (input == "exit") {
            exit(0); // Encerra o programa se o usuário digitar "exit"
        }

        // Converte a string para um vetor de char
        std::vector<char> quadro(input.begin(), input.end());

        // Envia os dados para a camada inferior (Framing)
        if (inferior) {
            inferior->envia(quadro);
        }
    }

    // Método chamado em caso de timeout (não utilizado aqui)
    void handle_timeout() override {}
};

#endif // APPLICATION_H