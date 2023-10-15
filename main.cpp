#include <iostream>
#include "./src/FSControllerWithView.h"

int main() {
    std::cout << "#############################################################\n"
              << "########    Next Fit - Space Management Simulator   #########\n"
              << "#############################################################\n\n";

    //get user input
    int headerSize;
    std::cout << "Memory Header Size (0 - 100): ";
    std::cin >> headerSize;

    int memoryCapacity;
    std::cout << "Memory Capacity (headerSize - 10,000): ";
    std::cin >> memoryCapacity;


    int isRandomInput;
    std::cout << "Do you want randomly generated processes? (0-No, 1-Yes): ";
    std::cin >> isRandomInput;
    bool isRandom;
    if(1 == isRandomInput){
        isRandom = true;
    }else{
        isRandom = false;
    }

    std::string userInput;
    int processCount = 0;
    if(isRandom){
        std::cout << "Number of processes to generate: ";
        std::cin >> processCount;
    }else{
        std::cout << "Please type in your processes divided by ',' e.g. 20,-5,1,-50\n:";
        std::cin >> userInput;
    }

    int doCoalesceInput;
    std::cout << "Enable coalesce? (0-No, 1-Yes): ";
    std::cin >> doCoalesceInput;
    bool doCoalesce;
    if(1 == doCoalesceInput){
        doCoalesce = true;
    }else{
        doCoalesce = false;
    }

    //start running simulation and get output
    auto* nextFirstSimulation = new FSControllerWithView(headerSize, memoryCapacity, processCount, isRandom, doCoalesce, userInput);
    nextFirstSimulation->runSimulation();
    delete(nextFirstSimulation);

    return 0;
}
