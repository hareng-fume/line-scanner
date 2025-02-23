// Processor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "shared_memory.h"

#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
    std::wcout << "Processor started." << std::endl;

    HANDLE hMapFile, hEvent;
    SharedMemoryBlock* sharedData = OpenSharedMemory(hMapFile);
    hEvent = OpenEventSync();

    if (!sharedData || !hEvent) {
        std::wcerr << L"Processor: Failed to access shared memory or event!" << std::endl;
        return 1;
    }

    while (true) {
        WaitForEvent(hEvent);  // Wait for Generator

        if (sharedData->data_ready) {
            int result = sharedData->num1 + sharedData->num2;
            std::wcout << L"Processor: Received (" << sharedData->num1 << ", " << sharedData->num2 << L"), Sum = " << result << std::endl;
            sharedData->data_ready = false;  // Reset flag
        }
        // Optionally reset the event for next round if needed
        ResetEvent(hEvent);
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
