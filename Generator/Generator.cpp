// Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "shared_memory.h"

#include <iostream>
#include <random>
#include <thread>

int wmain(int argc, wchar_t* argv[])
{
    HANDLE hMapFile, hEvent;
    SharedMemoryBlock* sharedData = CreateSharedMemory(hMapFile);
    hEvent = CreateEventSync();

    if (!sharedData || !hEvent) {
        std::cerr << "Generator: Failed to initialize shared memory or event!" << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    while (true) {
        sharedData->num1 = dist(gen);
        sharedData->num2 = dist(gen);
        sharedData->data_ready = true;

        std::wcout << L"Generator: Produced (" << sharedData->num1 << L", " << sharedData->num2 << L")" << std::endl;

        SignalEvent(hEvent);  // Notify Processor
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Simulate work
    }

    CloseSharedMemory(hMapFile, sharedData);
    CloseEvent(hEvent);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
