#ifndef FRAMING_H
#define FRAMING_H

#include <vector>
#include <stdexcept>
#include "Subcamada.h"
#include <iomanip>  
#include <cstdint>
#include <iostream>
#include "../libs/crc16.h"
#include "../libs/serial.h"

// Define os caracteres usados para o framing
#define FRAME_DELEMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Classe para a camada Framing
class Framing : public Subcamada
{
private: 
    Serial & serial;

public:
    // Construtor
    Framing(Serial & porta, long tout) : Subcamada(porta.get(), tout), serial(porta){}

    // Método para enviar dados (implementação da Subcamada)
    void envia(const std::vector<char> &quadro) override;

    // Método para receber dados (implementação da Subcamada)
    void recebe(const std::vector<char> &quadro) override;

    // Método chamado em caso de timeout
    void handle_timeout() {}

    // Método chamado em caso de dados recebidos
    void handle()
    {
        // Lê dados da porta serial
        char receivedData = serial.read_byte();

        // Exibe os dados recebidos da porta serial
        std::cout << "IO received: ";

        std::cout << receivedData << std::endl;


        // desenquadra o quadro usando metodo recebe, depois envia para a camada de cima. 
        // recebe(receivedData);

        // // Envia os dados recebidos para a camada superior (Applicaçao)
        // if (superior)
        // {
        //     superior->recebe(receivedData);
        // }
    }

private:
    // Calcula o CRC16 e retorna a mensagem com o CRC anexado
    std::vector<char> calculateCRC(const std::vector<char> &quadro);

    // Remove o CRC16 da mensagem e verifica sua integridade
    std::vector<char> removeCRC(const std::vector<char> &quadro);
};

#endif // FRAMING_H