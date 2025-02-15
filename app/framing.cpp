#include "framing.h"

void dump(const std::vector<char> buffer, std::ostream & out) {
    int n = 0;

    out << std::hex << std::setprecision(2);
    for (auto & c: buffer) {
        int x = (unsigned char)c;
        out << x << " ";
        n++;
        if ((n % 20) == 0) out << std::endl;
    }

    out << std::endl;
}

std::vector<char> addCRC(std::vector<char> & quadro) {
    // Implementação da adição do CRC16 (exemplo simplificado)

    // Calcula o CRC16
    auto crc = make_crc16(quadro);

    crc.generate_into(quadro);

    return quadro;
}

bool removeCRC(std::vector<char> & quadro) {
    // Implementação da remoção e verificação do CRC16 (exemplo simplificado)

    auto crc = make_crc16(quadro);

    if(crc.check()){
        quadro.pop_back();
        quadro.pop_back();
        return true;
    } 
    return false;
}


void Framing::envia(Frame * frame) {

    // Serializa o quadro
    vector<char> quadro = frame->serialize();

    std::cout << "Sem CRC: "; 
    dump(quadro, std::cout);

    // Adiciona o CRC ao quadro
    quadro = addCRC(quadro);


    std::cout << "Com CRC: ";
    dump(quadro, std::cout);


    vector<char> framedPacket;
    framedPacket.push_back(FRAME_DELEMITER); // Início do quadro

    // Serializa a mensagem, escapando caracteres especiais
    for (char c : quadro) {
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

    std::cout << "Com Frame: ";
    dump(framedPacket, std::cout);
    std::cout << std::endl;
    
    serial.write(framedPacket);
}

void Framing::recebe(Frame * frame) {
}

void Framing::interpreter(vector<char> & quadro) {

    std::cout << "Frame Recebido: ";
    dump(quadro, std::cout);

    vector<char> receivedPacket;

    // Desserializa o quadro, desfazendo o escape dos caracteres
    for (size_t i = 1; i < quadro.size(); i++) {
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
    // remove o delimitador do quadro
    receivedPacket.pop_back();


    std::cout << "Recebido C-CRC: ";
    dump(receivedPacket, std::cout);
    
    // tenta remover o CRC, se não conseguir, descarta o quadro
    if(!removeCRC(receivedPacket)){
        std::cout << "CRC inválido, descartando quadro\n";
        return;
    }

    std::cout << "Recebido S-CRC: ";
    dump(receivedPacket, std::cout);
    std::cout << std::endl;

    // Cria um novo quadro com os dados recebidos
    Frame frame(receivedPacket);

    // Envia a mensagem para a camada superior (Application)
    if (superior) {
        superior->recebe(&frame);
    }
}

