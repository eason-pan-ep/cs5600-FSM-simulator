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

void test_run_simulation_random(){
    auto* testRun = new FSControllerWithView(0, 1024, 5, true);
    testRun->runSimulation();
    delete(testRun);
}

void test_run_simulation_random_coalesce(){
    auto* testRun = new FSControllerWithView(0, 1024, 5, true, true);
    testRun->runSimulation();
    delete(testRun);
}




int main(){
    std::cout << "*****************************************************\n";
    std::cout << "Print Random Processes Test:\n";
    test_print_processes_random();

    std::cout << "*****************************************************\n";
    std::cout << "Print Given Processes Test:\n";
    test_print_process_given();

    std::cout << "*****************************************************\n";
    std::cout << "Running Simulation Test -- NO coalesce: \n";
    test_run_simulation_random();

    std::cout << "*****************************************************\n";
    std::cout << "Running Simulation Test -- with coalesce: \n";
    test_run_simulation_random_coalesce();


    return 0;
}