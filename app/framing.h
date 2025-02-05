#ifndef FRAMING_H
#define FRAMING_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include "../libs/crc16.h"

// Define os caracteres usados para o framing
#define FRAME_DELEMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Classe para a camada Framing
class Framing : public Subcamada
{
public:
    // Construtor
    Framing(int fd, long tout) : Subcamada(fd, tout) {}

    // Método para enviar dados (implementação da Subcamada)
    void envia(const std::vector<char> &quadro) override;

    // Método para receber dados (implementação da Subcamada)
    void recebe(const std::vector<char> &quadro) override;

    // Método chamado em caso de timeout
    void handle_timeout() {}

    // Método chamado em caso de dados recebidos
    void handle()
    {
        // Se receber do IO desenquadra e envia para a camada superior
        vector<char> quadro;
        if (inferior)
        {
            inferior->recebe(quadro);
        }

        // Se receber da camada superior enquadra e envia para o IO
        if (superior)
        {
            superior->envia(quadro);
        }
    }

private:
    // Calcula o CRC16 e retorna a mensagem com o CRC anexado
    std::vector<char> calculateCRC(const std::vector<char> &quadro);

    // Remove o CRC16 da mensagem e verifica sua integridade
    std::vector<char> removeCRC(const std::vector<char> &quadro);
};

#endif // FRAMING_H