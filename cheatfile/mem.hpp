#pragma once
#include "util/vmmdll/leechcore.h"
#include "util/vmmdll/vmmdll.h"
#include <iostream>
class _DMAA
{
public:
    VMM_HANDLE hVMM = NULL;
    BOOL result;
    DWORD ProcessID = 0;
    std::string ProcessName = "Discovery.exe"; // Use a normal string here
    //std::string dllname = "client.dll";
    uintptr_t modbase;
    VMMDLL_SCATTER_HANDLE hS = NULL;
    VMMDLL_SCATTER_HANDLE hS2 = NULL;
    VMMDLL_SCATTER_HANDLE hS3 = NULL;
    VMMDLL_SCATTER_HANDLE hS4 = NULL;
public:



    void _DMA() {
        std::cout << "------------------------------------------------------------\n";
        std::cout << "# Initialize from FPGA:\n";
        LPSTR args[] = { (LPSTR)"-norefresh", (LPSTR)"-device", (LPSTR)"FPGA" };
        hVMM = VMMDLL_Initialize(3, args);
        
        if (hVMM) {
            std::cout << "SUCCESS: VMMDLL_Initialize\n";
        }
        else {
            std::cerr << "FAIL: VMMDLL_Initialize\n";
            throw std::runtime_error("VMMDLL_Initialize failed.");
        }



        if (VMMDLL_PidGetFromName(hVMM, (LPSTR)ProcessName.c_str(), &ProcessID)) {
            std::cout << "SUCCESS: Obtained PID for discovery.exe: " << ProcessID << "\n";
        }
        else {
            std::cerr << "FAIL: Unable to obtain PID for discovery.exe\n";
        }

        hS = VMMDLL_Scatter_Initialize(hVMM, ProcessID, 1);
        if (hS == NULL) {
            std::cerr << "FAIL: VMMDLL_SCATTER_HANDLE\n";
            throw std::runtime_error("VMMDLL_SCATTER_HANDLE fail.");
        }
        else {
            std::cout << "SUCCESS: VMMDLL_SCATTER_HANDLE\n";
        }

        hS2 = VMMDLL_Scatter_Initialize(hVMM, ProcessID, 1);
        if (hS2 == NULL) {
            std::cerr << "FAIL: VMMDLL_SCATTER_HANDLE\n";
            throw std::runtime_error("VMMDLL_SCATTER_HANDLE fail.");
        }
        else {
            std::cout << "SUCCESS: VMMDLL_SCATTER_HANDLE\n";
        }
        hS3 = VMMDLL_Scatter_Initialize(hVMM, ProcessID, 1);
        if (hS3 == NULL) {
            std::cerr << "FAIL: VMMDLL_SCATTER_HANDLE\n";
            throw std::runtime_error("VMMDLL_SCATTER_HANDLE fail.");
        }
        else {
            std::cout << "SUCCESS: VMMDLL_SCATTER_HANDLE\n";
        }
        hS4 = VMMDLL_Scatter_Initialize(hVMM, ProcessID, 1);
        if (hS4 == NULL) {
            std::cerr << "FAIL: VMMDLL_SCATTER_HANDLE\n";
            throw std::runtime_error("VMMDLL_SCATTER_HANDLE fail.");
        }
        else {
            std::cout << "SUCCESS: VMMDLL_SCATTER_HANDLE\n";
        }

    }
    uintptr_t getModbase() {
        return modbase = VMMDLL_ProcessGetModuleBaseU(hVMM, ProcessID, (LPSTR)ProcessName.c_str());
    }
    template <typename T>
    T readMemory(ULONG64 qwA) {
        T value{};
        DWORD bytesRead;
        if (VMMDLL_MemReadEx(hVMM, ProcessID, qwA, reinterpret_cast<PBYTE>(&value), sizeof(T), &bytesRead, VMMDLL_FLAG_NOCACHE)) {
            // ????
            return value;
        }
        else {
            // ?????????
            throw std::runtime_error("Memory read failed.");
        }
    }

    template <typename T>
    void readMemoryArray(DWORD dwPID, ULONG64 qwA, T* buffer, size_t count) {
        DWORD bytesRead;
        size_t size = sizeof(T) * count;
        if (VMMDLL_MemReadEx(hVMM, dwPID, qwA, reinterpret_cast<PBYTE>(buffer), size, &bytesRead, VMMDLL_FLAG_NOCACHE)) {
            // ????
            if (bytesRead != size) {
                throw std::runtime_error("Partial memory read.");
            }
        }
        else {
            // ?????????
            throw std::runtime_error("Memory read failed.");
        }
    }


    bool SPrepare(VMMDLL_SCATTER_HANDLE wtf,uint64_t va, DWORD cb) {
        if (!VMMDLL_Scatter_Prepare(wtf, va, cb)) {
            std::cout << "Scatter prepare failed." << std::endl;
        }

        return true;
    }


    bool ExecuteMemoryReads(VMMDLL_SCATTER_HANDLE wtf) {
        if (!VMMDLL_Scatter_ExecuteRead(wtf)) {
            throw std::runtime_error("Scatter execute read failed.");
        }
    }

    template<typename T>
    T SReads(VMMDLL_SCATTER_HANDLE wtf, uint64_t ptr) {
        T buff;
        DWORD pcbRead;
        if (!VMMDLL_Scatter_Read(wtf, ptr, sizeof(T), reinterpret_cast<PBYTE>(&buff), &pcbRead)) {
            throw std::runtime_error("Scatter read failed.");
        }
        if (pcbRead != sizeof(T)) {
            std::cout << "Partial Scatter read." << std::endl;
        }
        return buff;
    }


    template<typename T>
    void SReadformatrix(VMMDLL_SCATTER_HANDLE wtf, uint64_t ptr, T* buff, size_t size) {
        DWORD pcbRead;
        if (!VMMDLL_Scatter_Read(wtf, ptr, size, reinterpret_cast<PBYTE>(buff), &pcbRead)) {
            std::cout << "failed read" << std::endl;
        }
        if (pcbRead != size) {
            std::cout << "failed read" << std::endl;
        }
    }

    bool SClear(VMMDLL_SCATTER_HANDLE wtf, DWORD flags)
    {
        return  VMMDLL_Scatter_Clear(wtf, ProcessID, flags);
    }


    bool Scatter_Read(VMMDLL_SCATTER_HANDLE HS, uint64_t addr, PVOID pBuf, DWORD size)
    {
        return VMMDLL_Scatter_Read(HS, addr, size, (PBYTE)pBuf, 0);
    }
    void ReadMatrix(VMMDLL_SCATTER_HANDLE HS, uint64_t addr, DOUBLE viewMatrix[4][4]) {
        DWORD matrixSize = sizeof(DOUBLE) * 4 * 4; 
        Scatter_Read(HS, addr, viewMatrix, matrixSize);
    }
    template<typename T>

    T SRead(VMMDLL_SCATTER_HANDLE HS, uint64_t ptr)
    {
        T buff;
        Scatter_Read(HS, ptr, &buff, sizeof(T));
        return buff;
    }
    void SReadmatrix(VMMDLL_SCATTER_HANDLE HS, uint64_t ptr, PVOID pBuf, size_t size)
    {
        Scatter_Read(HS, ptr, pBuf, size);
    }

    template <typename T>
    T readMemoryScatter( ULONG64 qwA) {
        SClear(hS, VMMDLL_FLAG_NOCACHE);

        T value{};
        DWORD bytesRead;
        if (!VMMDLL_Scatter_PrepareEx(hS, qwA, sizeof(T), reinterpret_cast<PBYTE>(&value), &bytesRead)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter prepare failed.");
        }

        if (!VMMDLL_Scatter_ExecuteRead(hS)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter execute read failed.");
        }

        //VMMDLL_Scatter_CloseHandle(hS);
        return value;
    }

    template <typename T>
    void readMemoryScatter2(ULONG64 qwA, T& buffer, size_t size) {
        SClear(hS, VMMDLL_FLAG_NOCACHE);

        DWORD bytesRead;
        if (!VMMDLL_Scatter_PrepareEx(hS, qwA, size, reinterpret_cast<PBYTE>(&buffer), &bytesRead)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter prepare failed.");
        }

        if (!VMMDLL_Scatter_ExecuteRead(hS)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter execute read failed.");
        }

        // VMMDLL_Scatter_CloseHandle(hS); // Consider when to close the handle
    }


    template <typename T>
    void readMemoryArrayScatter(ULONG64 qwA, T* buffer, size_t count) {
        SClear(hS, VMMDLL_FLAG_NOCACHE);
        size_t size = sizeof(T) * count;
        if (!VMMDLL_Scatter_PrepareEx(hS, qwA, static_cast<DWORD>(size), reinterpret_cast<PBYTE>(buffer), nullptr)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter prepare failed.");
        }

        // ????
        if (!VMMDLL_Scatter_ExecuteRead(hS)) {
            VMMDLL_Scatter_CloseHandle(hS);
            throw std::runtime_error("Scatter execute read failed.");
        }

        // ?? Scatter ????
       // VMMDLL_Scatter_CloseHandle(hS);
    }
};

_DMAA dma;


