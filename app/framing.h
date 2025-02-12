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
    vector<char> buffer;
    int delimiter = 0;

public:
    // Construtor
    Framing(Serial & porta, long tout) : Subcamada(porta.get(), tout), serial(porta){}

    // Método para enviar dados (implementação da Subcamada)
    void envia(Frame * Frame) override;

    // Método para receber dados (implementação da Subcamada)
    void recebe(Frame * Frame) override;


    // Método chamado em caso de timeout
    void handle_timeout() {}

    void interpreter(vector<char> & quadro);

    // Método chamado em caso de dados recebidos
    void handle()
    {
        // Lê dados da porta serial
        char receivedData = serial.read_byte();

        // adiciona ao buffer
        buffer.push_back(receivedData);

        // verifica se é um escape, se for, incrementa o contador
        if (receivedData == FRAME_DELEMITER)
        {
            delimiter++;
        }

        // verifica se há um quadro completo
        if (delimiter % 2 == 0)
        {
            // Desserializa o quadro, desfazendo o escape dos caracteres
            interpreter(buffer);
            
            // limpa o buffer
            buffer.clear();

            // reseta o contador
            delimiter = 0;
        }
    }
};

#endif // FRAMING_H