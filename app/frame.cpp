#include "frame.h"


std::vector<char> Frame::serialize(){

    std::vector<char> frame;

    // first byte is for control. 
    std::vector<char> firstByte;

    // bit 7 is control bit
    // bit 3 is sequence bit
    frame.push_back((this->controlBit << 7) | (this->sequenceBit << 3));

    // second byte is for reserved
    frame.push_back(this->reserved);
    
    // third byte is for idProto
    frame.push_back(this->idProto);

    std::copy(this->data.begin(), this->data.end(), std::back_inserter(frame));

    return frame;
}


uint8_t Frame::getSequenceBit() const{
    return this->sequenceBit;
}

uint8_t Frame::getControlBit() const{
    return this->controlBit;
}

uint8_t Frame::getReserved()const{
    return this->reserved;
}

uint8_t Frame::getIdProto() const{
    return this->idProto;
}

std::string Frame::getData() const{
    return std::string(this->data.begin(), this->data.end());
}



void Frame::addIdProto(::idProto idProto){
    this->idProto = idProto;
}

void Frame::addReserved(::reserved reserved){
    this->reserved = reserved;
}
void Frame::addControlBit(::controlBit controlBit){
    this->controlBit = controlBit;

}
void Frame::addSequenceNumber(int sequenceNumber){
    this->sequenceBit = sequenceNumber;
}

