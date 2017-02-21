// Anti-debug_tls.cpp : 定义控制台应用程序的入口点。
//
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib,"ntdll.lib")

extern "C" NTSTATUS NTAPI NtQueryInformationProcess(HANDLE hProcess, ULONG InfoClass, PVOID Buffer, ULONG Length, PULONG ReturnLength);

#define NtCurrentProcess() (HANDLE)-1

void NTAPI __stdcall TLS_CALLBACK(PVOID DllHandle, DWORD dwReason, PVOID Reserved)
{
	HANDLE DebugPort = NULL;
	if (!NtQueryInformationProcess(NtCurrentProcess(), 7, &DebugPort, sizeof(HANDLE), NULL))
	{
		if (DebugPort)
		{
			MessageBoxA(NULL, "I caught you!", "goodbye", MB_ICONSTOP);
			exit(0);
		}
	}
}

#ifdef _M_IX86
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")
#else
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback")
#endif

EXTERN_C
#ifdef _M_X64
#pragma const_seg (".CRT$XLB")
const
#else
#pragma data_seg (".CRT$XLB")
#endif

PIMAGE_TLS_CALLBACK _tls_callback[] = { TLS_CALLBACK, 0 };
#pragma data_seg ()
#pragma const_seg ()

int main()
{
	char str[] = { 0x09 ,0x04 ,0x19 ,0x19 ,0x31 ,0x3E ,0x26 ,0x39 ,0x15 ,0x0B ,0x24 ,0x3E ,0x23 ,0x67 ,0x2E ,0x2F ,0x28 ,0x3F ,0x2D ,0x37, 0 };
	for (int i = 0; str[i] != 0; i++)
		str[i] ^= 0x4A;
	return 0;
}
