#include <conio.h>
#include <regex>
#include <stdio.h>
#include <Windows.h>

int wmain()
{
    LPTSTR p = GetCommandLine();
    std::wcmatch m {};
    if (std::regex_match(p, m, std::wregex {LR"(".*"[[:s:]](.*))"}) ||
        std::regex_match(p, m, std::wregex {LR"(.*?[[:s:]](.*))"}))
    {
        const wchar_t farManager[] = L"d:\\far\\far.exe";
        size_t size = _countof(farManager) + m[1].length() + 1;
        p = static_cast<LPTSTR> (HeapAlloc(GetProcessHeap(), 0, size * sizeof(TCHAR)));
        if (p)
        {
            wcscpy_s(p, size, farManager);
            wcscat_s(p, size, L" ");
            wcscat_s(p, size, m[1].str().c_str());
            STARTUPINFO si {};
            si.cb = sizeof(STARTUPINFO);
            si.dwFlags = 0;
            PROCESS_INFORMATION pi;
            BOOL result = CreateProcess(
                farManager, p, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);
            if (result)
            {
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
            }
            HeapFree(GetProcessHeap(), 0, p);
        }
    }
    return 0;
}