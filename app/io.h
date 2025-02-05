#ifndef IO_H
#define IO_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "../libs/serial.h"

// Classe para a camada IO
class Io : public Subcamada
{
private:
    Serial serial;

public:
    // Construtor
    Io(const char *port, int baudrate) : Subcamada(0, 0), serial(port, baudrate) {}

    // Método para enviar dados (implementação da Subcamada)
    void envia(const std::vector<char> &quadro) override;

    // Método para receber dados (implementação da Subcamada)
    void recebe(const std::vector<char> &quadro) override;

    // Método para ler dados da porta serial
    void handle()
    {
        // Lê dados da porta serial
        vector<char> receivedData = serial.read(32);

        // Envia os dados recebidos para a camada superior (Framing)
        if (superior)
        {
            superior->recebe(receivedData);
        }
    }

    // Método chamado em caso de timeout (não utilizado aqui)
    void handle_timeout() override {}
};

#endif // IO_H