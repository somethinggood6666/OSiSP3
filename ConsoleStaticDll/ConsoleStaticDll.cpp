#include <iostream>
#include "../Dll/memanalyze.h"

int main()
{
    std::wstring abc = L"ababcababc";
    std::wcout << L"Start \"abc\" value: " << abc << std::endl;
    int rplCount = replaceMemory(L"abc", L"cba");
    std::wcout << L"End \"abc\" value: " << abc << std::endl;
    std::cout << "Replaces: " << rplCount << std::endl;
    return 0;
}
