#include "pch.h"
#include "shared_memory.h"

#include <iostream>

//-----------------------------------------------------------------------------
SharedMemoryBlock* CreateSharedMemory(HANDLE& hMapFile) {
    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE
        , NULL
        , PAGE_READWRITE
        , 0
        , SHARED_MEMORY_SIZE
        , SHARED_MEMORY_NAME);

    if (!hMapFile) {
        std::cerr << "Failed to create shared memory!" << std::endl;
        return nullptr;
    }
    return (SharedMemoryBlock*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEMORY_SIZE);
}

//-----------------------------------------------------------------------------
SharedMemoryBlock* OpenSharedMemory(HANDLE& hMapFile) {
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEMORY_NAME);
    if (!hMapFile) {
        std::cerr << "Failed to open shared memory!" << std::endl;
        return nullptr;
    }
    return (SharedMemoryBlock*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEMORY_SIZE);
}

//-----------------------------------------------------------------------------
void CloseSharedMemory(HANDLE hMapFile, SharedMemoryBlock* sharedData) {
    UnmapViewOfFile(sharedData);
    CloseHandle(hMapFile);
}

//-----------------------------------------------------------------------------
HANDLE CreateEventSync() {
    return CreateEvent(NULL, FALSE, FALSE, EVENT_NAME);
}

//-----------------------------------------------------------------------------
HANDLE OpenEventSync() {
    return OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, EVENT_NAME);
}

//-----------------------------------------------------------------------------
void SignalEvent(HANDLE hEvent) {
    SetEvent(hEvent);
}

//-----------------------------------------------------------------------------
void WaitForEvent(HANDLE hEvent) {
    WaitForSingleObject(hEvent, INFINITE);
}

//-----------------------------------------------------------------------------
void CloseEvent(HANDLE hEvent) {
    CloseHandle(hEvent);
}