#include <iostream>
#include <windows.h>
#include <libloaderapi.h>

typedef int(__cdecl* replaceMemoryFunc)(std::wstring, std::wstring);

int main()
{
    std::wstring abc = L"ababcababc";
    std::wcout << L"Start \"abc\" value: " << abc << std::endl;

    HINSTANCE hlib = LoadLibrary(L"Dll.dll");
    replaceMemoryFunc replaceMemory = NULL;
    int result = -1;

    if (hlib) 
    {
        replaceMemory = (replaceMemoryFunc)GetProcAddress(hlib, "replaceMemory");
        if (replaceMemory) 
        {
            result = replaceMemory(L"abc", L"cba");
        }
        FreeLibrary(hlib);
    }

    std::wcout << L"End \"abc\" value: " << abc << std::endl;
    std::cout << "Replaces: " << result << std::endl;
}
