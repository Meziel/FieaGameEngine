#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace LibraryDesktopTests
{

	TEST_CLASS(FooTests)
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
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITests)
		{
			Foo a;
			RTTI* rtti = &a;
			Assert::IsTrue(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Foo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
		}

		TEST_METHOD(Constructor)
		{
			const Foo a;
			Assert::AreEqual(0, a.Data());

			const Foo b(10);
			Assert::AreEqual(10, b.Data());
		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a(10);
			Assert::AreEqual(10, a.Data());

			const Foo b(a);
			Assert::AreEqual(a, b);

			Foo c;
			c = a;
			Assert::AreEqual(c, a);
		}

		TEST_METHOD(EqualityOperators)
		{
			const Foo a(10);
			const Foo b(a);
			Assert::AreEqual(a, b);

			const Foo c;
			Assert::IsTrue(c != a); // Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(SetData)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			Foo a(data);
			Assert::AreEqual(data, a.Data());

			Foo b(std::move(a));
			Assert::AreEqual(data, b.Data());

			Foo c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}