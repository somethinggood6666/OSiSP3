#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <io.h>

DWORD GetProcessByName(wchar_t* processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32W process;
    process.dwSize = sizeof(PROCESSENTRY32W);
    DWORD procId = 0;
    if (Process32First(snapshot, &process))
    {
        while (Process32Next(snapshot, &process))
        {
            if (wcscmp(process.szExeFile, processName) == 0)
            {
                procId = process.th32ProcessID;
                break;
            }
        }
    }
    //else
    //{
    //    procId = GetLastError();
    //}
    CloseHandle(snapshot);
    return procId;
}

bool FileExist(wchar_t* name)
{
    return _waccess(name, 0) != -1;
}

bool Inject(DWORD pID, wchar_t* path)
{
    HANDLE proc_handle;
    LPVOID RemoteString;
    LPCVOID LoadLibAddy;
    if (pID == 0)
        return false;

    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
    if (proc_handle == 0)
        return false;

    LoadLibAddy = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    RemoteString = VirtualAllocEx(proc_handle, NULL, wcslen(path) * sizeof(wchar_t), 
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(proc_handle, RemoteString, path, wcslen(path) * sizeof(wchar_t), NULL);
    CreateRemoteThread(proc_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);
    CloseHandle(proc_handle);
    return true;
}
int main()
{
    wchar_t process_name[32];
    wchar_t dll_name[32];
    wchar_t path[256];
    std::wcout << L"Enter process name: ";
    std::wcin >> process_name;
    DWORD pID = GetProcessByName(process_name);
    std::wcout << L"Waiting " << process_name << L" for start..." << std::endl;
    for (;; Sleep(50))
    {
        if (pID == 0)
            pID = GetProcessByName(process_name);
        if (pID != 0) break;
    }

    std::wcout << process_name << L" found(pid = " << pID << L")!" << std::endl;
    while (FileExist(path) == false)
    {
        std::cout << "Enter DLL name: ";
        std::wcin >> dll_name;
        GetFullPathNameW(dll_name, sizeof(path) / sizeof(wchar_t), path, NULL);
        if (FileExist(path))
        {
            std::cout << "DLL found!" << std::endl;
            break;
        }
        else
            std::cout << "DLL not found!" << std::endl;
    }

    std::cout << "Preparing DLL for injection..." << std::endl;
    if (Inject(pID, path))
    {
        std::cout << "DLL successfully injected!" << std::endl;
    }
    else
    { 
        std::cout << "CRITICAL ERROR!" << std::endl << " Destroying window..." << std::endl;
    }
}
