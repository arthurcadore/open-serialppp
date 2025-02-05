#include <iostream>
#include <vector>
#include <iomanip>  
#include "crc16.h"

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

int main() {
    // um buffer com dados
    std::vector<uint8_t> buffer = {0x88, 0x89, 0xaa, 0x0, 0xf1};

    // cria um gerador/verificador de CRC16
    auto crc = make_crc16(buffer);

    // mostra o conteúdo do buffer antes de gerar o CRC
    dump(buffer, std::cout);
    std::cout << std::endl;

    // gera o valor de CRC16 para o buffer, anexando-o ao final desse buffer
    crc.generate_into(buffer);

    // mostra o conteúdo do buffer após gerar o CRC
    dump(buffer, std::cout);
    std::cout << std::endl;

    // cria novo gerador/verificador de CRC
    auto checker = make_crc16(buffer);

    // verifica se o buffer está correto, com base em seu valor de CRC
    std::cout << std::boolalpha;
    std::cout << "Verificação do buffer: " << checker.check() << std::endl;

}