#pragma once

#include <Core/config.h>

#include <string>
#include <iostream>
#include <stdio.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#else
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace terminus
{
	namespace process_dispatch
	{
		enum class ProcessExitCode
		{
			SUCCESS = 0,
			FAILED = 99999
		};

		enum class ProcessStartCode
		{
			SUCCESS = 0,
			FAILED = 99999
		};

#if defined(TERMINUS_PLATFORM_WIN32)

		struct ProcessHandle
		{
			PROCESS_INFORMATION proc_info;
			STARTUPINFO			startup_info;
			HANDLE				child_std_out_read;
			HANDLE				child_std_out_write;
			HANDLE				child_std_err_read;
			HANDLE				child_std_err_write;
			ProcessExitCode		exit_code;
			ProcessStartCode	start_code;
		};

#else
		struct ProcessHandle
		{
			ProcessExitCode		exit_code;
			ProcessStartCode	start_code;
			FILE*               pipe;
		};

#endif

		extern ProcessHandle create(const char* cmd);
		extern void close(ProcessHandle& handle);

	}
}