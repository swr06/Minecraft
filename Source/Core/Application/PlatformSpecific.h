#include <iostream>

#ifdef _WIN32 || _WIN64
	#include <Windows.h>
	#include <Psapi.h>
	#include <tchar.h>
	#include <Pdh.h>
#endif

#ifdef _LINUX
	// Todo : Include linux libs here!
#endif

namespace Minecraft
{
	namespace PlatformSpecific
	{
		struct ProcessDebugInfo 
		{
			long long memory;
			long long total_mem;
			long long total_mem_used;
			long long vm;
			long long total_vm;
			long long total_vm_used;
			long cpu_usage;
		};

		void GetProcDebugInfo(ProcessDebugInfo& info);
	}
}