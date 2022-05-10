#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\cherk\source\repos\Shannon-Fano\Shannon-Fano\Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShannonFanoTests
{
	TEST_CLASS(ShannonFanoTests)
	{
	public:
		
		TEST_METHOD(TestCountProb)
		{
			string input = "abcccccaddd";
			int size = countProb(input);
			Assert::IsTrue(size == 4);
			Assert::IsTrue(elemArray[0].key == 'c' && elemArray[1].key == 'd' && elemArray[2].key == 'a' && elemArray[3].key == 'b');
		}

		TEST_METHOD(TestMakeCodeTable)
		{
			string input = "abcccccaddd";
			int size = countProb(input);
			makeCodeTable(size);
			Assert::IsTrue(codes['c'] == "0" && codes['a'] == "110" && codes['b'] == "111" && codes['d'] == "10");
			Assert::IsTrue(decodes["0"]=='c' && decodes["110"] == 'a' && decodes["111"] == 'b' && decodes["10"] == 'd');
		}

		TEST_METHOD(TestCode)
		{
			string input = "abcccccaddd";
			int size = countProb(input);
			makeCodeTable(size);
			Assert::IsTrue(code(input) == "11011100000110101010");
		}

		TEST_METHOD(TestDecode)
		{
			string input = "abcccccaddd";
			int size = countProb(input);
			makeCodeTable(size);
			Assert::IsTrue(decode(code(input)) == input);
		}
	};
}
