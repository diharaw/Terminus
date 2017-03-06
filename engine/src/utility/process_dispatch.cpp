#include <utility/process_dispatch.h>
#include <io/logger.h>

#define BUFSIZE 4096

namespace terminus
{
	namespace process_dispatch
	{

#if defined(TERMINUS_PLATFORM_WIN32)

		ProcessHandle create(const char* cmd)
		{
			ProcessHandle hndl;
			hndl.start_code = ProcessStartCode::FAILED;

			SECURITY_ATTRIBUTES sa;

			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&hndl.child_std_err_read, &hndl.child_std_err_write, &sa, 0))
				return hndl;

			if (!SetHandleInformation(hndl.child_std_err_read, HANDLE_FLAG_INHERIT, 0))
				return hndl;

			if (!CreatePipe(&hndl.child_std_out_read, &hndl.child_std_out_write, &sa, 0))
				return hndl;

			if (!SetHandleInformation(hndl.child_std_out_read, HANDLE_FLAG_INHERIT, 0))
				return hndl;

			wchar_t cmdline[1024];
			size_t len = strlen(cmd);

			mbstowcs(&cmdline[0], cmd, len + 1);

			ZeroMemory(&hndl.proc_info, sizeof(hndl.proc_info));

			ZeroMemory(&hndl.startup_info, sizeof(hndl.startup_info));
			hndl.startup_info.cb = sizeof hndl.startup_info;

			hndl.startup_info.hStdError = hndl.child_std_err_write;
			hndl.startup_info.hStdOutput = hndl.child_std_out_write;
			hndl.startup_info.dwFlags |= STARTF_USESTDHANDLES;
			
			BOOL success = CreateProcess(NULL,
										 (LPSTR)cmd,		 // command line
										 NULL,				 // process security attributes
										 NULL,				 // primary thread security attributes
										 TRUE,				 // handles are inherited
										 0,					 // creation flags
										 NULL,				 // use parent's environment
										 NULL,				 // use parent's current directory
										 &hndl.startup_info, // STARTUPINFO pointer
										 &hndl.proc_info);	 // receives PROCESS_INFORMATION

			if (success)
				hndl.start_code = ProcessStartCode::SUCCESS;

			return hndl;
		}

		void close(ProcessHandle& handle)
		{
			if (handle.start_code == ProcessStartCode::SUCCESS)
			{
				DWORD exit_code;
				WaitForSingleObject(handle.proc_info.hProcess, INFINITE);

				GetExitCodeProcess(handle.proc_info.hProcess, &exit_code);

				if (exit_code == 0)
					handle.exit_code = ProcessExitCode::SUCCESS;
				else
					handle.exit_code = ProcessExitCode::FAILED;

				CloseHandle(handle.child_std_err_write);
				CloseHandle(handle.child_std_out_write);
				CloseHandle(handle.proc_info.hProcess);
				CloseHandle(handle.proc_info.hThread);

				DWORD dwRead;
				CHAR chBuf[BUFSIZE];
				bool bSuccess = FALSE;
				std::string out = "", err = "";

				// print std out
				while (true)
				{
					bSuccess = ReadFile(handle.child_std_out_read, chBuf, BUFSIZE, &dwRead, NULL);

					if (!bSuccess || dwRead == 0)
						break;

					std::string s(chBuf, dwRead);
					T_LOG_INFO(s);
				}

				dwRead = 0;

				// print std err
				while (true)
				{
					bSuccess = ReadFile(handle.child_std_err_read, chBuf, BUFSIZE, &dwRead, NULL);

					if (!bSuccess || dwRead == 0)
						break;

					std::string s(chBuf, dwRead);
					T_LOG_INFO(s);

				}
			}
		}

#else

		ProcessHandle create(const char* cmd)
		{
			ProcessHandle handle;
			handle.start_code = ProcessStartCode::FAILED;
			handle.exit_code = ProcessExitCode::FAILED;

			if ((handle.pipe = (FILE*)popen(cmd, "r")))
				handle.start_code = ProcessStartCode::SUCCESS;

			return handle;
		}

		void close(ProcessHandle& handle)
		{
			if (handle.start_code == ProcessStartCode::SUCCESS)
			{
				char line[256];

				while (fgets(line, sizeof line, handle.pipe))
				{
					std::string l_line = std::string(line);
					std::cout << l_line;
				}
				int stat = pclose(handle.pipe);

				if (stat == 0)
					handle.exit_code = ProcessExitCode::SUCCESS;
				else
					handle.exit_code = ProcessExitCode::FAILED;
			}
		}

#endif

	}
}
