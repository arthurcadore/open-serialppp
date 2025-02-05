#include "framing.h"

void dump(const std::vector<uint8_t> & buffer, std::ostream & out) {
    int n = 0;

    out << std::hex << std::setprecision(2);
    for (auto & c: buffer) {
        int x = (unsigned char)c;
        out << x << " ";
        n++;
        if ((n % 20) == 0) out << std::endl;
    }
}

void Framing::envia(const vector<char>& quadro) {
    // Calcula o CRC16 e anexa à mensagem
    vector<char> quadroComCRC = calculateCRC(quadro);

    vector<char> framedPacket;
    framedPacket.push_back(FRAME_DELEMITER); // Início do quadro

    // Serializa a mensagem, escapando caracteres especiais
    for (char c : quadroComCRC) {
        if (c == FRAME_DELEMITER) {
            framedPacket.push_back(ESCAPE_CHARACTER);
            framedPacket.push_back(0x5E); // Escapa 0x7E como 0x7D 0x5E
        } else if (c == ESCAPE_CHARACTER) {
            framedPacket.push_back(ESCAPE_CHARACTER);
            framedPacket.push_back(0x5D); // Escapa 0x7D como 0x7D 0x5D
        } else {
            framedPacket.push_back(c);
        }
    }

    framedPacket.push_back(FRAME_DELEMITER); // Fim do quadro

    // imprime os dados do quadro para debug
    std::cout << "Framing sending: ";
    for (char c : framedPacket) {
        std::cout << c;
    }

    serialTx.write(quadro);
}

void Framing::recebe(const vector<char>& quadro) {

    // imprime os dados do quadro para debug
    std::cout << "\n" <<"Framing received: ";
    for (char c : quadro) {
        std::cout << c;
    }

    vector<char> receivedPacket;


    // Desserializa o quadro, desfazendo o escape dos caracteres
    for (size_t i = 0; i < quadro.size(); i++) {
        if (quadro[i] == ESCAPE_CHARACTER) {
            i++; // Pula o caractere de escape
            if (i >= quadro.size()) {
                throw std::runtime_error("Invalid escape sequence");
            }

            if (quadro[i] == 0x5E) {
                receivedPacket.push_back(FRAME_DELEMITER);
            } else if (quadro[i] == 0x5D) {
                receivedPacket.push_back(ESCAPE_CHARACTER);
            } else {
                throw std::runtime_error("Invalid escape sequence");
            }
        } else {
            receivedPacket.push_back(quadro[i]);
        }
    }

    // Remove o CRC16 e verifica a integridade da mensagem
    vector<char> receivedMessage = removeCRC(receivedPacket);

    // Envia a mensagem para a camada superior (Application)
    if (superior) {
        superior->recebe(receivedMessage);
    }
}

vector<char> Framing::calculateCRC(const vector<char>& quadro) {
    // Implementação do cálculo do CRC16 (exemplo simplificado)
    vector<char> quadroComCRC = quadro;
    quadroComCRC.push_back(0x00); // CRC byte 1 (simulado)
    quadroComCRC.push_back(0x00); // CRC byte 2 (simulado)
    return quadroComCRC;
}

vector<char> Framing::removeCRC(const vector<char>& quadro) {
    // Implementação da remoção e verificação do CRC16 (exemplo simplificado)
    if (quadro.size() < 2) {
        throw std::runtime_error("Invalid frame: too short");
    }

    vector<char> quadroSemCRC(quadro.begin(), quadro.end() - 2);
    return quadroSemCRC;
}