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

#define FRAME_DELIMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Define estados da FSM
enum class FrameState {
    WAITING_START,    // Esperando início do quadro
    RECEIVING_DATA,   // Recebendo dados normais
    ESCAPE_RECEIVED,  // Recebeu caractere de escape
    FRAME_COMPLETE    // Quadro completo recebido
};

class Framing : public Subcamada {
private: 
    Serial & serial;
    vector<char> buffer;
    FrameState current_state;
    
    // Método privado para processar caractere recebido baseado no estado atual
    void process_char(char c) {
        switch (current_state) {
            case FrameState::WAITING_START:
                if (c == FRAME_DELIMITER) {
                    buffer.clear();
                    buffer.push_back(c);
                    current_state = FrameState::RECEIVING_DATA;
                }
                break;

            case FrameState::RECEIVING_DATA:
                buffer.push_back(c);
                if (c == FRAME_DELIMITER && buffer.size() > 1) {
                    current_state = FrameState::FRAME_COMPLETE;
                } else if (c == ESCAPE_CHARACTER) {
                    current_state = FrameState::ESCAPE_RECEIVED;
                }
                break;

            case FrameState::ESCAPE_RECEIVED:
                if (c == 0x5E || c == 0x5D) {
                    buffer.push_back(c);
                    current_state = FrameState::RECEIVING_DATA;
                } else {
                    // Sequência de escape inválida
                    buffer.clear();
                    current_state = FrameState::WAITING_START;
                }
                break;

            case FrameState::FRAME_COMPLETE:
                // Este estado é processado em handle()
                break;
        }
    }
    // Função utilitária para "dump" de um vetor de bytes no formato hexadecimal.
// Útil para depuração e visualização do conteúdo dos buffers.
void dump(const std::vector<char> buffer, std::ostream & out) {
    int n = 0;
    // Configura a saída para hexadecimal com precisão de 2 dígitos.
    out << std::hex << std::setprecision(2);
    // Itera por cada byte do buffer.
    for (auto & c: buffer) {
        // Converte o byte para inteiro sem sinal.
        int x = (unsigned char)c;
        out << x << " ";
        n++;
        // Insere uma quebra de linha a cada 20 bytes para melhor legibilidade.
        if ((n % 20) == 0) out << std::endl;
    }
    out << std::endl;
}

// Função que adiciona o CRC16 ao final do quadro.
// Recebe um vetor de bytes (quadro), calcula o CRC e insere-o no final do vetor.
std::vector<char> addCRC(std::vector<char> & quadro) {
    // Calcula o CRC16 a partir do quadro
    auto crc = make_crc16(quadro);
    // Gera os bytes do CRC e os adiciona ao vetor 'quadro'
    crc.generate_into(quadro);
    return quadro;
}

// Função que remove o CRC16 do final do quadro e verifica se o CRC está correto.
// Se o CRC estiver válido, remove os dois últimos bytes e retorna true; caso contrário, retorna false.
bool removeCRC(std::vector<char> & quadro) {
    auto crc = make_crc16(quadro);
    if(crc.check()){
        // Remove os dois últimos bytes que correspondem ao CRC
        quadro.pop_back();
        quadro.pop_back();
        return true;
    } 
    return false;
}


public:
    Framing(Serial & porta, long tout) 
        : Subcamada(porta.get(), tout)
        , serial(porta)
        , current_state(FrameState::WAITING_START) {
        disable_timeout();
    }

    void envia(Frame * frame) override {
        vector<char> quadro = frame->serialize();
        
        std::cout << "Sem CRC: "; 
        dump(quadro, std::cout);

        quadro = addCRC(quadro);
        
        std::cout << "Com CRC: ";
        dump(quadro, std::cout);

        vector<char> framedPacket;
        framedPacket.push_back(FRAME_DELIMITER);

        for (char c : quadro) {
            if (c == FRAME_DELIMITER) {
                framedPacket.push_back(ESCAPE_CHARACTER);
                framedPacket.push_back(0x5E);
            } else if (c == ESCAPE_CHARACTER) {
                framedPacket.push_back(ESCAPE_CHARACTER);
                framedPacket.push_back(0x5D);
            } else {
                framedPacket.push_back(c);
            }
        }

        framedPacket.push_back(FRAME_DELIMITER);

        std::cout << "Com Frame: ";
        dump(framedPacket, std::cout);
        std::cout << std::endl;
        
        serial.write(framedPacket);
    }

    void recebe(Frame * frame) override {}

    void interpreter(vector<char> & quadro) {
        std::cout << "Frame Recebido: ";
        dump(quadro, std::cout);

        vector<char> receivedPacket;
        bool escape_next = false;

        // Processa do segundo ao penúltimo caractere (remove delimitadores)
        for (size_t i = 1; i < quadro.size() - 1; i++) {
            if (escape_next) {
                if (quadro[i] == 0x5E) {
                    receivedPacket.push_back(FRAME_DELIMITER);
                } else if (quadro[i] == 0x5D) {
                    receivedPacket.push_back(ESCAPE_CHARACTER);
                } else {
                    throw std::runtime_error("Invalid escape sequence");
                }
                escape_next = false;
            } else if (quadro[i] == ESCAPE_CHARACTER) {
                escape_next = true;
            } else {
                receivedPacket.push_back(quadro[i]);
            }
        }

        std::cout << "Recebido C-CRC: ";
        dump(receivedPacket, std::cout);
        
        if (!removeCRC(receivedPacket)) {
            std::cout << "CRC inválido, descartando quadro\n";
            return;
        }

        std::cout << "Recebido S-CRC: ";
        dump(receivedPacket, std::cout);
        std::cout << std::endl;

        Frame new_frame(receivedPacket);
        if (superior) {
            superior->recebe(&new_frame);
        }
    }

    void handle_timeout() {}

    void handle() {
        char received_data = serial.read_byte();
        process_char(received_data);

        if (current_state == FrameState::FRAME_COMPLETE) {
            interpreter(buffer);
            buffer.clear();
            current_state = FrameState::WAITING_START;
        }
    }
};

#endif // FRAMING_H