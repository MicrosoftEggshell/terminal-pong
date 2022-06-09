#include <locale.h>

short CONSOLE_H;
short CONSOLE_W;

#ifdef _WIN32

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>

void EnableControlCodes(void) {
    DWORD consoleMode;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(consoleHandle, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // colour codes, cursor positioning - https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    SetConsoleMode(consoleHandle, consoleMode);
}

BOOL unicodeSupported = FALSE;

BOOL CALLBACK checkUnicode(TCHAR* cp){
    if(0 == strcmp(cp, "65001"))
        unicodeSupported = TRUE;
    return TRUE;
}

void FixCodepage(void) {
    setlocale(LC_ALL, "");
    EnumSystemCodePages(checkUnicode, CP_SUPPORTED);
    if(unicodeSupported){
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        _setmode(_fileno(stdout), _O_U8TEXT);
    }
}

short GetConsoleSize(void){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    // I don't think top and left are ever anything other than 1, but oh well
    CONSOLE_H = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    CONSOLE_W = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
}

void platform_init(void){
    EnableControlCodes();
    GetConsoleSize();
    FixCodepage();
}

short up_pressed(void){
    return GetKeyState(VK_UP) & 0xF000; // GetKeyState returns short; higher order bit determines whether it's pressed
}

short w_pressed(void){
    return GetKeyState('W') & 0xF000;
}

short s_pressed(void){
    return GetKeyState('S') & 0xF000;
}

short down_pressed(void){
    return GetKeyState(VK_DOWN) & 0xF000;
}

#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)) // http://web.archive.org/web/20160306052035/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system#HowtodetectPOSIXandUNIX

void platform_init(void){}
// TODO

#else // end POSIX
void platform_init(void){
    printf("What the fuck sort of OS are you running\n");
    exit(1);
}
#endif // end OS detection
