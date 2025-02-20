#include "framing.h"

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

// Função para enviar um frame (quadro) pela camada de Framing.
void Framing::envia(Frame * frame) {
    // Serializa o frame em um vetor de bytes.
    vector<char> quadro = frame->serialize();

    std::cout << "Sem CRC: "; 
    dump(quadro, std::cout);  // Exibe o quadro sem o CRC

    // Adiciona o CRC ao quadro
    quadro = addCRC(quadro);

    std::cout << "Com CRC: ";
    dump(quadro, std::cout);  // Exibe o quadro com o CRC incluído

    // Prepara o quadro para framing (adicionando delimitadores e realizando escapes)
    vector<char> framedPacket;
    framedPacket.push_back(FRAME_DELEMITER); // Adiciona o delimitador de início

    // Percorre cada byte do quadro para tratar caracteres especiais (delimitador e caractere de escape)
    for (char c : quadro) {
        if (c == FRAME_DELEMITER) {
            // Se o byte for o delimitador, insere o caractere de escape seguido do código de escape (0x5E)
            framedPacket.push_back(ESCAPE_CHARACTER);
            framedPacket.push_back(0x5E); // Representa o byte original 0x7E
        } else if (c == ESCAPE_CHARACTER) {
            // Se o byte for o caractere de escape, insere-o duas vezes com o código de escape (0x5D)
            framedPacket.push_back(ESCAPE_CHARACTER);
            framedPacket.push_back(0x5D); // Representa o byte original 0x7D
        } else {
            // Caso contrário, insere o byte normalmente
            framedPacket.push_back(c);
        }
    }

    framedPacket.push_back(FRAME_DELEMITER); // Adiciona o delimitador de fim

    std::cout << "Com Frame: ";
    dump(framedPacket, std::cout);  // Exibe o quadro final (framed packet)
    std::cout << std::endl;
    
    // Escreve o framed packet na interface serial
    serial.write(framedPacket);
}

// Função handle() chamada quando há dados disponíveis para leitura na porta serial.
// Ela lê um byte e o envia para a FSM (função fsm) encapsulado em um evento.
void Framing::handle() {
    // Lê um byte da porta serial
    char receivedData = serial.read_byte();
    
    // Chama a função fsm() passando um evento do tipo BYTE_RECEIVED com o byte lido
    fsm(FramingEvent(FramingEvent::BYTE_RECEIVED, receivedData));
}

// Função da FSM (Máquina de Estados) para processar os bytes recebidos e montar um quadro.
void Framing::fsm(FramingEvent e) {
    switch (estado) {
        case WAITING:
            // Se estiver no estado WAITING e o byte for o delimitador de quadro...
            if (e.data == FRAME_DELEMITER) {
                // Limpa o buffer, marca que o frame começou e muda o estado para RECEIVING
                buffer.clear();
                frame_started = true;
                estado = RECEIVING;
            }
            break;

        case RECEIVING:
            // Se o byte recebido é o caractere de escape e ainda não está em modo de escape...
            if (e.data == ESCAPE_CHARACTER && !escaping) {
                escaping = true; // O próximo byte será tratado como escapado
            } else if (e.data == FRAME_DELEMITER && !escaping) {
                // Se o byte for o delimitador final (não escapado), considera que o quadro está completo
                interpreter(buffer);  // Processa o quadro completo
                buffer.clear();         // Limpa o buffer para o próximo quadro
                frame_started = false;
                estado = WAITING;       // Retorna ao estado WAITING
            } else {
                if (escaping) {
                    // Se estamos no modo de escape, processa o byte conforme a sequência de escape
                    if (e.data == 0x5E) {
                        buffer.push_back(FRAME_DELEMITER);  // Restaura o delimitador original
                    } else if (e.data == 0x5D) {
                        buffer.push_back(ESCAPE_CHARACTER); // Restaura o caractere de escape original
                    } else {
                        // Se o byte não é válido para escape, limpa o buffer, reseta o estado e lança erro
                        buffer.clear();
                        estado = WAITING;
                        throw std::runtime_error("Caractere escapado inválido");
                    }
                    escaping = false; // Sai do modo de escape
                } else {
                    // Caso contrário, insere o byte normalmente no buffer
                    buffer.push_back(e.data);
                }
            }
            break;
    }
}

// Em caso de timeout, a função handle_timeout() limpa o buffer e reseta o estado da FSM.
void Framing::handle_timeout() {
    buffer.clear();
    estado = WAITING; // Reseta o estado para WAITING
    std::cout << "Timeout: Buffer limpo\n";
}

// Função recebe() para a camada Framing.
// Neste exemplo, não há implementação, pois o processamento é feito via FSM (handle e fsm).
void Framing::recebe(Frame * frame) {
    // Não implementado – o recebimento é gerenciado via FSM no método handle()
}

// Função interpreter() que processa o quadro completo recebido.
// Realiza o "unescape" dos bytes, remove os delimitadores e verifica o CRC.
void Framing::interpreter(std::vector<char> & quadro) {


    // Neste exemplo, utilizamos o quadro completo, pois a FSM já tratou dos delimitadores.
    std::vector<char> receivedPacket = quadro;

    std::cout << "Recebido C-CRC: ";
    dump(receivedPacket, std::cout);
    
    // Tenta remover o CRC; se o CRC for inválido, descarta o quadro e imprime mensagem.
    if(!removeCRC(receivedPacket)){
        std::cout << "CRC inválido, descartando quadro\n";
        return;
    }

    std::cout << "Recebido S-CRC: ";
    dump(receivedPacket, std::cout);
    std::cout << std::endl;

    // Cria um objeto Frame com os dados recebidos (sem os bytes do CRC)
    Frame frame(receivedPacket);

    // Encaminha o frame para a camada superior (Application), se existir
    if (superior) {
        superior->recebe(&frame);
    }
}
