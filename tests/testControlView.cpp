//
// Created by Eason Pan on 2023-10-15.
//
#include <iostream>
#include "../src/FSControllerWithView.h"

// this is a printing test
void test_print_processes_random(){
    auto* testPrint = new FSControllerWithView(0, 100, 2, true);
    testPrint->printProcesses();
    delete(testPrint);
}

void test_print_process_given(){
    auto* testPrint = new FSControllerWithView(0, 100, 3, false, false, "10,-20,8");
    testPrint->printProcesses();
    delete(testPrint);
}



// adding unit tests in this array
int (*unitTests[])() = {
        nullptr,
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
    if(testsCounter > 0){
        std::cout << "\n---------------------------------------\n";
        std::cout << testsPassed << " of " << testsCounter << " tests passed\n";
        if(testsPassed == testsCounter){
            std::cout << "❖ Passed All Tests!!\n";
        }
    }else{
        std::cout << "No Unit Test Provided.\n";
    }

    std::cout << "Printing Methods Tests:\n";
    std::cout << "Random Process:\n";
    test_print_processes_random();
    std::cout << "Given Process:\n";
    test_print_process_given();


    return 0;
}