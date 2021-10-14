#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <windows.h>
#include <tchar.h>

extern "C" __declspec(dllexport) int replaceMemory(std::wstring src, std::wstring rpl);