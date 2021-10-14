#include "memanalyze.h"
#include "pch.h"

int replaceMemory(std::wstring src, std::wstring rpl)
{
    void* pMemory = 0;

    int memStrPos = 0;
    wchar_t* pMemoryStrStart = 0;
    int length = src.size();
    int rplLength = rpl.size();

    int res = 0;

    while (rplLength < length) {
        rpl += L" ";
        rplLength++;
    }

    if (rplLength > length) {
        rpl = rpl.substr(0, length);
        rplLength = length;
    }

    MEMORY_BASIC_INFORMATION memInfo;

    for (; VirtualQuery(pMemory, &memInfo, sizeof(memInfo)); 
        pMemory = (char*)pMemory + memInfo.RegionSize)
    {
        if (memInfo.AllocationProtect != PAGE_READWRITE ||
            memInfo.Protect != PAGE_READWRITE)
        {
            memStrPos = 0;
            pMemoryStrStart = (wchar_t*)((char*)pMemory + memInfo.RegionSize);
            continue;
        } 
            

        for (wchar_t* pMemoryW = (wchar_t*)pMemory;
            (char*)pMemoryW < (char*)pMemory + memInfo.RegionSize;) 
        {
            if (*pMemoryW == src[memStrPos] && 
                (pMemoryW < (wchar_t*)&src || pMemoryW > ((wchar_t*)&src + length)))
            {
                memStrPos++;
                pMemoryW++;

                if (memStrPos >= length)
                {
                    for (int i = 0; i < length; i++)
                    {
                        *pMemoryStrStart = rpl[i];
                        pMemoryStrStart++;
                    }

                    memStrPos = 0;
                    res++;
                }
            }
            else
            {
                pMemoryStrStart++;
                pMemoryW = pMemoryStrStart;
                memStrPos = 0;
            }
        }
           
    }

    return res;
}