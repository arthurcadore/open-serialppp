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

void Framing::handle() {
    char receivedData = serial.read_byte();
    
    fsm(FramingEvent(FramingEvent::BYTE_RECEIVED, receivedData));
}

void Framing::fsm(FramingEvent e) {
    switch (estado) {
        case WAITING:
            if (e.data == FRAME_DELEMITER) {
                buffer.clear();
                frame_started = true;
                estado = RECEIVING;
            }
            break;

        case RECEIVING:
            if (e.data == ESCAPE_CHARACTER && !escaping) {
                escaping = true; // Próximo byte é escapado
            } else if (e.data == FRAME_DELEMITER && !escaping) {
                // Delimitador final não escapado: fim do quadro
                interpreter(buffer);
                buffer.clear();
                frame_started = false;
                estado = WAITING;
            } else {
                if (escaping) {
                    // Processa byte escapado
                    if (e.data == 0x5E) {
                        buffer.push_back(FRAME_DELEMITER);
                    } else if (e.data == 0x5D) {
                        buffer.push_back(ESCAPE_CHARACTER);
                    } else {
                        buffer.clear();
                        estado = WAITING;
                        throw std::runtime_error("Caractere escapado inválido");
                    }
                    escaping = false;
                } else {
                    buffer.push_back(e.data); // Byte normal
                }
            }
            break;
    }
}
void Framing::handle_timeout() {
    buffer.clear();
    estado = WAITING; // Reset em caso de timeout
    std::cout << "Timeout: Buffer limpo\n";
}

void Framing::recebe(Frame * frame) {

}

void Framing::interpreter(std::vector<char> & quadro) {
    std::cout << "Frame Recebido: ";
    dump(quadro, std::cout);

    // Usa o quadro inteiro, pois a FSM já removeu os delimitadores
    std::vector<char> receivedPacket = quadro;

    std::cout << "Recebido C-CRC: ";
    dump(receivedPacket, std::cout);
    
    // Tenta remover o CRC; se não conseguir, descarta o quadro
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


