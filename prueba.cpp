#include <windows.h>
#include <process.h>
#include <iostream>

#define SIZE 4096

using namespace std;

int main() {
    HANDLE hMapFile;
    char* pBuf;

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            SIZE,
            TEXT("SharedObject")
    );

    if (hMapFile == NULL) {
        cerr << "Could not create file mapping object: " << GetLastError() << endl;
        return 1;
    }

    pBuf = (char*) MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            SIZE
    );

    if (pBuf == NULL) {
        cerr << "Could not map view of file: " << GetLastError() << endl;
        CloseHandle(hMapFile);
        return 1;
    }

    strcpy(pBuf, "Hello, child process!");

    _spawnl(_P_WAIT, "child.exe", "child.exe", NULL);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    

    return 0;
}