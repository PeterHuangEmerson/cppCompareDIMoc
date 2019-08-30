#include "pch.h"
#include <Windows.h>
#include <vector>
#include <string.h>  

using namespace std;



typedef struct SourceVerification
{
	int size;
	string source;
	vector<string> verification;
}SourceVerificationType;


class ObjectToTest
{
public:
	virtual void Testfunct(int value, char* returnVal)
	{
		string val = to_string(value);
		strcpy_s((char*)returnVal, val.length() + 1, val.c_str());
	}
};

class MockObjectToTest : public   ObjectToTest
{
public:
	MOCK_METHOD(void, Testfunct, (int value, char* returnVal), (override));

};

ACTION_TEMPLATE(SetArgNPointeeTo, HAS_1_TEMPLATE_PARAMS(unsigned, uIndex), AND_2_VALUE_PARAMS(pData, uiDataSize))
{
	std::memcpy(std::get<uIndex>(args), pData, uiDataSize);
}


class ObjectHolder
{
	friend class MockObjectHolderFixure;
public:
	ObjectToTest* objectToTest;
	void Create()
	{
		objectToTest = new ObjectToTest();
	}
	void RunTestFunct(int value, char* returnVal)
	{
		objectToTest->Testfunct(value, returnVal);
	}
};

class MockObjectHolderFixure : public ::testing::Test
{
public:
	ObjectHolder objectHolder;
	NiceMock<MockObjectToTest>* pMockObjectToTest = nullptr;
	void RunTest(int value, char* returnVal)
	{
		objectHolder.RunTestFunct(value, returnVal);
	}
	void SetUp() override
	{
		pMockObjectToTest = new NiceMock<MockObjectToTest>;
		objectHolder.objectToTest = pMockObjectToTest;

	}
};


class TestConstructorDI_MockObject : public MockObjectHolderFixure, public ::testing::WithParamInterface<SourceVerificationType> {};
TEST_P(TestConstructorDI_MockObject, TestWithGMockVerifyLIneSeparation)
{
	SourceVerification sourceVerification = GetParam();

	int value = 52;
	char  expected[] = "52";
	size_t bufSize = sizeof("52");
	char* resultValue = (char*)new char[3];

	EXPECT_CALL(*pMockObjectToTest, Testfunct(52, testing::_)).WillOnce(SetArgNPointeeTo<1>(std::begin(expected), bufSize));

	RunTest(value, resultValue);

	string result = resultValue;
	string expect = expected;

	ASSERT_EQ(expect, result);

	//StringBuffer sb = readFileLineSp->ReadFileAndResturnCertianLength(m_Path, sourceVerification.size);

}

INSTANTIATE_TEST_CASE_P(TestCase, TestConstructorDI_MockObject,
	::testing::ValuesIn(testCases_ExampleVerificationParameter));

//class TestConstructorDI_VerifyNumber2 : public MockObjectHolderFixure,
//	TEST_F(TestConstructorDI_VerifyNumber2, TestWithConstructorVerifyLineSeparation)
//{
//	int value = 52;
//	const char  expected[] = "52";
//	size_t bufSize = sizeof("52");
//	char* resultValue = (char*)new char[3];
//
//	EXPECT_CALL(*m_pMockObjectToTest, Testfunct(52, testing::_)).WillOnce(SetArgNPointeeTo<1>(std::begin(expected), bufSize));
//
//	RunTest(value, resultValue);
//
//	string result = resultValue;
//	string expect = expected;
//
//	ASSERT_EQ(expect, result);
//
//	//StringBuffer sb = readFileLineSp->ReadFileAndResturnCertianLength(m_Path, sourceVerification.size);
//
//}
