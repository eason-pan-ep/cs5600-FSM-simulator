//
// Created by Eason Pan on 2023-10-15.
//

#ifndef CS5600_FSM_SIMULATOR_FSCONTROLLERWITHVIEW_H
#define CS5600_FSM_SIMULATOR_FSCONTROLLERWITHVIEW_H
#include "Memory.h"
#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <math.h>



class FSControllerWithView {

private:
    Memory* memory;
    bool isRandom;
    int processCount;
    bool doCoalesce;
    std::vector<int> processes;

public:
    /**
     * Constructor
     * @param headerSize
     * @param memoryCapacity
     * @param processCount
     * @param isRandom
     * @param doCoalesce
     * @param userInput process list "10,-10,-20"
     */
    FSControllerWithView(int headerSize, int memoryCapacity, int processCount=10, bool isRandom=false, bool doCoalesce=false, std::string userInput=""){
        if(headerSize > memoryCapacity){
            throw std::invalid_argument("Header size should be smaller than memoryCapacity.\n");
        }
        if(headerSize < 0 || headerSize > 100){
            throw std::invalid_argument("Header size range: 0 - 100\n");
        }
        if(userInput.empty() && !isRandom){
            throw std::invalid_argument("Please giving processes or change it to random processes.\n");
        }
        if(memoryCapacity < 1 || memoryCapacity > 10000){
            throw std::invalid_argument("Memory capacity range: 1 - 10,000\n");
        }
        this->memory = new Memory(memoryCapacity);
        this->memory->allocateSpace(headerSize);
        this->isRandom = isRandom;
        this->processCount = processCount;
        this->doCoalesce = doCoalesce;
        this->processes = this->getProcessList(userInput);
    }

    /**
     * Destructor
     */
    ~ FSControllerWithView(){
        delete(this->memory);
        this->processes.clear();
    }

    /**
     * Print out running processes (malloc - positive num & free - negative num)
     */
    void printProcesses(){
        std::cout << "Operations: ";
        for(int i : this->processes){
            std::cout << i << ", ";
        }
        std::cout << "\n";
    }

    /**
     * Running simulation. doing by size.
     * @param mode  1 - all doing by size, can allocate and free any size at any chunk,
     *              0 - only allocate when a chunk is equal to or greater than requested,
     *                  only free the chunk exactly the size requested (assume good inputs)
     */
    void runSimulation(int mode){
        std::cout << "============================    Simulation Starts    ============================\n";
        this->printProcesses();
        int totalSearch = 0;
        std::vector<int> searchLog;

        if(!this->doCoalesce){
            std::cout << "Coalesce Disabled.\n";
        }else{
            std::cout << "Coalesce Enabled.\n";
        }
        std::cout << "-----------------------------------------------\n";

        for(int i : this->processes){
            int currentSearch = 0;
            if(i > 0){
                if(1 == mode){
                    currentSearch = this->memory->allocateSpace(i);
                }else{
                    currentSearch = this->memory->staticAllocate(i);
                }
                if(this->doCoalesce){
                    currentSearch += this->memory->coalesce();
                }
                this->printStatus(1, currentSearch, i);

            }else{
                if(1 == mode){
                    currentSearch = this->memory->freeSpace(abs(i));
                }else{
                    currentSearch = this->memory->staticFree(abs(i));
                }
                if(this->doCoalesce){
                    currentSearch += this->memory->coalesce();
                }
                this->printStatus(0, currentSearch, abs(i));
            }
            if(currentSearch < 0){ // invalid operation may be included when in mode 1
                currentSearch = 0;
            }
            currentSearch += 1; //add the 1 operation count for free or allocation call
            totalSearch += currentSearch;
            searchLog.push_back(currentSearch);
        }
        std::cout << "Total operations: " << totalSearch << " times\n";
        std::cout << "Operation Count Log: \n[ ";
        for(int i : searchLog){
            std::cout << i << ", ";
        }
        std::cout << "]\n";
        std::cout << "============================    Simulation Complete    ============================\n";
    }





private:
    /**
     * generate process vector.
     * @param userInput string of processes e.g."10,-10,8,-9".
     * @return vector<int> random or based on user input.
     */
    std::vector<int> getProcessList(std::string userInput){
        if(this->isRandom){
            return this->getRandomProcesses();
        }else{
            return this->parseInputToVector(userInput);
        }
    }


    /**
     * Generate process vector with random numbers.
     * @return vector of int between [-50, 50].
     */
    std::vector<int> getRandomProcesses(){
        std::vector<int> result;
        auto randomEngine = std::mt19937 {};
        auto const ticks = std::chrono::system_clock::now().time_since_epoch().count();
        randomEngine.seed(ticks);
        std::uniform_int_distribution randomDistributor {-50, 50}; //generate random ints between -50 to 50
        for(int i = 0 ; i < this->processCount; i++){
            result.push_back(randomDistributor(randomEngine));
        }
        return result;
    }


    /**
     * Parse string input with commas to vector<int>
     * @param userInput string of commas, e.g. "10,20,-10,5"
     * @return vector<int>
     */
    std::vector<int> parseInputToVector(std::string userInput){
        std::stringstream parsingInput(userInput);
        std::vector<int> result;

        while(parsingInput.good()){
            std::string current;
            getline(parsingInput, current, ',');
            result.push_back(std::stoi(current));
        }

        return result;
    }


    /**
     * print current work status.
     * @param workType 1 - allocate, 0 - free.
     * @param searchCount int. total search used in this work.
     */
    void printStatus(int workType, int searchCount, int requirement){
        std::string currentWorkType;
        if(1 == workType){
            currentWorkType = "Allocate";
        }else{
            currentWorkType = "Free";
        }
        std::cout << "Work Required: " << currentWorkType << " " << requirement << "kb" << "\n";
        if(searchCount >= 0){
            std::cout << "Operation Count: " << searchCount << "\n";
            std::cout << "Current Memory Map:\n";
            this->memory->printMemory();
        }else{
            std::cout << "| -- Invalid Requirement -- |\n";
            if(1 == workType){
                std::cout << "Can't allocate space when the required space is greater than space left.\n";
            }else{
                std::cout << "Can't free space when the memory is all freed. \n";
            }
        }


        std::cout << "-----------------------------------------------\n";
    }

};


#endif //CS5600_FSM_SIMULATOR_FSCONTROLLERWITHVIEW_H
