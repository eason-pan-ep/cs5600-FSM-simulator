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

int test_destructor(){
    Memory* test1 = new Memory(10);
    delete(test1);
    return 1;
}

int test_allocateItem_sizeEqualToCurrentChunk(){
    Memory* testM = new Memory(1);
    int searchCount = testM->allocateSpace(1);
    if(0 == testM->getSpaceLeft() && 0 == searchCount && 1 == testM->getChunkCount()){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_allocateItem_sizeSmallerThanCurrentChunk(){
    Memory* testM = new Memory(3);
    int searchCount = 0;
    searchCount += testM->allocateSpace(1);
    searchCount += testM->allocateSpace(1);
    if(1 == testM->getSpaceLeft() && 0 == searchCount && 3 == testM->getChunkCount()){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_allocateItem_to_fullMemory(){
    Memory* testM = new Memory(3);
    testM->allocateSpace(3);
    int result = testM->allocateSpace(1);
    if(-1 == result){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_free_empty(){
    Memory* testM = new Memory(2);
    int result = testM->freeSpace(1);
    if(-1 == result){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_free_size(){
    int result[3];
    Memory* testM = new Memory(3);
    testM->allocateSpace(2);
    result[0] = testM->getSpaceLeft();
    testM->freeSpace(1);
    result[1] = testM->getSpaceLeft();
    result[2] = testM->freeSpace(3);
    if(1 == result[0] && 2 == result[1] && -1 == result[2]){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_coalesce_noChange(){
    Memory* testM = new Memory(3);
    testM->allocateSpace(2);
    testM->freeSpace(1);
    int result1 = testM->getChunkCount(); //expect 3
    testM->coalesce();
    if(result1 == testM->getChunkCount()){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}

int test_coalesce_change(){
    Memory* testM = new Memory(3);
    testM->allocateSpace(1);
    testM->freeSpace(1);
    testM->coalesce();
    if(1 == testM->getChunkCount()){
        delete(testM);
        return 1;
    }
    delete(testM);
    return 0;
}




// adding unit tests in this array
int (*unitTests[])() = {
        test_memory_constructor_invalid_capacity_1,
        test_memory_constructor_invalid_capacity_2,
        test_destructor, //check results through memory leak check
        test_allocateItem_sizeEqualToCurrentChunk,
        test_allocateItem_sizeSmallerThanCurrentChunk,
        test_allocateItem_to_fullMemory,
        test_free_empty,
        test_free_size,
        test_coalesce_noChange,
        test_coalesce_change,
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
        std::cout << "❖ Passed All Tests!!\n";
    }

    return 0;
}
