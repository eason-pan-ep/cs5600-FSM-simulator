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
    FSControllerWithView(int headerSize=0, int memoryCapacity=1024, int processCount=10, bool isRandom=false, bool doCoalesce=false, std::string userInput=""){
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
        std::cout << "Processes: ";
        for(int i : this->processes){
            std::cout << i << ", ";
        }
        std::cout << "\n";
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



};


#endif //CS5600_FSM_SIMULATOR_FSCONTROLLERWITHVIEW_H
