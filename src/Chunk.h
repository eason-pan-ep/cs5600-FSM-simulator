//
// Created by Eason Pan on 2023-10-13.
//

#ifndef CS5600_FSM_SIMULATOR_CHUNK_H
#define CS5600_FSM_SIMULATOR_CHUNK_H


class Chunk {
public:
    int address;
    int size;
    bool isFree;
    Chunk* next;

public:
    Chunk(int address, int size, bool isFree){
        this->address = address;
        this->size = size;
        this->isFree = isFree;
        this->next = nullptr;
    }
};


#endif //CS5600_FSM_SIMULATOR_CHUNK_H
