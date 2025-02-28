#pragma once

#include "utils.h"
#include "shared_data.h"

#include <windows.h>
#include <iostream>

namespace IPC {

    /*struct HANDLE_DELETER {
        inline void operator()(HANDLE handle) const
        {
            if (handle && handle != INVALID_HANDLE_VALUE)
                CloseHandle(handle);
        }
    };
    using unique_handle = std::unique_ptr<void, HANDLE_DELETER>;

    template <typename T = void>
    struct VOF_DELETER {
        inline void operator()(T* pData) const {
            UnmapViewOfFile(pData);
        }
    };
    template <typename T = void>
    using unique_vof = std::unique_ptr<T, VOF_DELETER>;

    template <>
    struct VOF_DELETER<SharedData> {
        inline void operator()(SharedData* pData) const {
            UnmapViewOfFile(pData);
        }
    };

    using unique_sd = std::unique_ptr<SharedData, VOF_DELETER<SharedData>>;

    template <typename TSharedData>
    class FileMapping {
        public:
            static unique_handle Create() {
                HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE
                    , NULL
                    , PAGE_READWRITE
                    , 0
                    , sizeof(TSharedData)
                    , TSharedData::g_name);
                if (!hMapFile)
                    throw std::runtime_error("OpenFileMapping failed: Err " + std::to_string(GetLastError()));
                return hMapFile;
            }

        static TSharedData* MapView(HANDLE hMapFile) {
            TSharedData* pData = (TSharedData*)MapViewOfFile(hMapFile
                , FILE_MAP_ALL_ACCESS
                , 0
                , 0
                , sizeof(TSharedData));

            if (!pData) {
                CloseHandle(hMapFile);
                throw std::runtime_error("MapViewOfFile failed: Err " + std::to_string(GetLastError()));
            }
            return pData;
        }

        static unique_handle Open() {
            return OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TSharedData::g_name);
        }
    };*/

    //-------------------------------------------------------------------------
    template <typename TDerivedServer, typename TSharedData>
    class ServerBase {
    
    public:
        explicit ServerBase(const std::chrono::nanoseconds& i_ns)
            : hMapFile(nullptr), pData(nullptr), m_ns(i_ns) {
        }

        ~ServerBase() {
            if (pData) UnmapViewOfFile(pData);
            if (hMapFile) CloseHandle(hMapFile);
        }

        bool Init() {
            auto hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TSharedData), TSharedData::NAME);
            if (!hMapFile) {
                std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
                return false;
            }

            pData = static_cast<TSharedData*>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TSharedData)));
            if (!pData) {
                std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
                CloseHandle(hMapFile);
                return false;
            }

            return static_cast<TDerivedServer*>(this)->Init();
        }

        void Start() {
            static_cast<TDerivedServer*>(this)->Start();
        }

    protected:
        HANDLE hMapFile;
        TSharedData* pData;

        std::chrono::nanoseconds m_ns;
    };

    //-------------------------------------------------------------------------
    template <typename TSharedData>
    class ClientBase {

    };

} // namespace IPC
