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
#include "frame.h"  // Inclui a definição da classe Frame

// Define os caracteres usados para o framing
#define FRAME_DELEMITER 0x7E
#define ESCAPE_CHARACTER 0x7D

// Estados da FSM para a camada de framing
enum FramingState {
    WAITING,    // Estado aguardando o início do quadro (delimitador)
    RECEIVING   // Estado recebendo os dados do quadro (tratamento de escapes incluso)
};

// Classe para representar um evento na FSM de framing
class FramingEvent {
public:
    // Tipos de eventos: BYTE_RECEIVED ou TIMEOUT
    enum Type {
        BYTE_RECEIVED,
        TIMEOUT
    };

    Type type;   // Tipo de evento
    char data;   // Byte recebido (no caso de BYTE_RECEIVED)

    // Construtor do evento: inicializa o tipo e o dado recebido
    FramingEvent(Type t, char d) : type(t), data(d) {}
};

// Classe Framing que implementa a camada de framing utilizando FSM.
// Deriva de Subcamada e gerencia o processo de framing, incluindo escapes e CRC.
class Framing : public Subcamada {
private: 
    Serial & serial;             // Referência para a porta serial
    std::vector<char> buffer;    // Buffer para acumular bytes do quadro
    FramingState estado;         // Estado atual da FSM (WAITING ou RECEIVING)
    bool escaping = false;       // Flag para indicar se o próximo byte deve ser tratado como escapado
    bool frame_started = false;  // Indica se o início do quadro já foi detectado

    // Função da FSM que processa os eventos de framing
    void fsm(FramingEvent e);

public:
    // Construtor: inicializa com o estado WAITING e desativa timeout (pode ser ativado se necessário)
    Framing(Serial & porta, long tout) 
        : Subcamada(porta.get(), tout), serial(porta), estado(WAITING) {
        disable_timeout();  
    }

    // Método para enviar um frame (implementação de Subcamada)
    void envia(Frame * frame) override;
    // Método para receber um frame (não utilizado neste exemplo, pois a recepção é gerenciada via FSM)
    void recebe(Frame * frame) override;
    // Método chamado em caso de timeout; limpa o buffer e reseta o estado
    void handle_timeout() override;
    // Método que interpreta o quadro completo recebido (desfaz escapes, remove CRC e encaminha para a camada superior)
    void interpreter(std::vector<char> & quadro);
    // Método handle() chamado quando há dados disponíveis; lê um byte e passa para a FSM
    void handle() override;
};

#endif // FRAMING_H
