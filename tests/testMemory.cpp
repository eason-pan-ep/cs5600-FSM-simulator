//
// Created by Eason Pan on 2023-10-13.
//
#include <iostream>
#include "../src/Memory.h"

int test_memory_constructor_invalid_capacity_1(){
    try{
        Memory(0);
    }catch (std::invalid_argument& e){
        return 1;
    }
    return 0;
}

int test_memory_constructor_invalid_capacity_2(){
    try{
        Memory(51);
    }catch (std::invalid_argument& e){
        return 1;
    }
    return 0;
}


// adding unit tests in this array
int (*unitTests[])() = {
        test_memory_constructor_invalid_capacity_1,
        test_memory_constructor_invalid_capacity_2,
        nullptr
};


int main(){

    unsigned int testsPassed = 0;
    int testsCounter = 0;

    while(unitTests[testsCounter] != nullptr){
        std::cout << "============= Unit Test " <<  (testsCounter+1) << " =============\n";
        if(1 == unitTests[testsCounter]()){
            std::cout << "✓ Passed tests\n";
            testsPassed ++;
        }else{
            std::cout << "✗ Failed tests\n";
        }
        testsCounter ++;
    }
    std::cout << "\n---------------------------------------\n";
    std::cout << testsPassed << " of " << testsCounter << " tests passed\n";
    if(testsPassed == testsCounter){
        std::cout << "❖ All tests passed!!\n";
    }

    return 0;
}
