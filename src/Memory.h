//
// Created by Eason Pan on 2023-10-13.
//

#ifndef CS5600_FSM_SIMULATOR_MEMORY_H
#define CS5600_FSM_SIMULATOR_MEMORY_H
#include "Chunk.h"


class Memory {
private:
    int capacity;
    int spaceLeft;
    Chunk* memoryList;
    Chunk* currentPos;
    int chunkCount;

public:
    Memory(int capacity){
        if(capacity < 1 || capacity > 50){
            throw std::invalid_argument("Capacity should between 1 - 50\n");
        }
        this->capacity = capacity;
        this->spaceLeft = capacity;
        this->memoryList = new Chunk(0, capacity, true);
        this->currentPos = this->memoryList;
        this->chunkCount = 1;
    }
};


#endif //CS5600_FSM_SIMULATOR_MEMORY_H
