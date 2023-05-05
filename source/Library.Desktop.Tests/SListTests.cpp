#include "pch.h"
#include "Literals.h"
#include "CppUnitTest.h"
#include <cstdint>
#include "Foo.h"
#include "SList.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Foo>::iterator>(const SList<Foo>::iterator& t)
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
	inline std::wstring ToString<SList<Foo>::const_iterator>(const SList<Foo>::const_iterator& t)
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
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(true, list.IsEmpty());
		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			
			// Populate initial list
			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreEqual(size_t(3), list.Size());

			// Copy
			SList<Foo> list2(list);

			// First element check
			Assert::AreEqual(size_t(3), list2.Size());
			Assert::AreEqual(a, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());

			// Second element check
			list.PopFront();
			list2.PopFront();
			Assert::AreEqual(b, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());
			
			// Third element check
			list.PopFront();
			list2.PopFront();
			Assert::AreEqual(c, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());

			// Check size was actually correct after copy
			list2.PopFront();
			Assert::ExpectException<std::runtime_error>([&list2]() { auto& front = list2.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list2]() { auto& back = list2.Back(); UNREFERENCED_LOCAL(back); });
		}

		TEST_METHOD(Move)
		{
			// Move constructor
			{

				SList<Foo> sList = { Foo(1), Foo(2) };
				Assert::AreEqual(size_t(2), sList.Size());
				Assert::IsFalse(sList.IsEmpty());
				SList<Foo> newSList(std::move(sList));
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), sList.Size());
				Assert::IsTrue(sList.IsEmpty());
#pragma warning(pop)
				Assert::AreEqual(size_t(2), newSList.Size());
				Assert::IsFalse(newSList.IsEmpty());
			}

			// Move assignment
			{
				SList<Foo> sList = { Foo(1), Foo(2) };
				SList<Foo> newSList{};
				Assert::AreEqual(size_t(2), sList.Size());
				Assert::IsFalse(sList.IsEmpty());
				newSList = std::move(sList);
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), sList.Size());
				Assert::IsTrue(sList.IsEmpty());
#pragma warning(pop)
				Assert::AreEqual(size_t(2), newSList.Size());
				Assert::IsFalse(newSList.IsEmpty());
			}
		}

		TEST_METHOD(Assignemnt)
		{
			const Foo a1{ 10 };
			const Foo b1{ 20 };
			const Foo c1{ 30 };

			const Foo a2{ 100 };
			const Foo b2{ 200 };

			// Populate first list
			SList<Foo> list;
			list.PushBack(a1);
			list.PushBack(b1);
			list.PushBack(c1);

			Assert::AreEqual(a1, list.Front());
			Assert::AreEqual(c1, list.Back());
			Assert::AreEqual(size_t(3), list.Size());

			// Populate second list
			SList<Foo> list2;
			list2.PushBack(a2);
			list2.PushBack(b2);

			Assert::AreEqual(a2, list2.Front());
			Assert::AreEqual(b2, list2.Back());
			Assert::AreEqual(size_t(2), list2.Size());

			// Assign list2
			list2 = list;

			Assert::AreEqual(a1, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());
			Assert::AreEqual(c1, list2.Back());
			Assert::AreNotSame(list.Back(), list2.Back());
			Assert::AreEqual(size_t(3), list2.Size());

			// Second element check
			list.PopFront();
			list2.PopFront();
			Assert::AreEqual(b1, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());

			// Third element check
			list.PopFront();
			list2.PopFront();
			Assert::AreEqual(c1, list2.Front());
			Assert::AreNotSame(list.Front(), list2.Front());

			// Check size was actually correct after assignment
			list2.PopFront();
			Assert::ExpectException<std::runtime_error>([&list2]() { auto& front = list2.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list2]() { auto& back = list2.Back(); UNREFERENCED_LOCAL(back); });
		}

		TEST_METHOD(Emplace)
		{
			// Front
			{
				SList<Foo> list;
				Assert::AreEqual(size_t(0), list.Size());
				list.EmplaceFront(1);
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(Foo(1), list.Front());
			}

			// Back
			{
				SList<Foo> list;
				Assert::AreEqual(size_t(0), list.Size());
				list.EmplaceBack(1);
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(Foo(1), list.Front());
			}
		}

		TEST_METHOD(PushFront)
		{
			Foo a(1);
			Foo b(2);

			// Create list
			SList<Foo> list;
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(true, list.IsEmpty());

			// Add first element
			list.PushFront(a);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			// Check copy was made
			Assert::AreNotSame(a, list.Front());
			Assert::AreNotSame(a, list.Back());
			Assert::AreEqual(false, list.IsEmpty());

			// Add second element
			list.PushFront(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
			// Check copy was made
			Assert::AreNotSame(b, list.Front());
			Assert::AreNotSame(a, list.Back());
			Assert::AreEqual(false, list.IsEmpty());
		}

		TEST_METHOD(PopFront)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);

			// Create list
			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			// Initial check
			Assert::AreEqual(a, list.Front());
			Assert::AreNotSame(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreNotSame(c, list.Back());
			Assert::AreEqual(size_t(3), list.Size());

			// Pop
			list.PopFront();
			Assert::AreEqual(b, list.Front());
			Assert::AreNotSame(b, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreNotSame(c, list.Back());
			Assert::AreEqual(size_t(2), list.Size());

			// Pop
			list.PopFront();
			Assert::AreEqual(c, list.Front());
			Assert::AreNotSame(c, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::AreNotSame(c, list.Back());
			Assert::AreEqual(size_t(1), list.Size());

			// Pop
			list.PopFront();
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(size_t(0), list.Size());

			// Popping with nothing in list creates exception
			Assert::ExpectException <std::runtime_error>([&list]() { list.PopFront(); });
		}

		TEST_METHOD(PushBack)
		{
			Foo a(1);
			Foo b(2);

			// Create list
			SList<Foo> list;
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(true, list.IsEmpty());

			// Add first element
			list.PushBack(a);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			// Check copy was made
			Assert::AreNotSame(a, list.Front());
			Assert::AreNotSame(a, list.Back());
			Assert::AreEqual(false, list.IsEmpty());

			// Add second element
			list.PushBack(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());
			// Check copy was made
			Assert::AreNotSame(a, list.Front());
			Assert::AreNotSame(b, list.Back());
			Assert::AreEqual(false, list.IsEmpty());
		}

		TEST_METHOD(PopBack)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);

			// Create list
			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			// Initial check
			Assert::AreEqual(c, list.Back());
			Assert::AreNotSame(c, list.Back());
			Assert::AreEqual(a, list.Front());
			Assert::AreNotSame(a, list.Front());
			Assert::AreEqual(size_t(3), list.Size());

			// Pop
			list.PopBack();
			Assert::AreEqual(b, list.Back());
			Assert::AreNotSame(b, list.Back());
			Assert::AreEqual(a, list.Front());
			Assert::AreNotSame(a, list.Front());
			Assert::AreEqual(size_t(2), list.Size());

			// Pop
			list.PopBack();
			Assert::AreEqual(a, list.Back());
			Assert::AreNotSame(a, list.Back());
			Assert::AreEqual(a, list.Front());
			Assert::AreNotSame(a, list.Front());
			Assert::AreEqual(size_t(1), list.Size());

			// Pop
			list.PopBack();
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(size_t(0), list.Size());

			// Popping with nothing in list creates exception
			Assert::ExpectException <std::runtime_error>([&list]() { list.PopBack(); });
		}

		TEST_METHOD(IsEmpty)
		{
			SList<Foo> list;
			Assert::AreEqual(true, list.IsEmpty());

			list.PushFront(Foo{1});

			Assert::AreEqual(false, list.IsEmpty());

			list.Clear();

			Assert::AreEqual(true, list.IsEmpty());
		}

		TEST_METHOD(Front)
		{
			// Non-const
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
				Foo a(1);
				Foo b(2);
				Foo c(2);

				// Push front and verify front changed
				list.PushFront(a);
				Assert::AreEqual(a, list.Front());

				// Push front and verify front changed
				list.PushFront(b);
				Assert::AreEqual(b, list.Front());

				// Push back and verify no change
				list.PushBack(c);
				Assert::AreEqual(b, list.Front());

				// After clear check front throws exception
				list.Clear();
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			}

			// Const
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = const_cast<const SList<Foo>&>(list).Front(); UNREFERENCED_LOCAL(front); });
				Foo a(1);
				Foo b(2);
				Foo c(2);

				// Push front and verify front changed
				list.PushFront(a);
				Assert::AreEqual(a, const_cast<const SList<Foo>&>(list).Front());

				// Push front and verify front changed
				list.PushFront(b);
				Assert::AreEqual(b, const_cast<const SList<Foo>&>(list).Front());

				// Push back and verify no change
				list.PushBack(c);
				Assert::AreEqual(b, const_cast<const SList<Foo>&>(list).Front());

				// After clear check front throws exception
				list.Clear();
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = const_cast<const SList<Foo>&>(list).Front(); UNREFERENCED_LOCAL(front); });
			}
		}

		TEST_METHOD(Back)
		{
			// Non-const
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
				Foo a(1);
				Foo b(2);
				Foo c(2);

				// Push back and verify back changed
				list.PushBack(a);
				Assert::AreEqual(a, list.Back());

				// Push back and verify back changed
				list.PushBack(b);
				Assert::AreEqual(b, list.Back());

				// Push front and verify no change
				list.PushFront(c);
				Assert::AreEqual(b, list.Back());

				// After clear check back throws exception
				list.Clear();
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			}
			// Const
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = const_cast<const SList<Foo>&>(list).Back(); UNREFERENCED_LOCAL(back); });
				Foo a(1);
				Foo b(2);
				Foo c(2);

				// Push back and verify back changed
				list.PushBack(a);
				Assert::AreEqual(a, const_cast<const SList<Foo>&>(list).Back());

				// Push back and verify back changed
				list.PushBack(b);
				Assert::AreEqual(b, const_cast<const SList<Foo>&>(list).Back());

				// Push front and verify no change
				list.PushFront(c);
				Assert::AreEqual(b, const_cast<const SList<Foo>&>(list).Back());

				// After clear check back throws exception
				list.Clear();
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = const_cast<const SList<Foo>&>(list).Back(); UNREFERENCED_LOCAL(back); });
			}
		}

		TEST_METHOD(Size)
		{
			SList<Foo> list;

			Foo a(1);
			Assert::AreEqual(size_t(0), list.Size());

			list.PushFront(a);
			Assert::AreEqual(size_t(1), list.Size());

			list.PushBack(a);
			Assert::AreEqual(size_t(2), list.Size());

			list.Clear();
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;

			Foo a(1);

			// Initial conditions check
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(size_t(0), list.Size());
			
			// Clear empty list
			list.Clear();

			// Nothing should have changed
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(size_t(0), list.Size());

			// Add elements
			list.PushBack(a);
			list.PushBack(a);
			list.PushBack(a);

			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(size_t(3), list.Size());

			// Clear
			list.Clear();

			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(begin)
		{
			// Non-const
			{
				SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

				SList<Foo>::iterator it = list.begin();
				Assert::AreSame(list.Front(), *list.begin());

				SList<Foo> list2;
				SList<Foo>::iterator it2 = list2.begin();
				Assert::ExpectException<std::runtime_error>([&it2]() { Foo& value = *it2; UNREFERENCED_LOCAL(value); });
			}

			// Non-const
			{
				SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

				SList<Foo>::const_iterator it = list.begin();
				Assert::AreSame(list.Front(), *list.begin());

				const SList<Foo> list2;
				SList<Foo>::const_iterator it2 = list2.begin();
				Assert::ExpectException<std::runtime_error>([&it2]() { const Foo& value = *it2; UNREFERENCED_LOCAL(value); });
			}
		}

		TEST_METHOD(end)
		{
			// Non-const
			{
				SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

				SList<Foo>::iterator it = list.end();
				Assert::ExpectException<std::runtime_error>([&it]() { Foo& value = *it; UNREFERENCED_LOCAL(value); });
			}

			// Const
			{
				SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

				SList<Foo>::const_iterator it = list.end();
				Assert::ExpectException<std::runtime_error>([&it]() { const Foo& value = *it; UNREFERENCED_LOCAL(value); });
			}
		}

		TEST_METHOD(cbegin)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

			SList<Foo>::const_iterator it = list.cbegin();
			Assert::AreSame(list.Front(), *list.cbegin());

			SList<Foo> list2;
			SList<Foo>::const_iterator it2 = list2.cbegin();
			Assert::ExpectException<std::runtime_error>([&it2]() { const Foo& value = *it2; UNREFERENCED_LOCAL(value); });
		}

		TEST_METHOD(cend)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };

			SList<Foo>::const_iterator it = list.cend();
			Assert::ExpectException<std::runtime_error>([&it]() { const Foo& value = *it; UNREFERENCED_LOCAL(value); });
		}

		TEST_METHOD(InsertAfter)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Foo bad(5);

			SList<Foo>::iterator defaultIt;
			SList<Foo> list = { a, c };
			// After first insert 
			SList<Foo> list2 = { a, b, c };
			// After second insert
			SList<Foo> list3 = { a, b, c, d};
			
			Assert::ExpectException<std::runtime_error>([&list, &defaultIt, &bad]() { list.InsertAfter(defaultIt, bad); });
			Assert::AreEqual(size_t(2), list.Size());

			// Insert after first element
			list.InsertAfter(list.begin(), b);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::IsTrue(std::equal(list.begin(), list.end(), list2.begin()));

			// Insert after last element
			list.InsertAfter(list.end(), d);
			Assert::AreEqual(size_t(4), list.Size());
			Assert::IsTrue(std::equal(list.begin(), list.end(), list3.begin()));
		}
		
		TEST_METHOD(Find)
		{
			// Non-const
			{
				SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
				SList<Foo>::iterator it = list.Find(Foo(1));
				Assert::AreEqual(list.begin(), it);

				it = list.Find(Foo(4));
				Assert::AreEqual(list.end(), it);
			}

			// Const
			{
				const SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
				SList<Foo>::const_iterator it = list.Find(Foo(1));
				Assert::AreEqual(list.begin(), it);

				it = list.Find(Foo(4));
				Assert::AreEqual(list.end(), it);
			}
		}

		TEST_METHOD(Remove)
		{
			// Value
			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);

				SList<Foo> list = { a, b, c };
				bool rt;

				// Remove element that doesn't exist in container
				rt = list.Remove(d);
				Assert::IsFalse(rt);
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(c, list.Back());

				// Remove first element
				rt = list.Remove(a);
				Assert::IsTrue(rt);
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(b, list.Front());
				Assert::AreEqual(c, list.Back());

				// Remove last element
				rt = list.Remove(c);
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(b, list.Front());
				Assert::AreEqual(b, list.Back());

				// Remove remaining element
				rt = list.Remove(b);
				Assert::AreEqual(size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			
				SList<Foo> oneElementList = { a };
				Assert::AreEqual(size_t(1), oneElementList.Size());
				Assert::AreEqual(a, oneElementList.Front());
				Assert::AreEqual(a, oneElementList.Back());

				oneElementList.Remove(a);

				Assert::AreEqual(size_t(0), oneElementList.Size());
				Assert::ExpectException<std::runtime_error>([&oneElementList]() { auto& front = oneElementList.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&oneElementList]() { auto& back = oneElementList.Back(); UNREFERENCED_LOCAL(back); });
			}

			// Iterator
			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);

				SList<Foo> list = { a, b, c };
				bool rt;

				// Remove default constructed iterator
				rt = list.Remove(SList<Foo>::iterator{});
				Assert::IsFalse(rt);
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(c, list.Back());

				// Remove element that doesn't exist in container
				rt = list.Remove(list.end());
				Assert::IsFalse(rt);
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(c, list.Back());

				// Remove first element
				rt = list.Remove(list.begin());
				Assert::IsTrue(rt);
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(b, list.Front());
				Assert::AreEqual(c, list.Back());

				// Remove last element
				SList<Foo>::iterator it = list.begin();
				++it;

				rt = list.Remove(it);
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(b, list.Front());
				Assert::AreEqual(b, list.Back());

				// Remove remaining element
				rt = list.Remove(list.begin());
				Assert::AreEqual(size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
				Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			}
		}

		TEST_METHOD(iterator)
		{
			// operator*
			{
				// Default constructed iterator
				SList<Foo>::iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				// Iterator from list
				Foo a(1);
				SList<Foo> list = { a };

				it = list.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				it = list.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(1, it->Data());
			}

			// operator++
			{
				// Default constructed iterator
				SList<Foo>::iterator it;
				SList<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it++; });

				// Iterator from list
				Foo a(1);
				Foo b(2);
				SList<Foo> list = { a, b };

				// First element
				it = list.begin();
				Assert::AreEqual(a, *it);

				previousIt = it++;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(list.begin(), previousIt);
				Assert::AreEqual(a, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::AreEqual(list.end(), previousIt);
			}

			// ++operator
			{
				// Default constructed iterator
				SList<Foo>::iterator it;
				SList<Foo>::iterator it2;
				SList<Foo>::iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { ++it; });

				// Iterator from list
				Foo a(1);
				Foo b(2);
				SList<Foo> list = { a, b };

				// First element
				it = list.begin();
				Assert::AreEqual(a, *it);

				it2 = ++it;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it2);
			}

			// operator== & !=
			{
				// Compare default constructed iterators
				SList<Foo>::iterator it;
				SList<Foo>::iterator it2;
				Assert::AreEqual(it, it2);
				Assert::IsFalse(it != it2);

				// Comparing two lists
				SList<Foo> list1 = { Foo(1), Foo(2) };
				SList<Foo> list2 = { Foo(1), Foo(2) };
				it = list1.begin();
				it2 = list2.begin();
				Assert::AreNotEqual(it, it2);
				Assert::IsTrue(it != it2);

				// Comparing elements in same list
				Assert::AreEqual(list1.begin(), list1.begin());
				Assert::AreEqual(list1.end(), list1.end());
				Assert::AreNotEqual(list1.begin(), list1.end());
				Assert::IsTrue(list1.begin() != list1.end());
				
				SList<Foo> list3;
				Assert::AreEqual(list3.begin(), list3.end());
				Assert::IsFalse(list3.begin() != list3.end());
			}
		}

		TEST_METHOD(const_iterator)
		{
			// Typecast constructor
			{
				SList<Foo> list = { Foo(1), Foo(2) };
				SList<Foo>::const_iterator it = list.begin();
				Assert::AreEqual(list.cbegin(), it);
				Assert::AreEqual(++list.cbegin(), ++it);
			}

			// operator*
			{
				// Default constructed iterator
				SList<Foo>::const_iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				// Iterator from list
				Foo a(1);
				const SList<Foo> list = { a };

				it = list.end();
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });

				it = list.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(1, it->Data());
			}

			// operator++
			{
				// Default constructed iterator
				SList<Foo>::const_iterator it;
				SList<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { it++; });

				// Iterator from list
				Foo a(1);
				Foo b(2);
				const SList<Foo> list = { a, b };

				// First element
				it = list.begin();
				Assert::AreEqual(a, *it);

				previousIt = it++;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(list.begin(), previousIt);
				Assert::AreEqual(a, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::AreEqual(b, *previousIt);

				previousIt = it++;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::ExpectException<std::runtime_error>([&previousIt]() { auto& foo = *previousIt; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::AreEqual(list.end(), previousIt);
			}

			// ++operator
			{
				// Default constructed iterator
				SList<Foo>::const_iterator it;
				SList<Foo>::const_iterator it2;
				SList<Foo>::const_iterator previousIt;
				Assert::ExpectException<std::runtime_error>([&it]() { ++it; });

				// Iterator from list
				Foo a(1);
				Foo b(2);
				const SList<Foo> list = { a, b };

				// First element
				it = list.begin();
				Assert::AreEqual(a, *it);

				it2 = ++it;

				// Second element
				Assert::AreEqual(b, *it);
				Assert::AreEqual(b, *it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it2);

				it2 = ++it;

				// end
				Assert::ExpectException<std::runtime_error>([&it]() { auto& foo = *it; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it);
				Assert::ExpectException<std::runtime_error>([&it2]() { auto& foo = *it2; UNREFERENCED_LOCAL(foo); });
				Assert::AreEqual(list.end(), it2);
			}

			// operator== & !=
			{
				// Compare default constructed iterators
				SList<Foo>::const_iterator it;
				SList<Foo>::const_iterator it2;
				Assert::AreEqual(it, it2);
				Assert::IsFalse(it != it2);

				// Comparing two lists
				const SList<Foo> list1 = { Foo(1), Foo(2) };
				const SList<Foo> list2 = { Foo(1), Foo(2) };
				it = list1.begin();
				it2 = list2.begin();
				Assert::AreNotEqual(it, it2);
				Assert::IsTrue(it != it2);

				// Comparing elements in same list
				Assert::AreEqual(list1.begin(), list1.begin());
				Assert::AreEqual(list1.end(), list1.end());
				Assert::AreNotEqual(list1.begin(), list1.end());
				Assert::IsTrue(list1.begin() != list1.end());

				const SList<Foo> list3;
				Assert::AreEqual(list3.begin(), list3.end());
				Assert::IsFalse(list3.begin() != list3.end());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
