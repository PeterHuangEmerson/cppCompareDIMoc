#pragma once
#include <string>
#include <filesystem>

using namespace std;


class IWindowsFileSystem
{
public:
	
	virtual HANDLE WINAPI CreateFile(
		_In_	LPCSTR                lpFileName,
		_In_	DWORD                 dwDesiredAccess,
		_In_	DWORD                 dwShareMode,
		_In_opt_	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_	DWORD                 dwCreationDisposition,
		_In_	DWORD                 dwFlagsAndAttributes,
		_In_opt_	HANDLE                hTemplateFile
	) = 0;

	virtual DWORD WINAPI GetFileSize(
		_In_	HANDLE  hFile,
		_Out_opt_	LPDWORD lpFileSizeHigh
	) = 0;

	virtual BOOL WINAPI ReadFile(
		_In_ HANDLE hFile,
		_Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
		_In_ DWORD nNumberOfBytesToRead,
		_Out_opt_ LPDWORD lpNumberOfBytesRead,
		_Inout_opt_ LPOVERLAPPED lpOverlapped
	) = 0;

	virtual BOOL WINAPI CloseHandle(
		_In_	HANDLE       hFile
	) = 0;

	virtual ~IWindowsFileSystem() = default;
	IWindowsFileSystem() = default;

	IWindowsFileSystem(const IWindowsFileSystem&) = delete;
	IWindowsFileSystem(const IWindowsFileSystem&&) = delete;
	IWindowsFileSystem& operator=(const IWindowsFileSystem&) = delete;
	IWindowsFileSystem& operator=(const IWindowsFileSystem&&) = delete;

};


//class FileSystemOperation final : virtual public IWindowsFileSystem
class FileSystemOperation  : virtual public IWindowsFileSystem
{
public: 
	FileSystemOperation() {}

	HANDLE WINAPI CreateFile(
		_In_	LPCSTR                lpFileName,
		_In_	DWORD                 dwDesiredAccess,
		_In_	DWORD                 dwShareMode,
		_In_opt_	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_	DWORD                 dwCreationDisposition,
		_In_	DWORD                 dwFlagsAndAttributes,
		_In_opt_	HANDLE                hTemplateFile
	) override
	{
		return ::CreateFileA(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile
		);
	}

	BOOL WINAPI CloseHandle(
		_In_	HANDLE       hFile
	)
	{
		return ::CloseHandle(hFile);
	}

	BOOL WINAPI ReadFile(
		_In_ HANDLE hFile,
		_Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
		_In_ DWORD nNumberOfBytesToRead,
		_Out_opt_ LPDWORD lpNumberOfBytesRead,
		_Inout_opt_ LPOVERLAPPED lpOverlapped
	)
	{
		return ::ReadFile(
			hFile,
			lpBuffer,
			nNumberOfBytesToRead,
			lpNumberOfBytesRead,
			lpOverlapped
		);
	}

	DWORD WINAPI GetFileSize(
		_In_	HANDLE  hFile,
		_Out_	LPDWORD lpFileSizeHigh
	)
	{
		return ::GetFileSize(
			hFile,
			lpFileSizeHigh
		);
	}
};