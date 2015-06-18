#include "stdafx.h"
#include "pe_reader.hh"

#if 0
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
int main(void)
{
    PEReader pe_reader("pe_reader.dll");
    pe_reader.PatchRelocations();

    return 0;
}
#endif
