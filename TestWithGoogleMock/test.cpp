#include "pch.h"
#include <Windows.h>
#include <vector>
#include <string.h>  
#include <iterator>
#include "ReadFileLine.h"

typedef struct SourceVerification
{
	int size;
	string source;
	vector<string> verification;
}SourceVerificationType;

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


class MockFs : public virtual  IWindowsFileSystem
{
public:
	MOCK_METHOD( BOOL, CloseHandle, (HANDLE hFile), (override, Calltype(__stdcall)));

	MOCK_METHOD( HANDLE, CreateFile , (
		LPCSTR                    lpFileName,
		DWORD                 dwDesiredAccess,
		DWORD                 dwShareMode,
		LPSECURITY_ATTRIBUTES	  lpSecurityAttributes,
		DWORD                 dwCreationDisposition,
		DWORD                 dwFlagsAndAttributes,
		HANDLE					  hTemplateFile),
		(override, Calltype(__stdcall)));

	MOCK_METHOD(DWORD, GetFileSize, (HANDLE hFile, LPDWORD lpFileSizeHigh), (override, Calltype(__stdcall)));

	MOCK_METHOD(BOOL, ReadFile, (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,  LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped), (override, Calltype(__stdcall)));
};


class TestWithDITestFixture : public ::testing::Test
{
	
protected:
	const HANDLE m_hMockProcess = reinterpret_cast<HANDLE>(0x44444444);
	const string m_Path = "d:\testing\abc.txt";
public:
	shared_ptr<ReadFileLine> readFileLineSp;
	ProcessInput processInput;
	SelectLine selectLine;
	NiceMock<MockFs>* mockFs = nullptr;
	
	SourceVerificationType sourceVerification;
	bool m_FileOpened = false;

	TestWithDITestFixture() {}

	void SetUp() override
	{
		mockFs = new NiceMock<MockFs>;
		readFileLineSp = std::make_shared< ReadFileLine>(processInput, selectLine, *mockFs);
	}

	void TearDown() override
	{
		readFileLineSp.reset();
	}
};

ACTION_TEMPLATE(SetArgNPointeeTo, HAS_1_TEMPLATE_PARAMS(unsigned, uIndex), AND_2_VALUE_PARAMS(pData, uiDataSize))
{
	std::memcpy(std::get<uIndex>(args), pData, uiDataSize);
}

class TestGmock_VerifyNumber : public TestWithDITestFixture, public ::testing::WithParamInterface<SourceVerificationType> {};
TEST_P(TestGmock_VerifyNumber, TestWithGMockVerifyLIneSeparation)
{
	SourceVerification sourceVerification = GetParam();

	char expected[1024] = { 0 };

	strncpy_s(expected,  (const char*)sourceVerification.source.c_str(), sourceVerification.source.length());

	EXPECT_CALL(*mockFs, CreateFile(testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_)).WillOnce(Return(m_hMockProcess));

	EXPECT_CALL(*mockFs, GetFileSize(testing::_, testing::_)).WillOnce(Return(sourceVerification.source.length()));

	EXPECT_CALL(*mockFs, ReadFile(testing::_, testing::_, testing::_, testing::_, testing::_)).WillOnce(
		DoAll(SetArgNPointeeTo<1>(std::begin(expected), sourceVerification.source.length()),
			SetArgPointee<3>(sourceVerification.source.length()),
			Return(TRUE)));

	EXPECT_CALL(*mockFs, CloseHandle(testing::_)).WillOnce(Return(TRUE));

	StringBuffer sb = readFileLineSp->ReadFileAndReturnCertianLength(m_Path, sourceVerification.size);

	UINT resultCnt = sb.size();
	UINT expectedCnt = sourceVerification.verification.size();

	ASSERT_EQ(resultCnt, expectedCnt);

	for (UINT cnt = 0; cnt < expectedCnt; cnt++)
	{
		const string& expected = sourceVerification.verification[0];
		const string& result = sb[0];
		ASSERT_EQ(result, result);
	}
}

INSTANTIATE_TEST_CASE_P(TestCase, TestGmock_VerifyNumber,
	::testing::ValuesIn(testCases_ExampleVerificationParameter));
