#include "PlatformSpecific.h"

namespace Minecraft
{
	namespace PlatformSpecific
	{
#ifdef _WIN32

		void GetProcDebugInfo(ProcessDebugInfo& info)
		{
			static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
			static int numProcessors;
			static HANDLE self;
			static bool first_run = true;

			if (first_run)
			{
				first_run = false;

				SYSTEM_INFO sysInfo;
				FILETIME ftime, fsys, fuser;

				GetSystemInfo(&sysInfo);
				numProcessors = sysInfo.dwNumberOfProcessors;

				GetSystemTimeAsFileTime(&ftime);
				memcpy(&lastCPU, &ftime, sizeof(FILETIME));

				self = GetCurrentProcess();
				GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
				memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
				memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
			}

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			info.total_vm = memInfo.ullTotalPageFile;
			info.total_vm_used = (DWORDLONG) memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
			
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			SIZE_T proc_vm = pmc.PrivateUsage;
			info.vm = proc_vm;

			DWORDLONG phys_mem = memInfo.ullTotalPhys;
			info.total_mem = phys_mem;
			info.memory = pmc.WorkingSetSize;

			// To get the current cpu usage by proc

			FILETIME ftime, fsys, fuser;
			ULARGE_INTEGER now, sys, user;
			double percent;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&now, &ftime, sizeof(FILETIME));

			GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			memcpy(&sys, &fsys, sizeof(FILETIME));
			memcpy(&user, &fuser, sizeof(FILETIME));
			percent = (sys.QuadPart - lastSysCPU.QuadPart) +
				(user.QuadPart - lastUserCPU.QuadPart);
			percent /= (now.QuadPart - lastCPU.QuadPart);
			percent /= numProcessors;
			lastCPU = now;
			lastUserCPU = user;
			lastSysCPU = sys;
			percent *= 100;

			info.cpu_usage = percent;
		}

#endif

#ifdef _LINUX
		void GetProcDebugInfo(ProcessDebugInfo& info)
		{
			// TODO : Get this on linux
			info = { 0, 0, 0, 0, 0 };
		}
#endif 
	}
}