#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "HashSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(HashTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState,
				&endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(GenericTest)
		{
			const int32_t a = 10;
			const int32_t b = 20;
			const int32_t c = 10;
			DefaultHash<int32_t> defaultHash;
			const size_t hashValueA = defaultHash(a);
			const size_t hashValueB = defaultHash(b);
			const size_t hashValueC = defaultHash(c);
			Assert::AreEqual(hashValueA, hashValueC);
			Assert::AreNotEqual(hashValueA, hashValueB);
			Assert::AreNotEqual(hashValueB, hashValueC);
		}

		TEST_METHOD(CharArrayTest)
		{
			const char* a = "AString";
			const char* b = "AnotherString";
			char c[8];
			memcpy_s(c, sizeof(c), a, sizeof(a));
			DefaultHash<const char*> defaultHash;
			const size_t hashValueA = defaultHash(a);
			const size_t hashValueB = defaultHash(b);
			const size_t hashValueC = defaultHash(c);
			Assert::AreEqual(hashValueA, hashValueC);
			Assert::AreNotEqual(hashValueA, hashValueB);
			Assert::AreNotEqual(hashValueB, hashValueC);
		}

		TEST_METHOD(StringTest)
		{
			const string a = "AString";
			const string b = "AnotherString";
			const string c = "AString";
			DefaultHash<const string> defaultHash;
			const size_t hashValueA = defaultHash(a);
			const size_t hashValueB = defaultHash(b);
			const size_t hashValueC = defaultHash(c);
			Assert::AreEqual(hashValueA, hashValueC);
			Assert::AreNotEqual(hashValueA, hashValueB);
			Assert::AreNotEqual(hashValueB, hashValueC);
		}

		TEST_METHOD(FooTest)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(10);
			DefaultHash<Foo> defaultHash;
			const size_t hashValueA = defaultHash(a);
			const size_t hashValueB = defaultHash(b);
			const size_t hashValueC = defaultHash(c);
			Assert::AreEqual(hashValueA, hashValueC);
			Assert::AreNotEqual(hashValueA, hashValueB);
			Assert::AreNotEqual(hashValueB, hashValueC);
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}