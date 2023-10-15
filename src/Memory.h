//
// Created by Eason Pan on 2023-10-13.
//

#ifndef CS5600_FSM_SIMULATOR_MEMORY_H
#define CS5600_FSM_SIMULATOR_MEMORY_H
#include "Chunk.h"
#include <iostream>


class Memory {
private:
    int capacity;
    int spaceLeft;
    Chunk* memoryList;
    Chunk* currentPos;
    int chunkCount;

public:
    Memory(int capacity){
        if(capacity < 1 || capacity > 10000){
            throw std::invalid_argument("Capacity should between 1 - 10000\n");
        }
        this->capacity = capacity;
        this->spaceLeft = capacity;
        this->memoryList = new Chunk(0, capacity, true);
        this->currentPos = this->memoryList;
        this->chunkCount = 1;
    }


    /**
     * Destructor, clear out all Chunk items in this object.
     */
    ~Memory() {
        Chunk *current = this->memoryList;
        while (current != nullptr) {
            Chunk *temp = current;
            current = current->next;
            delete (temp);
        }
    }


    /**
     * Get how much space left.
     * @return int. space left in the memory.
     */
    int getSpaceLeft(){
        return this->spaceLeft;
    }


    /**
     * Get how many chunks in the memory.
     * @return int. chunk count
     */
    int getChunkCount(){
        return this->chunkCount;
    }


    /**
     * Allocate the given amount of space to the memory.
     * @param spaceSize the size we want to allocate.
     * @return -1 when failed. Otherwise, return total search time.
     */
    int allocateSpace(int spaceSize){
        if(0 == spaceSize){
            return 0;
        }
        if(this->canAllocate(spaceSize)){ //check whether we can allocate such amount of space
            this->spaceLeft -= spaceSize;
            int searchCount = this->useMemory(spaceSize, 1);
            return searchCount;
        }
        return -1;
    }


    /**
     * Free the given amount of space in the memory.
     * @param spaceSize the size we want to free.
     * @return -1 when failed. Otherwise, return total search time.
     */
    int freeSpace(int spaceSize){
        if(0 == spaceSize){
            return 0;
        }
        if(this->canFree(spaceSize)){ //check whether we can free such amount of space
            int processingSize;
            processingSize = std::min((this->capacity - this->spaceLeft), spaceSize);
            this->spaceLeft += processingSize;
            int searchCount = this->useMemory(processingSize, 0);
            return searchCount;
        }
        return -1;
    }


    /**
     * Coalesce free space in the memory.
     * Generally, then get the currentPos to the first free space.
     */
    void coalesce(){
        if(1 == this->chunkCount){
            return;
        }
        Chunk* current = this->memoryList;

        while(current->next != nullptr){
            if(!current->isFree){
                current = current->next;
            }else if(current->next->isFree){
                this->currentPos = current;
                Chunk* temp = current->next;
                current->next = temp->next;
                current->size += temp->size;
                delete(temp);
                this->chunkCount -= 1;
            }else{
                current = current->next;
            }
        }
    }


    /**
     * Print out the current memory status.
     */
    void printMemory(){
        std::cout << "Current Pointer @ 000" << this->currentPos->address << "\n";
        std::cout << "Memory Status: \n";
        Chunk* current = this->memoryList;
        while(current != nullptr){
            std::string currentStatus;
            if(current->isFree){
                currentStatus = "Free";
            }else{
                currentStatus = "In-Use";
            }
            std::cout << "[ Address: (000" << current->address << "), Size: " << current->size << "kb" << ", Status: " << currentStatus << " ]\n";
            current = current->next;
        }
    }



private:
    /**
     * use memory - allocate or free space.
     * @param spaceSize allocate or free the given amount of space.
     * @param mode 1 - allocate, 0 - free.
     * @return total search used.
     */
    int useMemory(int spaceSize, int mode){
        if(this->currentPos == nullptr){
            this->currentPos = this->memoryList; //get to the head, if it's at the end
        }
        bool assignValue;
        if(1 == mode){
            assignValue = false;
        }else{
            assignValue = true;
        }
        int searchCount = 0;
        int leftSize = spaceSize;
        while(leftSize > 0){
            if(this->currentPos == nullptr){
                this->currentPos = this->memoryList;
            }

            if(this->currentPos->isFree == !assignValue){
                if(this->currentPos->size == spaceSize){ //current position is the same as required
                    this->currentPos->isFree = assignValue;
                    leftSize = 0;
                }else if(this->currentPos->size > spaceSize){ // current position is bigger than required
                    this->currentPos->isFree = assignValue;
                    this->currentPos = splitChunk(this->currentPos, spaceSize, mode);
                    leftSize = 0;
                }else{ //current position is not big enough
                    this->currentPos->isFree = assignValue;
                    leftSize -= this->currentPos->size;
                    searchCount += this->findNext(searchCount, mode);
                }
            }else{
                searchCount += this->findNext(searchCount, mode);
            }
        }
        if(this->currentPos == nullptr){
            this->currentPos = this->memoryList; //get to the head, if it's at the end
        }
        return searchCount;
    }


    bool canFree(int spaceSize){
        if(this->spaceLeft == this->capacity){
            return false;
        }
        return true;
    }


    bool canAllocate(int spaceSize){
        if(this->spaceLeft == 0 || spaceSize > this->spaceLeft){
            return false;
        }
        return true;
    }


    /**
     * Find next chunk based on mode, free chunk when allocating, used chunk when freeing.
     * @param currentSearchCount current total search count.
     * @param mode 1 - allocate, 0 - free.
     * @return
     */
    int findNext(int currentSearchCount, int mode){
        bool assignValue;
        if(1 == mode){
            assignValue = false;
        }else{
            assignValue = true;
        }
        int searchCount = currentSearchCount;
        while(currentPos != nullptr && currentPos->isFree == assignValue){
            currentPos = currentPos->next;
            searchCount += 1;
        }
        return searchCount;
    }


    /**
     * split the current chunk
     * @param currentChunk ptr to the current chunk
     * @param spaceSize target used size of space
     * @param mode 1 - allocate, 0 - free
     * @return
     */
    Chunk* splitChunk(Chunk* currentChunk, int spaceSize, int mode){
        bool assignValue;
        if(1 == mode){
            assignValue = true;
        }else{
            assignValue = false;
        }
        int newChunkSize;

        newChunkSize = currentChunk->size - spaceSize;

        int newChunkAddress = currentChunk->address + spaceSize;
        Chunk* newChunk = new Chunk(newChunkAddress, newChunkSize, assignValue);
        currentChunk->size = spaceSize;
        newChunk->next = currentChunk->next;
        currentChunk->next = newChunk;
        this->chunkCount += 1;
        return newChunk;

    }
};


#endif //CS5600_FSM_SIMULATOR_MEMORY_H
