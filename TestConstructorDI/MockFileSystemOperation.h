#pragma once
#include "pch.h"
#include <vector>
#include <Windows.h>
#include <LM.h>
#include <functional>
#include "ReadFileLine.h"

	typedef std::function <HANDLE WINAPI(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE)>MockCreateFile;
	typedef std::function <DWORD WINAPI(HANDLE, LPDWORD)> MockGetFileSize;
	typedef std::function <BOOL WINAPI(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED)> MockReadFile;
	typedef std::function <BOOL WINAPI(HANDLE)> MockCloseHandle;


	class  MockFileSystemOperation final : virtual public IWindowsFileSystem
	{
	public:
		MockFileSystemOperation() = default;
		~MockFileSystemOperation() = default;

		MockFileSystemOperation(const MockFileSystemOperation&) = delete;
		MockFileSystemOperation(const MockFileSystemOperation&&) = delete;
		MockFileSystemOperation& operator=(const MockFileSystemOperation&) = delete;
		MockFileSystemOperation& operator=(const MockFileSystemOperation&&) = delete;

		MockCreateFile CreateFileFunc = nullptr;
		MockGetFileSize GetFileSizeFunc = nullptr;
		MockReadFile ReadFileFunc = nullptr;
		MockCloseHandle CloseHandleFunc = nullptr;

		BOOL WINAPI CloseHandle(
			_In_	HANDLE       hFile
		) override
		{
			if (CloseHandleFunc != nullptr)
			{
				return CloseHandleFunc(hFile);
			}

			throw std::exception("CloseHandle Mock is not implemented.");
		}


		BOOL WINAPI ReadFile(
			_In_ HANDLE hFile,
			_Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
			_In_ DWORD nNumberOfBytesToRead,
			_Out_opt_ LPDWORD lpNumberOfBytesRead,
			_Inout_opt_ LPOVERLAPPED lpOverlapped
		)
		{
			if (ReadFileFunc)
			{
				return ReadFileFunc(
					hFile,
					lpBuffer,
					nNumberOfBytesToRead,
					lpNumberOfBytesRead,
					lpOverlapped
				);
			}
			throw std::exception("ReadFile Mock is not implemented.");
		}

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
			if (CreateFileFunc)
			{
				return CreateFileFunc(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile
				);
			}
			throw std::exception("CreateFile Mock is not implemented.");
		}

		DWORD WINAPI GetFileSize(
			_In_	HANDLE  hFile,
			_Out_	LPDWORD lpFileSizeHigh
		)
		{
			if (GetFileSizeFunc)
			{
				return GetFileSizeFunc(
					hFile,
					lpFileSizeHigh
				);
			}
			throw std::exception("GetFileSize Mock is not implemented.");
		}
	};