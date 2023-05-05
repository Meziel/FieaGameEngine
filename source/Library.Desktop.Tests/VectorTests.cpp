#include "pch.h"
#include "Literals.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Vector.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Vector<Foo>::iterator>(const Vector<Foo>::iterator& t)
	{
		wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::const_iterator>(const Vector<Foo>::const_iterator& t)
	{
		wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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

		TEST_METHOD(Constructor)
		{
			// Default constructor
			{
				Vector<Foo> vector;
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(0), vector.Capacity());
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& front = vector.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });
				Assert::AreEqual(true, vector.IsEmpty());
			}

			// default capacity
			{
				// default capacity = 1
				Vector<Foo> vector(1);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(1), vector.Capacity());

				// default capacity = 5
				Vector<Foo> vector2(5);
				Assert::AreEqual(size_t(0), vector2.Size());
				Assert::AreEqual(size_t(5), vector2.Capacity());
			}
			
			// Initializer list
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector = { a, b };
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(size_t(2), vector.Capacity());
				Assert::AreEqual(a, vector[0]);
				Assert::AreEqual(b, vector[1]);
				Assert::AreNotSame(a, vector[0]);
				Assert::AreNotSame(a, vector[1]);
			}
		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a(1);
			const Foo b(2);
			Vector<Foo> vector1 = { a, b };
			Vector<Foo> vector2 = vector1;

			Assert::IsTrue(std::equal(vector1.begin(), vector1.end(), vector2.begin()));
			Assert::AreEqual(vector2.Size(), vector1.Size());
		}
		
		TEST_METHOD(MoveSemantics)
		{
			const Foo a(1);
			const Foo b(2);
			Vector<Foo> tmp = { a, b };
			Vector<Foo> vector(std::move(tmp));
			// Check vector
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
			Assert::AreNotSame(a, vector[0]);
			Assert::AreNotSame(a, vector[1]);
			// Check tmp was cleared
#pragma warning(push)
#pragma warning(disable:26800)
			Assert::AreEqual(size_t(0), tmp.Size());
			Assert::AreEqual(size_t(0), tmp.Capacity());
			Assert::AreEqual(true, tmp.IsEmpty());
#pragma warning(pop)
		}

		TEST_METHOD(Assignment)
		{
			// Copy assignment
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector1 = { a, b };
				Vector<Foo> vector2;
				vector2 = vector1;
				Assert::IsTrue(std::equal(vector1.begin(), vector1.end(), vector2.begin()));
				Assert::AreEqual(vector2.Capacity(), vector1.Capacity());
				Assert::AreEqual(vector2.Size(), vector1.Size());
			}

			// Move assignment
			{
				const Foo a(1);
				const Foo b(2);
				const Foo c(3);
				Vector<Foo> tmp = { a, b };
				Vector<Foo> vector = { c };
				vector = std::move(tmp);
				// Check vector
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(size_t(2), vector.Capacity());
				Assert::AreEqual(a, vector[0]);
				Assert::AreEqual(b, vector[1]);
				Assert::AreNotSame(a, vector[0]);
				Assert::AreNotSame(a, vector[1]);
				// Check tmp was cleared
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), tmp.Size());
				Assert::AreEqual(size_t(0), tmp.Capacity());
				Assert::AreEqual(true, tmp.IsEmpty());
#pragma warning(pop)
			}
		}

		TEST_METHOD(Resize)
		{
			Foo a(1);
			Foo b(2);
			Vector<Foo> vector = {a};
			Assert::AreEqual(size_t(1), vector.Size());
			Assert::AreEqual(size_t(1), vector.Capacity());

			vector.Resize(3);
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(0, vector[1].Data());
			Assert::AreEqual(0, vector[2].Data());

			vector.Resize(5, b);
			Assert::AreEqual(size_t(5), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(0, vector[1].Data());
			Assert::AreEqual(0, vector[2].Data());
			Assert::AreEqual(2, vector[3].Data());
			Assert::AreEqual(2, vector[4].Data());

			vector.Resize(5, b);
			Assert::AreEqual(size_t(5), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());

			vector.Resize(5);
			Assert::AreEqual(size_t(5), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());

			vector.Resize(4);
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());

			vector.Resize(3, b);
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());
		}

		TEST_METHOD(At)
		{
			const Foo a(1);
			Vector<Foo> vector;
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = vector.At(0); UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = vector[0]; UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = const_cast<const Vector<Foo>&>(vector).At(1); UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = const_cast<const Vector<Foo>&>(vector)[1]; UNREFERENCED_LOCAL(foo); });
			vector.PushBack(a);
			Assert::AreEqual(a, vector.At(0));
			Assert::AreNotSame(a, vector.At(0));
			Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).At(0));
			Assert::AreNotSame(a, const_cast<const Vector<Foo>&>(vector).At(0));
			Assert::AreEqual(a, vector[0]);
			Assert::AreNotSame(a, vector[0]);
			Assert::AreEqual(a, const_cast<Vector<Foo>&>(vector)[0]);
			Assert::AreNotSame(a, const_cast<Vector<Foo>&>(vector)[0]);
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = vector.At(1); UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = vector[1]; UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = const_cast<const Vector<Foo>&>(vector).At(1); UNREFERENCED_LOCAL(foo); });
			Assert::ExpectException<std::runtime_error>([&vector]() { const auto& foo = const_cast<const Vector<Foo>&>(vector)[1]; UNREFERENCED_LOCAL(foo); });
		}

		TEST_METHOD(PopBack)
		{
			const Foo a(1);
			const Foo b(2);
			Vector<Foo> vector = { a, b };
			Assert::AreEqual(b, vector.Back());
			vector.PopBack();
			Assert::AreEqual(a, vector.Back());
			vector.PopBack();
			Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });
			Assert::ExpectException<std::runtime_error>([&vector]() { vector.PopBack(); });
		}

		TEST_METHOD(IsEmpty)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());
			vector.PushBack(Foo(1));
			Assert::IsFalse(vector.IsEmpty());
			vector.PushBack(Foo(2));
			vector.PopBack();
			Assert::IsFalse(vector.IsEmpty());
			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(FrontBack)
		{
			{
				const Foo a(1);
				const Foo b(1);

				Vector<Foo> vector;
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& front = vector.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });
				Assert::ExpectException<std::runtime_error>([&vector]() { const auto& front = const_cast<const Vector<Foo>&>(vector).Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { const auto& back = const_cast<const Vector<Foo>&>(vector).Back(); UNREFERENCED_LOCAL(back); });

				vector.PushBack(a);
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(a, vector.Back());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Front());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Back());

				vector.PushBack(b);
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Front());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Back());

				vector.Remove(vector.begin());
				Assert::AreEqual(b, vector.Front());
				Assert::AreEqual(b, vector.Back());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Front());
				Assert::AreEqual(a, const_cast<const Vector<Foo>&>(vector).Back());
			}
		}
		
		TEST_METHOD(Size)
		{
			Vector<Foo> vector = {Foo(1)};
			Assert::AreEqual(size_t(1), vector.Size());
			vector.Remove(vector.begin());
			Assert::AreEqual(size_t(0), vector.Size());
		}

		TEST_METHOD(Capacity)
		{
			Vector<Foo> vector(0, [](size_t capacity) { return capacity + 1; });
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.PushBack(Foo(10));
			Assert::AreEqual(size_t(1), vector.Capacity());
			vector.PushBack(Foo(20));
			Assert::AreEqual(size_t(2), vector.Capacity());
			vector.Reserve(10);
			Assert::AreEqual(size_t(10), vector.Capacity());
		}

		TEST_METHOD(Reserve)
		{
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(0);
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(10);
			Assert::AreEqual(size_t(10), vector.Capacity());
			vector.Reserve(20);
			Assert::AreEqual(size_t(20), vector.Capacity());
			vector.Reserve(20);
			Assert::AreEqual(size_t(20), vector.Capacity());
			vector.Reserve(10);
			Assert::AreEqual(size_t(20), vector.Capacity());
		}

		TEST_METHOD(ConstReferencePushBack)
		{
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo>::iterator it1;
				Vector<Foo>::iterator it2;

				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				it1 = vector.PushBack(a);
				Assert::AreEqual(vector.begin(), it1);
				Assert::AreEqual(vector.end() - 1, it1);
				Assert::AreEqual(a, vector.Front());
				Assert::AreNotSame(a, vector.Front());
				Assert::AreEqual(a, vector.Back());
				Assert::AreNotSame(a, vector.Back());
				Assert::AreEqual(a, vector[0]);
				Assert::AreNotSame(a, vector[0]);
				Assert::AreEqual(size_t(1), vector.Capacity());

				it2 = vector.PushBack(b);
				Assert::AreNotEqual(vector.begin(), it2);
				Assert::AreEqual(vector.end() - 1, it2);
				Assert::AreEqual(a, vector.Front());
				Assert::AreNotSame(a, vector.Front());
				Assert::AreEqual(b, vector.Back());
				Assert::AreNotSame(b, vector.Back());
				Assert::AreEqual(a, vector[0]);
				Assert::AreNotSame(a, vector[0]);
				Assert::AreEqual(b, vector[1]);
				Assert::AreNotSame(b, vector[1]);
				Assert::AreEqual(size_t(2), vector.Capacity());
			}

			// Reserve strategy + 2
			{
				const Foo a(1);
				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 2; });
				Assert::AreEqual(size_t(0), vector.Capacity());
				vector.PushBack(a);
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.PushBack(a);
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.PushBack(a);
				Assert::AreEqual(size_t(4), vector.Capacity());
			}

			// Default reserve strategy
			{
				const Foo a(1);
				Vector<Foo> vector;
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.PushBack(a);
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.PushBack(a);
				Assert::AreEqual(size_t(8), vector.Capacity());
			}

			// Bad strategy
			{
				const Foo a(1);
				Vector<Foo> vector(0, [](size_t capacity) { return capacity == 0 ? capacity : capacity - 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.PushBack(a);
				Assert::AreEqual(size_t(3), vector.Capacity());
			}
		}

		TEST_METHOD(RValueReferencePushBack)
		{
			{
				Vector<Foo>::iterator it1;
				Vector<Foo>::iterator it2;

				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				it1 = vector.PushBack(Foo(1));
				Assert::AreEqual(vector.begin(), it1);
				Assert::AreEqual(vector.end() - 1, it1);
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreNotSame(Foo(1), vector.Front());
				Assert::AreEqual(Foo(1), vector.Back());
				Assert::AreNotSame(Foo(1), vector.Back());
				Assert::AreEqual(Foo(1), vector[0]);
				Assert::AreNotSame(Foo(1), vector[0]);
				Assert::AreEqual(size_t(1), vector.Capacity());

				it2 = vector.PushBack(Foo(2));
				Assert::AreNotEqual(vector.begin(), it2);
				Assert::AreEqual(vector.end() - 1, it2);
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreNotSame(Foo(1), vector.Front());
				Assert::AreEqual(Foo(2), vector.Back());
				Assert::AreNotSame(Foo(2), vector.Back());
				Assert::AreEqual(Foo(1), vector[0]);
				Assert::AreNotSame(Foo(1), vector[0]);
				Assert::AreEqual(Foo(2), vector[1]);
				Assert::AreNotSame(Foo(2), vector[1]);
				Assert::AreEqual(size_t(2), vector.Capacity());
			}

			// Reserve strategy + 2
			{
				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 2; });
				Assert::AreEqual(size_t(0), vector.Capacity());
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(4), vector.Capacity());
			}

			// Default reserve strategy
			{
				Vector<Foo> vector;
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(8), vector.Capacity());
			}

			// Bad strategy
			{
				Vector<Foo> vector(0, [](size_t capacity) { return capacity == 0 ? capacity : capacity - 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(3), vector.Capacity());
			}
		}

		TEST_METHOD(EmplaceBack)
		{
			{
				Vector<Foo>::iterator it1;
				Vector<Foo>::iterator it2;

				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				it1 = vector.EmplaceBack(1);
				Assert::AreEqual(vector.begin(), it1);
				Assert::AreEqual(vector.end() - 1, it1);
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreNotSame(Foo(1), vector.Front());
				Assert::AreEqual(Foo(1), vector.Back());
				Assert::AreNotSame(Foo(1), vector.Back());
				Assert::AreEqual(Foo(1), vector[0]);
				Assert::AreNotSame(Foo(1), vector[0]);
				Assert::AreEqual(size_t(1), vector.Capacity());

				it2 = vector.EmplaceBack(2);
				Assert::AreNotEqual(vector.begin(), it2);
				Assert::AreEqual(vector.end() - 1, it2);
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreNotSame(Foo(1), vector.Front());
				Assert::AreEqual(Foo(2), vector.Back());
				Assert::AreNotSame(Foo(2), vector.Back());
				Assert::AreEqual(Foo(1), vector[0]);
				Assert::AreNotSame(Foo(1), vector[0]);
				Assert::AreEqual(Foo(2), vector[1]);
				Assert::AreNotSame(Foo(2), vector[1]);
				Assert::AreEqual(size_t(2), vector.Capacity());
			}

			// Reserve strategy + 2
			{
				Vector<Foo> vector(0, [](size_t capacity) { return capacity + 2; });
				Assert::AreEqual(size_t(0), vector.Capacity());
				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(4), vector.Capacity());
			}

			// Default reserve strategy
			{
				Vector<Foo> vector;
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(4), vector.Capacity());
				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(8), vector.Capacity());
			}

			// Bad strategy
			{
				Vector<Foo> vector(0, [](size_t capacity) { return capacity == 0 ? capacity : capacity - 1; });
				Assert::AreEqual(size_t(0), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(1), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(2), vector.Capacity());

				vector.EmplaceBack(1);
				Assert::AreEqual(size_t(3), vector.Capacity());
			}
		}

		TEST_METHOD(Find)
		{
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			
			// non-const
			{
				Vector<Foo>::iterator it;
				Vector<Foo> vector = { a, b };
				it = vector.Find(a);
				Assert::AreEqual(vector.begin(), it);
				it = vector.Find(b);
				Assert::AreEqual(vector.end() - 1, it);
				it = vector.Find(c);
				Assert::AreEqual(vector.end(), it);
			}

			// const
			{
				Vector<Foo>::const_iterator it;
				const Vector<Foo> vector = { a, b };
				it = vector.Find(a);
				Assert::AreEqual(vector.begin(), it);
				it = vector.Find(b);
				Assert::AreEqual(vector.end() - 1, it);
				it = vector.Find(c);
				Assert::AreEqual(vector.end(), it);
			}
		}

		TEST_METHOD(Clear)
		{
			const Foo a(1);
			const Foo b(2);
			Vector<Foo> vector = { a, b };
			Assert::AreEqual(size_t(2), vector.Size());
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.Size());
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.Size());
		}

		TEST_METHOD(ShrinkToFit)
		{
			const Foo a(1);
			const Foo b(2);
			Vector<Foo> vector;
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(10);
			vector.PushBack(a);
			vector.PushBack(b);
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
		}

		TEST_METHOD(Remove)
		{
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			bool ret;

			// Remove value
			{
				Vector<Foo>::const_iterator it;
				Vector<Foo> vector = { a, b };
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());

				ret = vector.Remove(c);
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());

				ret = vector.Remove(b);
				Assert::IsTrue(ret);
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(a, vector.Back());

				ret = vector.Remove(a);
				Assert::IsTrue(ret);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& front = vector.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });
			}

			// Remove iterator
			{
				Vector<Foo>::const_iterator it;
				Vector<Foo> vector = { a, b };
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());

				ret = vector.Remove(vector.end());
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());

				ret = vector.Remove(vector.end() - 1);
				Assert::IsTrue(ret);
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(a, vector.Back());

				ret = vector.Remove(vector.begin());
				Assert::IsTrue(ret);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& front = vector.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });

				ret = vector.Remove(vector.begin());
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& front = vector.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&vector]() { auto& back = vector.Back(); UNREFERENCED_LOCAL(back); });
			}

			// Remove with 2 iterators
			{
				Vector<Foo>::const_iterator it;
				Vector<Foo> vector = { a, b, c };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(c, vector.Back());

				ret = vector.Remove(vector.begin() + 1, vector.end() + 1);
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(a, vector.Back());

				ret = vector.Remove(vector.begin() - 1, vector.end());
				Assert::AreEqual(size_t(0), vector.Size());

				vector = { a, b, c };

				// Remove with bad first iterator
				ret = vector.Remove(Vector<Foo>::iterator{}, vector.end());
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(3), vector.Size());

				// Remove with bad second iterator
				ret = vector.Remove(vector.begin(), Vector<Foo>::iterator{});
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(3), vector.Size());

				// Remove with first >= last
				ret = vector.Remove(vector.begin(), vector.begin());
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(3), vector.Size());

				// Out of range
				ret = vector.Remove(vector.begin()-2, vector.begin()-1);
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(3), vector.Size());

				// Out of range
				ret = vector.Remove(vector.end() + 2, vector.end() + 3);
				Assert::IsFalse(ret);
				Assert::AreEqual(size_t(3), vector.Size());
			}
		}

		TEST_METHOD(begin)
		{
			const Foo a(1);
			const Foo b(2);
			{
				Vector<Foo> vector = { a, b };
				Vector<Foo>::iterator it;
				Vector<Foo>::const_iterator constIt;

				// Get first element
				it = vector.begin();
				Assert::AreEqual(a, *it);
				Assert::AreNotSame(a, *it);

				constIt = const_cast<const Vector<Foo>&>(vector).begin();
				Assert::AreEqual(a, *constIt);
				Assert::AreNotSame(a, *constIt);

				constIt = vector.cbegin();
				Assert::AreEqual(a, *constIt);
				Assert::AreNotSame(a, *constIt);

				// Change first element
				vector[0] = b;

				it = vector.begin();
				Assert::AreEqual(b, *it);
				Assert::AreNotSame(b, *it);

				constIt = const_cast<const Vector<Foo>&>(vector).begin();
				Assert::AreEqual(b, *constIt);
				Assert::AreNotSame(b, *constIt);

				constIt = vector.cbegin();
				Assert::AreEqual(b, *constIt);
				Assert::AreNotSame(b, *constIt);

				// Clear list.
				vector.Clear();

				it = vector.begin();
				Assert::ExpectException<runtime_error>([&it]() { auto& value = *it; UNREFERENCED_LOCAL(value); });

				constIt = const_cast<const Vector<Foo>&>(vector).begin();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });

				constIt = vector.cbegin();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });

				// Shrink to fit.
				vector.ShrinkToFit();

				it = vector.begin();
				Assert::ExpectException<runtime_error>([&it]() { auto& value = *it; UNREFERENCED_LOCAL(value); });

				constIt = const_cast<const Vector<Foo>&>(vector).begin();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });

				constIt = vector.cbegin();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });
			}
		}

		TEST_METHOD(end)
		{
			const Foo a(1);
			const Foo b(2);
			{
				Vector<Foo> vector = { a, b };
				Vector<Foo>::iterator it;
				Vector<Foo>::const_iterator constIt;

				// Dereference end
				it = vector.end();
				Assert::ExpectException<runtime_error>([&it]() { auto& value = *it; UNREFERENCED_LOCAL(value); });

				constIt = const_cast<const Vector<Foo>&>(vector).end();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });

				constIt = vector.cend();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *constIt; UNREFERENCED_LOCAL(value); });

				// Get last element
				it = vector.end();
				Assert::AreEqual(b, *(it - 1));
				Assert::AreNotSame(b, *(it - 1));

				constIt = const_cast<const Vector<Foo>&>(vector).end();
				Assert::AreEqual(b, *(constIt - 1));
				Assert::AreNotSame(b, *(constIt - 1));

				constIt = vector.cend();
				Assert::AreEqual(b, *(constIt - 1));
				Assert::AreNotSame(b, *(constIt - 1));

				// Change last element
				vector[1] = a;

				it = vector.end();
				Assert::AreEqual(a, *(it - 1));
				Assert::AreNotSame(a, *(it - 1));

				constIt = const_cast<const Vector<Foo>&>(vector).end();
				Assert::AreEqual(a, *(constIt - 1));
				Assert::AreNotSame(a, *(constIt - 1));

				constIt = vector.cend();
				Assert::AreEqual(a, *(constIt - 1));
				Assert::AreNotSame(a, *(constIt - 1));

				// Clear list.
				vector.Clear();

				it = vector.end();
				Assert::ExpectException<runtime_error>([&it]() { auto& value = *(it - 1); UNREFERENCED_LOCAL(value); });

				constIt = const_cast<const Vector<Foo>&>(vector).end();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *(constIt - 1); UNREFERENCED_LOCAL(value); });

				constIt = vector.cend();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *(constIt - 1); UNREFERENCED_LOCAL(value); });

				// Shrink to fit.
				vector.ShrinkToFit();

				it = vector.end();
				Assert::ExpectException<runtime_error>([&it]() { auto& value = *(it - 1); UNREFERENCED_LOCAL(value); });

				constIt = const_cast<const Vector<Foo>&>(vector).end();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *(constIt - 1); UNREFERENCED_LOCAL(value); });

				constIt = vector.cend();
				Assert::ExpectException<runtime_error>([&constIt]() { auto& value = *(constIt - 1); UNREFERENCED_LOCAL(value); });
			}
		}

		TEST_METHOD(const_iterator)
		{
			// operator*
			{
				// Default constructed iterator
				Vector<Foo>::const_iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				// const_iterator from vector
				const Foo a(1);
				Vector<Foo> vector = { a };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				it = vector.begin();
				Assert::AreEqual(a, *it);
			}

			// operator++
			{
				// Default constructed const_iterator
				Vector<Foo>::const_iterator it;
				Vector<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it++; });

				// const_iterator from vector
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector = { a, b };

				// First element
				it = vector.begin();
				Assert::AreEqual(a, *it);

				previousIt = it++;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::AreEqual(a, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it);
				Assert::AreEqual(vector.end(), previousIt);
			}

			// ++operator
			{
				// Default constructed const_iterator
				Vector<Foo>::const_iterator it;
				Vector<Foo>::const_iterator it2;
				Vector<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { ++it; });

				// const_iterator from vector
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector = { a, b };

				// First element
				it = vector.begin();
				Assert::AreEqual(a, *it);

				it2 = ++it;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it2);
			}

			// operator--
			{
				// Default constructed const_iterator
				Vector<Foo>::const_iterator it;
				Vector<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { --it; });

				// const_iterator from vector
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector = { a, b };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				previousIt = --it;

				// last element
				Assert::AreEqual(vector.end() - 1, it);
				Assert::AreEqual(vector.end() - 1, previousIt);
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *previousIt);

				previousIt = --it;

				// first element
				Assert::AreEqual(vector.begin(), it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::AreEqual(a, *it);
				Assert::AreEqual(a, *previousIt);

				previousIt = --it;

				// begin - 1
				Assert::AreEqual(vector.begin() - 1, it);
				Assert::AreEqual(vector.begin() - 1, previousIt);
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });

				previousIt = --it;

				// begin - 2
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.begin() - 2, it);
				Assert::AreEqual(vector.begin() - 2, previousIt);
			}

			// --operator
			{
				// Default constructed const_iterator
				Vector<Foo>::const_iterator it;
				Vector<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it--; });

				// const_iterator from vector
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector = { a, b };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				previousIt = it--;

				// last element
				Assert::AreEqual(vector.end() - 1, it);
				Assert::AreEqual(vector.end(), previousIt);
				Assert::AreEqual(b, *it);
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });

				previousIt = it--;

				// first element
				Assert::AreEqual(vector.begin(), it);
				Assert::AreEqual(vector.begin() + 1, previousIt);
				Assert::AreEqual(a, *it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it--;

				// begin - 1
				Assert::AreEqual(vector.begin() - 1, it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(a, *previousIt);

				previousIt = it--;

				// begin - 2
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.begin() - 2, it);
				Assert::AreEqual(vector.begin() - 1, previousIt);
			}

			// operator== & !=
			{
				// Compare default constructed const_iterators
				Vector<Foo>::const_iterator it;
				Vector<Foo>::const_iterator it2;
				Assert::AreEqual(it, it2);
				Assert::IsFalse(it != it2);

				// Comparing two vector
				const Vector<Foo> vector1 = { Foo(1), Foo(2) };
				const Vector<Foo> vector2 = { Foo(1), Foo(2) };
				it = vector1.begin();
				it2 = vector2.begin();
				Assert::AreNotEqual(it, it2);
				Assert::IsTrue(it != it2);

				// Comparing elements in same vector
				Assert::AreEqual(vector1.begin(), vector1.begin());
				Assert::AreEqual(vector1.end(), vector1.end());
				Assert::AreNotEqual(vector1.begin(), vector1.end());
				Assert::IsTrue(vector1.begin() != vector1.end());

				Vector<Foo> vector3;
				Assert::AreEqual(vector3.begin(), vector3.end());
				Assert::IsFalse(vector3.begin() != vector3.end());
			}

			// operator+= & +
			{
				// Default constructed
				Vector<Foo>::const_iterator it;
				Assert::ExpectException<runtime_error>([&it]() {  auto temp = it + 1; UNREFERENCED_LOCAL(temp); });
				Assert::ExpectException<runtime_error>([&it]() { it += 1; });

				// Non default constructed
				const Foo a(1);
				const Foo b(1);
				const Vector<Foo> vector = { a, b };
				it = vector.begin();
				Assert::AreEqual(vector.end(), it + 2);
				Assert::AreEqual(vector.end(), it += 2);
			}

			// operator-= & -
			{
				// Default constructed
				Vector<Foo>::const_iterator it;
				Assert::ExpectException<runtime_error>([&it]() { auto temp = it - 1; UNREFERENCED_LOCAL(temp); });
				Assert::ExpectException<runtime_error>([&it]() { it -= 1; });

				// Non default constructed
				const Foo a(1);
				const Foo b(1);
				const Vector<Foo> vector = { a, b };
				it = vector.end();
				Assert::AreEqual(vector.begin(), it - 2);
				Assert::AreEqual(vector.begin(), it -= 2);
			}

			// Subtract two iterators
			{
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector{ a, b };
				const Vector<Foo> vector2{ a, b };
				Vector<Foo>::const_iterator it = vector.begin();
				Vector<Foo>::const_iterator it2 = vector2.begin();

				// subtract two iterators from different containers
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it - it2; UNREFERENCED_LOCAL(b); });

				// subtract two iterators from same container
				Assert::AreEqual(size_t(2), vector.end() - vector.begin());
			}

			// Compare iterators
			{
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector{ a, b };
				const Vector<Foo> vector2{ a, b };
				Vector<Foo>::const_iterator it = vector.begin();
				Vector<Foo>::const_iterator it2 = vector2.begin();

				// compare two iterators from different containers
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it < it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it <= it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it > it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it >= it2; UNREFERENCED_LOCAL(b); });

				// compare two iterators from same container
				Assert::IsTrue(vector.begin() < vector.end());
				Assert::IsFalse(vector.begin() < vector.begin());
				Assert::IsFalse(vector.end() < vector.begin());
				Assert::IsTrue(vector.begin() <= vector.end());
				Assert::IsTrue(vector.begin() <= vector.begin());
				Assert::IsFalse(vector.end() <= vector.begin());

				Assert::IsFalse(vector.begin() > vector.end());
				Assert::IsFalse(vector.begin() > vector.begin());
				Assert::IsTrue(vector.end() > vector.begin());
				Assert::IsFalse(vector.begin() >= vector.end());
				Assert::IsTrue(vector.begin() >= vector.begin());
				Assert::IsTrue(vector.end() >= vector.begin());
			}

			// random access an iterator
			{
				const Foo a(1);
				const Foo b(2);
				const Vector<Foo> vector{ a, b };
				Vector<Foo>::const_iterator it = vector.end();
				Assert::AreEqual(a, it[0]);
				Assert::AreEqual(b, it[1]);

				// default constructed
				Vector<Foo>::const_iterator itDefault;
				Assert::ExpectException<runtime_error>([&itDefault]() { const auto& v = itDefault[0]; UNREFERENCED_LOCAL(v); });
			}
		}

		TEST_METHOD(iterator)
		{
			// operator*
			{
				// Default constructed iterator
				Vector<Foo>::iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				// Iterator from vector
				const Foo a(1);
				Vector<Foo> vector = { a };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				it = vector.begin();
				Assert::AreEqual(a, *it);
			}

			// operator++
			{
				// Default constructed iterator
				Vector<Foo>::iterator it;
				Vector<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it++; });

				// Iterator from vector
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector = { a, b };

				// First element
				it = vector.begin();
				Assert::AreEqual(a, *it);

				previousIt = it++;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::AreEqual(a, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it);
				Assert::AreEqual(vector.end(), previousIt);
			}

			// ++operator
			{
				// Default constructed iterator
				Vector<Foo>::iterator it;
				Vector<Foo>::iterator it2;
				Vector<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { ++it; });

				// Iterator from vector
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector = { a, b };

				// First element
				it = vector.begin();
				Assert::AreEqual(a, *it);

				it2 = ++it;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end(), it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.end() + 1, it2);
			}

			// operator--
			{
				// Default constructed iterator
				Vector<Foo>::iterator it;
				Vector<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { --it; });

				// Iterator from vector
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector = { a, b };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				previousIt = --it;

				// last element
				Assert::AreEqual(vector.end() - 1, it);
				Assert::AreEqual(vector.end() - 1, previousIt);
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *previousIt);

				previousIt = --it;

				// first element
				Assert::AreEqual(vector.begin(), it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::AreEqual(a, *it);
				Assert::AreEqual(a, *previousIt);

				previousIt = --it;

				// begin - 1
				Assert::AreEqual(vector.begin() - 1, it);
				Assert::AreEqual(vector.begin() - 1, previousIt);
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });

				previousIt = --it;

				// begin - 2
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.begin() - 2, it);
				Assert::AreEqual(vector.begin() - 2, previousIt);
			}

			// --operator
			{
				// Default constructed iterator
				Vector<Foo>::iterator it;
				Vector<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it--; });

				// Iterator from vector
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector = { a, b };

				it = vector.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				previousIt = it--;

				// last element
				Assert::AreEqual(vector.end() - 1, it);
				Assert::AreEqual(vector.end(), previousIt);
				Assert::AreEqual(b, *it);
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });

				previousIt = it--;

				// first element
				Assert::AreEqual(vector.begin(), it);
				Assert::AreEqual(vector.begin() + 1, previousIt);
				Assert::AreEqual(a, *it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it--;

				// begin - 1
				Assert::AreEqual(vector.begin() - 1, it);
				Assert::AreEqual(vector.begin(), previousIt);
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(a, *previousIt);

				previousIt = it--;

				// begin - 2
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(vector.begin() - 2, it);
				Assert::AreEqual(vector.begin() - 1, previousIt);
			}

			// operator== & !=
			{
				// Compare default constructed iterators
				Vector<Foo>::iterator it;
				Vector<Foo>::iterator it2;
				Assert::AreEqual(it, it2);
				Assert::IsFalse(it != it2);

				// Comparing two vector
				Vector<Foo> vector1 = { Foo(1), Foo(2) };
				Vector<Foo> vector2 = { Foo(1), Foo(2) };
				it = vector1.begin();
				it2 = vector2.begin();
				Assert::AreNotEqual(it, it2);
				Assert::IsTrue(it != it2);

				// Comparing elements in same vector
				Assert::AreEqual(vector1.begin(), vector1.begin());
				Assert::AreEqual(vector1.end(), vector1.end());
				Assert::AreNotEqual(vector1.begin(), vector1.end());
				Assert::IsTrue(vector1.begin() != vector1.end());

				Vector<Foo> vector3;
				Assert::AreEqual(vector3.begin(), vector3.end());
				Assert::IsFalse(vector3.begin() != vector3.end());
			}

			// operator+= & +
			{
				// Default constructed
				Vector<Foo>::iterator it;
				Assert::ExpectException<runtime_error>([&it]() {  auto temp = it + 1; UNREFERENCED_LOCAL(temp); });
				Assert::ExpectException<runtime_error>([&it]() { it += 1; });

				// Non default constructed
				const Foo a(1);
				const Foo b(1);
				Vector<Foo> vector = { a, b };
				it = vector.begin();
				Assert::AreEqual(vector.end(), it + 2);
				Assert::AreEqual(vector.end(), it += 2);
			}

			// operator-= & -
			{
				// Default constructed
				Vector<Foo>::iterator it;
				Assert::ExpectException<runtime_error>([&it]() { auto temp = it - 1; UNREFERENCED_LOCAL(temp); });
				Assert::ExpectException<runtime_error>([&it]() { it -= 1; });

				// Non default constructed
				const Foo a(1);
				const Foo b(1);
				Vector<Foo> vector = { a, b };
				it = vector.end();
				Assert::AreEqual(vector.begin(), it - 2);
				Assert::AreEqual(vector.begin(), it -= 2);

			}

			// Subtract two iterators
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector{ a, b };
				Vector<Foo> vector2{ a, b };
				Vector<Foo>::iterator it = vector.begin();
				Vector<Foo>::iterator it2 = vector2.begin();

				// subtract two iterators from different containers
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it - it2; UNREFERENCED_LOCAL(b); });

				// subtract two iterators from same container
				Assert::AreEqual(size_t(2), vector.end() - vector.begin());
			}

			// Compare iterators
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector{ a, b };
				Vector<Foo> vector2{ a, b };
				Vector<Foo>::iterator it = vector.begin();
				Vector<Foo>::iterator it2 = vector2.begin();
				
				// compare two iterators from different containers
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it < it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it <= it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it > it2; UNREFERENCED_LOCAL(b); });
				Assert::ExpectException<runtime_error>([&it, &it2]() { auto b = it >= it2; UNREFERENCED_LOCAL(b); });

				// compare two iterators from same container
				Assert::IsTrue(vector.begin() < vector.end());
				Assert::IsFalse(vector.begin() < vector.begin());
				Assert::IsFalse(vector.end() < vector.begin());
				Assert::IsTrue(vector.begin() <= vector.end());
				Assert::IsTrue(vector.begin() <= vector.begin());
				Assert::IsFalse(vector.end() <= vector.begin());

				Assert::IsFalse(vector.begin() > vector.end());
				Assert::IsFalse(vector.begin() > vector.begin());
				Assert::IsTrue(vector.end() > vector.begin());
				Assert::IsFalse(vector.begin() >= vector.end());
				Assert::IsTrue(vector.begin() >= vector.begin());
				Assert::IsTrue(vector.end() >= vector.begin());
			}

			// random access an iterator
			{
				const Foo a(1);
				const Foo b(2);
				Vector<Foo> vector{ a, b };
				Vector<Foo>::iterator it = vector.end();
				Assert::AreEqual(a, it[0]);
				Assert::AreEqual(b, it[1]);

				// default constructed
				Vector<Foo>::iterator itDefault;
				Assert::ExpectException<runtime_error>([&itDefault]() { auto& v = itDefault[0]; UNREFERENCED_LOCAL(v); });
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}