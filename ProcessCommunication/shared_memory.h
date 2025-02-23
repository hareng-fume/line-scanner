#pragma once

#include <windows.h>
#include <atomic>

#define SHARED_MEMORY_NAME L"Local\\MySharedMemory"
#define SHARED_MEMORY_SIZE sizeof(SharedMemoryBlock)

#define EVENT_NAME L"Local\\DataReadyEvent"

struct SharedMemoryBlock {
    // std::atomic_flag lock = ATOMIC_FLAG_INIT;
    int num1;
    int num2;
    std::atomic<bool> data_ready = false;
};

// Functions to manage shared memory
SharedMemoryBlock* CreateSharedMemory(HANDLE& hMapFile);
SharedMemoryBlock* OpenSharedMemory(HANDLE& hMapFile);
void CloseSharedMemory(HANDLE hMapFile, SharedMemoryBlock* sharedData);

// Functions to manage events
HANDLE CreateEventSync();
HANDLE OpenEventSync();
void SignalEvent(HANDLE hEvent);
void WaitForEvent(HANDLE hEvent);
void CloseEvent(HANDLE hEvent);
