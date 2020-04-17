#include "pch.h"
#include "CppUnitTest.h"
#include "../parser/parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			try {
				const char* inp = "..\\inps\\inp0.txt";
				Storage s(inp);
				Assert::IsFalse(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Test2)
		{
			try {
				const char* inp = "..\\inps\\inp1.txt";
				Storage s(inp);
				Assert::IsTrue(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Test3)
		{
			try {
				const char* inp = "..\\inps\\inp2.txt";
				Storage s(inp);
				Assert::IsFalse(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Test4)
		{
			try {
				const char* inp = "..\\inps\\inp3.txt";
				Storage s(inp);
				Assert::IsFalse(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Test5)
		{
			try {
				const char* inp = "..\\inps\\inp4.txt";
				Storage s(inp);
				Assert::IsFalse(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Test6)
		{
			try {
				const char* inp = "..\\inps\\inp5.txt";
				Storage s(inp);
				Assert::IsFalse(s.parse());
			}
			catch (const std::exception& err) {
				Assert::IsTrue(false);
			}
		}
	};
}
