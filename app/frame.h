#ifndef __FRAME_H__
#define	__FRAME_H__

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

#define DATA_LENGTH 1024
#define FRAME_LENGTH 1029

enum controlBit{
    DATA = 0,
    ACK = 1
};

enum reserved{
    R = 0,
    N = 1
};

enum idProto{
    MSG = 1
};

class Frame {
    private: 
    uint8_t controlBit; 
    uint8_t sequenceBit;
    uint8_t reserved; 
    uint8_t idProto; 
    std::vector<char> data; 

    public:
    // constructors

    Frame(){}
    
    Frame(const std::string & data){
        // check if data is not empty and is equal or lower than 1024
        if(data.size() >= DATA_LENGTH){
            throw std::invalid_argument("Data size is invalid");
        }
        this->data = std::vector<char>(data.begin(), data.end());
    }
    Frame(const std::vector<char> & data){
        // check if data is not empty and is equal or lower than 1024
        if(data.size() >= FRAME_LENGTH || data.size() < 2){
            throw std::invalid_argument("Data size is invalid");
        }
        
        // first byte is for control.
        this->controlBit = (data[0] & 0x80) >> 7;
        this->sequenceBit = (data[0] & 0x08) >> 3;

        // second byte is for reserved
        this->reserved = data[1];

        // third byte is for idProto
        this->idProto = data[2];

        // copy the rest of the data
        std::copy(data.begin() + 3, data.end(), std::back_inserter(this->data));
    }

    void addIdProto(::idProto idProto);
    void addReserved(::reserved reserved);
    void addControlBit(::controlBit controlBit);
    void addSequenceNumber(int sequenceNumber);


    std::vector<char> serialize();

    uint8_t getSequenceBit() const;
    uint8_t getControlBit() const;
    uint8_t getReserved() const;
    uint8_t getIdProto() const;
    std::string getData() const;

};

#endif    /* __FRAME_H__ */