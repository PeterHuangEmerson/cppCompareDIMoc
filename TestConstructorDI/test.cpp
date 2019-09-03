#include "pch.h"
#include <vector>
#include <string.h>  
#include "ReadFileLine.h"
#include "MockFileSystemOperation.h"

typedef struct SourceVerification
{
	int size;
	string source;
	vector<string> verification;
}SourceVerificationType;

class TestWithDITestFixture : public ::testing::Test
{
protected:
	const HANDLE m_hMockProcess = reinterpret_cast<HANDLE>(0x44444444);
	const string m_Path = "d:\testing\abc.txt";
public:
	shared_ptr<ReadFileLine> readFileLineSp;
	MockFileSystemOperation mockFS;
	ProcessInput processInput;
	SelectLine selectLine;
	SourceVerificationType sourceVerification;
	bool m_FileOpened = false;

	TestWithDITestFixture() {}

	void SetupFSMock()
	{
		mockFS.CloseHandleFunc =
		MockCloseHandle([&](
			HANDLE hObject)-> BOOL
			{
				if (hObject == m_hMockProcess)
				{
					m_FileOpened = false;
				}

				return TRUE;
			});

		mockFS.CreateFileFunc =
		MockCreateFile([&](
			LPCSTR                lpFileName,
			DWORD                 dwDesiredAccess,
			DWORD                 dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD                 dwCreationDisposition,
			DWORD                 dwFlagsAndAttributes,
			HANDLE                hTemplateFile
			) -> HANDLE
			{
				//ASSERT_EQ(m_Path, lpFileName);
				m_FileOpened = true;
				return m_hMockProcess;
			});


		// work only with ASCII.
		mockFS.ReadFileFunc =
		MockReadFile([&](
			HANDLE hFile,
			LPVOID lpBuffer,
			DWORD nNumberOfBytesToRead,
			LPDWORD lpNumberOfBytesRead,
			LPOVERLAPPED lpOverlapped
			) -> BOOL
			{
				
				*lpNumberOfBytesRead = (sourceVerification.source.length() > nNumberOfBytesToRead) ?
					sourceVerification.source.length() : nNumberOfBytesToRead;

				strcpy_s((char *)lpBuffer, *lpNumberOfBytesRead, (const char*)sourceVerification.source.c_str());
					
				return true;
			});

		mockFS.GetFileSizeFunc =
		MockGetFileSize([&](
			HANDLE  hFile,
			LPDWORD lpFileSizeHigh
			) -> DWORD
			{
				*lpFileSizeHigh = 0;
				return (sourceVerification.source.length()+1);
			});
	}

	void SetUp() override
	{
		readFileLineSp = std::make_shared< ReadFileLine>(processInput, selectLine, mockFS);
		SetupFSMock();
	}

	void TearDown() override
	{
		readFileLineSp.reset();
	}
};

const string ResultArray1[] = { "ten-abcdef-10", "five--5", "twenty-abcdefghijklm-20" , "eleven-abcd-11" };

std::vector<std::string> ResultVector1(ResultArray1, ResultArray1 + sizeof(ResultArray1) / sizeof(string));

const string ResultArray2[] = { "ten-abcdef-10", "twenty-abcdefghijklm-20" , "eleven-abcd-11" };

std::vector<std::string> ResultVector2(ResultArray2, ResultArray2 + sizeof(ResultArray2) / sizeof(string));

vector<SourceVerificationType> testCases_ExampleVerificationParameter
{
   {
		3,
		"ten-abcdef\r\nfive-\r\ntwenty-abcdefghijklm\r\neleven-abcd\r\n",
		ResultVector1
   },
   {
		7,
		"ten-abcdef\r\nfive-\r\ntwenty-abcdefghijklm\r\neleven-abcd\r\n",
		ResultVector2
	},
    {
		30,
		"ten-abcdef\r\nfive-\r\ntwenty-abcdefghijklm\r\neleven-abcd\r\n",
		vector<string>()
	}
};

class TestConstructorDI_VerifyNumber : public TestWithDITestFixture, public ::testing::WithParamInterface<SourceVerificationType> {};
TEST_P(TestConstructorDI_VerifyNumber, TestWithConstructorVerifyLineSeparation)
{
	sourceVerification = GetParam();

	StringBuffer sb = readFileLineSp->ReadFileAndResturnCertianLength(m_Path, sourceVerification.size);

	UINT resultCnt = sb.size();
	UINT expectedCnt = sourceVerification.verification.size();
	
	ASSERT_EQ(resultCnt, expectedCnt);

	for (UINT cnt = 0; cnt < expectedCnt ; cnt++)
	{
		const string& expected = sourceVerification.verification[0];
		const string& result = sb[0];
		ASSERT_EQ(result, result);
	}
}

INSTANTIATE_TEST_CASE_P(TestCase, TestConstructorDI_VerifyNumber,
	::testing::ValuesIn(testCases_ExampleVerificationParameter));
