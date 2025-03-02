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

    class ProcessBase {
    public:
        explicit ProcessBase(const std::chrono::nanoseconds& i_ns)
            : mh_map_file(nullptr), mp_data(nullptr), m_ns(i_ns) {
        }
        ProcessBase(const ProcessBase&) = delete;
        ProcessBase& operator=(const ProcessBase&) = delete;

        ~ProcessBase() {
            if (mp_data) UnmapViewOfFile(mp_data);
            if (mh_map_file) CloseHandle(mh_map_file);
        }

        virtual bool Init() = 0;
        virtual void Start() = 0;

    protected:
        HANDLE mh_map_file;
        void* mp_data;

        std::chrono::nanoseconds m_ns;
    };

    //-------------------------------------------------------------------------
    template <typename TData>
    class ServerBase : public ProcessBase {
    protected:
        using data_type = TData;

    public:
        explicit ServerBase(const std::chrono::nanoseconds& i_ns)
            : ProcessBase(i_ns) {
        }

        bool Init() override {
            mh_map_file = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TData), TData::NAME);
            if (!mh_map_file) {
                std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
                return false;
            }

            mp_data = MapViewOfFile(mh_map_file, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TData));
            if (!mp_data) {
                std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
                CloseHandle(mh_map_file);
                return false;
            }

            return true;
        }
    };

    //-------------------------------------------------------------------------
    template <typename TData>
    class ClientBase : public ProcessBase {

    public:
        explicit ClientBase(const std::chrono::nanoseconds& i_ns)
            : ProcessBase(i_ns) {
        }

        bool Init() override {
            mh_map_file = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, TData::NAME);
            if (!mh_map_file) {
                std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
                Sleep(10000);
                return false;
            }

            mp_data = MapViewOfFile(mh_map_file, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TData));
            if (!mp_data) {
                std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
                CloseHandle(mh_map_file);
                Sleep(5000);
                return false;
            }

            return true;
        }
    };

} // namespace IPC
