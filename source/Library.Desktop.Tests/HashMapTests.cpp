#include "pch.h"
#include "Literals.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "HashMap.h"
#include "ToStringSpecializations.h"
#include "HashSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::PairType>(const HashMap<Foo, Foo>::PairType& pair)
	{
		return ToString(pair.first) + L","s + ToString(pair.second);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::iterator>(const HashMap<Foo, Foo>::iterator& t)
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
	inline std::wstring ToString<HashMap<Foo, Foo>::const_iterator>(const HashMap<Foo, Foo>::const_iterator& t)
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
	TEST_CLASS(HashMapTests)
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
			// No bucket count specified
			{
				const HashMap<Foo, Foo> hashMap;
				Assert::AreEqual(size_t(0), hashMap.Size());
				Assert::AreEqual(0.0f, hashMap.LoadFactor());
				Assert::AreEqual(hashMap.end(), hashMap.begin());
			}

			// Check bucket count
			{
				size_t expectedBucketCount = 13;
				const HashMap<Foo, Foo> hashMap(expectedBucketCount);
				Assert::AreEqual(expectedBucketCount, hashMap.BucketCount());
				Assert::AreEqual(size_t(0), hashMap.Size());
				Assert::AreEqual(0.0f, hashMap.LoadFactor());
				Assert::AreEqual(hashMap.end(), hashMap.begin());
			}
		}

		TEST_METHOD(CopySemantics)
		{
			HashMap<Foo, Foo> hashMap;
			hashMap.Insert(std::pair(Foo(1), Foo(2)));
			hashMap.Insert(std::pair(Foo(3), Foo(4)));
			// Copy
			HashMap<Foo, Foo> hashMapCopy = hashMap;
			Assert::IsTrue(std::equal(hashMapCopy.begin(), hashMapCopy.end(), hashMap.begin()));
		}

		TEST_METHOD(Find)
		{
			// Non-const
			{
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(std::pair(Foo(1), Foo(2)));
				hashMap.Insert(std::pair(Foo(3), Foo(4)));

				// Should find
				Assert::AreNotEqual(hashMap.end(), hashMap.Find(Foo(1)));
				// Should not find
				Assert::AreEqual(hashMap.end(), hashMap.Find(Foo(2)));
				// Should find
				Assert::AreNotEqual(hashMap.end(), hashMap.Find(Foo(3)));
			}

			// Const
			{
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(std::pair(Foo(1), Foo(2)));
				hashMap.Insert(std::pair(Foo(3), Foo(4)));

				const HashMap<Foo, Foo>& constHashMap = hashMap;

				// Should find
				Assert::AreNotEqual(constHashMap.end(), constHashMap.Find(Foo(1)));
				// Should not find
				Assert::AreEqual(constHashMap.end(), constHashMap.Find(Foo(2)));
				// Should find
				Assert::AreNotEqual(constHashMap.end(), constHashMap.Find(Foo(3)));
			}
		}

		TEST_METHOD(Insert)
		{
			std::pair<HashMap<Foo, Foo>::iterator, bool> ret;
			HashMap<Foo, Foo>::iterator firstIt;
			HashMap<Foo, Foo> hashMap;
			Assert::AreEqual(size_t(0), hashMap.Size());
			ret = hashMap.Insert(std::pair(Foo(1), Foo(2)));
			firstIt = ret.first;
			Assert::AreEqual(true, ret.second);
			Assert::AreNotEqual(hashMap.end(), ret.first);
			Assert::AreEqual(size_t(1), hashMap.Size());
			// Insert same key
			ret = hashMap.Insert(std::pair(Foo(1), Foo(3)));
			Assert::AreEqual(false, ret.second);
			Assert::AreNotEqual(hashMap.end(), ret.first);
			Assert::AreEqual(firstIt, ret.first);
			Assert::AreEqual(size_t(1), hashMap.Size());
			// Insert different key
			ret = hashMap.Insert(std::pair(Foo(2), Foo(4)));
			Assert::AreEqual(true, ret.second);
			Assert::AreNotEqual(hashMap.end(), ret.first);
			Assert::AreEqual(size_t(2), hashMap.Size());
		}

		TEST_METHOD(OperatorBracketBracket)
		{
			HashMap<Foo, Foo> hashMap;
			Assert::AreEqual(size_t(0), hashMap.Size());

			// Get non inserted element
			const Foo& value1 = hashMap[Foo(1)];
			Assert::AreEqual(size_t(1), hashMap.Size());
			// Check was default initialized
			Assert::AreEqual(Foo{}, value1);

			// Get inserted element
			hashMap.Insert({Foo(2), Foo(100)});
			Assert::AreEqual(size_t(2), hashMap.Size());
			const Foo& value2 = hashMap[Foo(2)];
			Assert::AreEqual(size_t(2), hashMap.Size());
			Assert::AreEqual(Foo(100), value2);
		}

		TEST_METHOD(Remove)
		{
			HashMap<Foo, Foo> hashMap;
			bool ret;
			Assert::AreEqual(size_t(0), hashMap.Size());

			// Remove key not in hash map
			ret = hashMap.Remove(Foo(1));
			Assert::IsFalse(ret);
			Assert::AreEqual(size_t(0), hashMap.Size());

			// Insert key
			hashMap.Insert({ Foo(1), Foo(2) });
			Assert::AreEqual(size_t(1), hashMap.Size());

			// Remove key just inserted
			ret = hashMap.Remove(Foo(1));
			Assert::IsTrue(ret);
			Assert::AreEqual(size_t(0), hashMap.Size());
		}

		TEST_METHOD(Clear)
		{
			HashMap<Foo, Foo> hashMap;
			hashMap.Insert({ Foo(1), Foo(2) });
			hashMap.Insert({ Foo(2), Foo(2) });
			Assert::AreEqual(size_t(2), hashMap.Size());
			Assert::IsTrue(hashMap.LoadFactor() > 0);
			Assert::AreNotEqual(hashMap.end(), hashMap.Find(Foo(1)));

			// Clear
			hashMap.Clear();
			Assert::AreEqual(size_t(0), hashMap.Size());
			Assert::AreEqual(0.0f, hashMap.LoadFactor());
			Assert::AreEqual(hashMap.end(), hashMap.Find(Foo(1)));
		}

		TEST_METHOD(Size)
		{
			HashMap<Foo, Foo> hashMap;
			Assert::AreEqual(size_t(0), hashMap.Size());
			hashMap.Insert({ Foo(1), Foo(2) });
			Assert::AreEqual(size_t(1), hashMap.Size());
			hashMap.Insert({ Foo(2), Foo(2) });
			Assert::AreEqual(size_t(2), hashMap.Size());
			hashMap.Clear();
			Assert::AreEqual(size_t(0), hashMap.Size());
		}

		TEST_METHOD(BucketCount)
		{
			HashMap<Foo, Foo> hashMap(11);
			Assert::AreEqual(size_t(11), hashMap.BucketCount());

			HashMap<Foo, Foo> hashMap2(13);
			Assert::AreEqual(size_t(13), hashMap2.BucketCount());
		}

		TEST_METHOD(LoadFactor)
		{
			HashMap<Foo, Foo> hashMap(11);
			Assert::AreEqual(0.0f, const_cast<const HashMap<Foo, Foo>&>(hashMap).LoadFactor());
			Assert::AreEqual(0.0f, hashMap.LoadFactor());

			hashMap.Insert({ Foo(1), Foo(2) });
			Assert::IsTrue(const_cast<const HashMap<Foo, Foo>&>(hashMap).LoadFactor() > 0.0f);
			Assert::IsTrue(hashMap.LoadFactor() > 0.0f);

			hashMap.Clear();
			Assert::AreEqual(0.0f, const_cast<const HashMap<Foo, Foo>&>(hashMap).LoadFactor());
			Assert::AreEqual(0.0f, hashMap.LoadFactor());
		}

		TEST_METHOD(ContainsKey)
		{
			HashMap<Foo, Foo> hashMap(11);
			Assert::IsFalse(hashMap.ContainsKey(Foo(1)));
			hashMap.Insert({Foo(1), Foo(2)});
			Assert::IsTrue(hashMap.ContainsKey(Foo(1)));
			hashMap.Remove(Foo(1));
			Assert::IsFalse(hashMap.ContainsKey(Foo(1)));
		}

		TEST_METHOD(At)
		{
			// Non-const
			{
				const Foo key(1);
				Foo value(2);
				HashMap<Foo, Foo> hashMap(11);
				Assert::ExpectException<runtime_error>([&hashMap, &key]() { auto v = hashMap.At(key); UNREFERENCED_LOCAL(v); });

				hashMap.Insert({ key, value });
				Assert::AreEqual(value, hashMap.At(key));
				Assert::AreNotSame(value, hashMap.At(key));
			}

			// Const
			{
				const Foo key(1);
				Foo value(2);
				HashMap<Foo, Foo> hashMap(11);
				Assert::ExpectException<runtime_error>([&hashMap, &key]() { auto v = const_cast<const HashMap<Foo, Foo>&>(hashMap).At(key); UNREFERENCED_LOCAL(v); });

				hashMap.Insert({ key, value });
				Assert::AreEqual(value, const_cast<const HashMap<Foo, Foo>&>(hashMap).At(key));
				Assert::AreNotSame(value, const_cast<const HashMap<Foo, Foo>&>(hashMap).At(key));
			}
		}

		TEST_METHOD(Resize)
		{
			HashMap<Foo, Foo> hashMap(11);
			hashMap.Insert({Foo(1), Foo(2)});
			Assert::AreEqual(size_t(11), hashMap.BucketCount());
			hashMap.Resize(29);
			Assert::AreEqual(size_t(29), hashMap.BucketCount());
			Assert::ExpectException<runtime_error>([&hashMap]() { hashMap.Resize(11); });
			Assert::IsTrue(hashMap.ContainsKey(Foo(1)));
		}

		TEST_METHOD(begin)
		{
			// Non-const
			{
				const HashMap<Foo, Foo>::PairType pair{ Foo(1), Foo(2) };
				HashMap<Foo, Foo> hashMap(11);
				Assert::AreEqual(hashMap.end(), hashMap.begin());
				// Insert
				hashMap.Insert(pair);
				Assert::AreEqual(pair, *hashMap.begin());
				// Remove
				hashMap.Remove(pair.first);
				Assert::AreEqual(hashMap.end(), hashMap.begin());
			}

			// Const
			{
				const HashMap<Foo, Foo>::PairType pair{ Foo(1), Foo(2) };
				HashMap<Foo, Foo> hashMap(11);
				Assert::AreEqual(const_cast<const HashMap<Foo, Foo>&>(hashMap).end(), const_cast<const HashMap<Foo, Foo>&>(hashMap).begin());
				// Insert
				hashMap.Insert(pair);
				Assert::AreEqual(pair, *const_cast<const HashMap<Foo, Foo>&>(hashMap).begin());
				// Remove
				hashMap.Remove(pair.first);
				Assert::AreEqual(const_cast<const HashMap<Foo, Foo>&>(hashMap).end(), const_cast<const HashMap<Foo, Foo>&>(hashMap).begin());
			}
		}

		TEST_METHOD(cbegin)
		{
			const HashMap<Foo, Foo>::PairType pair{ Foo(1), Foo(2) };
			HashMap<Foo, Foo> hashMap(11);
			Assert::AreEqual(hashMap.cend(), hashMap.cbegin());
			// Insert
			hashMap.Insert(pair);
			Assert::AreEqual(pair, *hashMap.cbegin());
			// Remove
			hashMap.Remove(pair.first);
			Assert::AreEqual(hashMap.cend(), hashMap.cbegin());
		}

		TEST_METHOD(end)
		{
			// Non-const
			{
				HashMap<Foo, Foo> hashMap(11);
				Assert::ExpectException<runtime_error>([&hashMap]() { auto& pair = *hashMap.end(); UNREFERENCED_LOCAL(pair); });
				Assert::AreEqual(hashMap.begin(), hashMap.end());
				hashMap.Insert({ Foo(1), Foo(2) });
				Assert::AreNotEqual(hashMap.begin(), hashMap.end());
				HashMap<Foo, Foo>::iterator it = hashMap.begin();
				++it;
				Assert::AreEqual(it, hashMap.end());
			}

			// Const
			{
				HashMap<Foo, Foo> hashMap(11);
				const HashMap<Foo, Foo>& constHashMap = hashMap;
				Assert::ExpectException<runtime_error>([&constHashMap]() { auto& pair = *constHashMap.end(); UNREFERENCED_LOCAL(pair); });
				Assert::AreEqual(constHashMap.begin(), constHashMap.end());
				hashMap.Insert({ Foo(1), Foo(2) });
				Assert::AreNotEqual(constHashMap.begin(), constHashMap.end());
				HashMap<Foo, Foo>::const_iterator it = constHashMap.begin();
				++it;
				Assert::AreEqual(it, constHashMap.end());
			}
		}

		TEST_METHOD(cend)
		{
			HashMap<Foo, Foo> hashMap(11);
			Assert::ExpectException<runtime_error>([&hashMap]() { auto& pair = *hashMap.cend(); UNREFERENCED_LOCAL(pair); });
			Assert::AreEqual(hashMap.cbegin(), hashMap.cend());
			hashMap.Insert({ Foo(1), Foo(2) });
			Assert::AreNotEqual(hashMap.cbegin(), hashMap.cend());
			HashMap<Foo, Foo>::const_iterator it = hashMap.cbegin();
			++it;
			Assert::AreEqual(it, hashMap.cend());
		}

		TEST_METHOD(const_iterator)
		{
			// iterator cast
			{
				HashMap<Foo, Foo>::PairType pair(Foo(1), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair);
				HashMap<Foo, Foo>::iterator it = hashMap.begin();
				HashMap<Foo, Foo>::const_iterator constIt(it);
				Assert::AreEqual(*it, *constIt);
			}

			// operator*
			{
				HashMap<Foo, Foo>::PairType pair(Foo(1), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair);
				HashMap<Foo, Foo>::const_iterator it = hashMap.cbegin();
				Assert::AreEqual(pair, *it);
				Assert::AreEqual(pair.first, it->first);
				++it;
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::const_iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { const auto& pair = *defaultIt; UNREFERENCED_LOCAL(pair); });
			}

			// ++operator
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				const HashMap<Foo, Foo>::PairType pair2(Foo(2), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);
				hashMap.Insert(pair2);
				HashMap<Foo, Foo>::const_iterator oldIt;
				HashMap<Foo, Foo>::const_iterator it = hashMap.cbegin();

				Assert::AreNotEqual(hashMap.cend(), it);
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::AreNotEqual(hashMap.cend(), it);
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });
				// Iterate past end
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::const_iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { ++defaultIt; });
			}

			// operator++
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				const HashMap<Foo, Foo>::PairType pair2(Foo(2), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);
				hashMap.Insert(pair2);
				HashMap<Foo, Foo>::const_iterator oldIt;
				HashMap<Foo, Foo>::const_iterator it = hashMap.cbegin();

				Assert::AreNotEqual(hashMap.cend(), it);
				oldIt = it++;
				Assert::AreNotEqual(oldIt, it);
				Assert::AreNotEqual(hashMap.cend(), it);
				Assert::AreEqual(hashMap.cbegin(), oldIt);
				oldIt = it++;
				Assert::AreNotEqual(oldIt, it);
				Assert::AreEqual(hashMap.cend(), it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });
				// Iterate past end
				oldIt = it++;
				Assert::AreEqual(oldIt, it);
				Assert::AreEqual(hashMap.cend(), it);
				Assert::AreEqual(hashMap.cend(), oldIt);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::const_iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { ++defaultIt; });
			}

			// operator== && !=
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);

				Assert::AreEqual(hashMap.cbegin(), hashMap.cbegin());
				Assert::AreEqual(hashMap.cend(), hashMap.cend());
				Assert::IsTrue(hashMap.cbegin() != hashMap.cend());

				Assert::AreEqual(HashMap<Foo, Foo>::const_iterator{}, HashMap<Foo, Foo>::const_iterator{});
				Assert::AreNotEqual(HashMap<Foo, Foo>::const_iterator{}, hashMap.cbegin());
				Assert::AreNotEqual(HashMap<Foo, Foo>::const_iterator{}, hashMap.cend());
				Assert::IsTrue(HashMap<Foo, Foo>::const_iterator{} != hashMap.cbegin());
				Assert::IsTrue(HashMap<Foo, Foo>::const_iterator{} != hashMap.cend());

				// Another hashmap
				HashMap<Foo, Foo> hashMap2(hashMap);
				Assert::AreNotEqual(hashMap2.cend(), hashMap.cend());
				Assert::IsTrue(hashMap2.cend() != hashMap.cend());
			}
		}

		TEST_METHOD(iterator)
		{
			// operator*
			{
				HashMap<Foo, Foo>::PairType pair(Foo(1), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair);
				HashMap<Foo, Foo>::iterator it = hashMap.begin();
				Assert::AreEqual(pair, *it);
				Assert::AreEqual(pair.first, it->first);
				++it;
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { const auto& pair = *defaultIt; UNREFERENCED_LOCAL(pair); });
			}

			// ++operator
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				const HashMap<Foo, Foo>::PairType pair2(Foo(2), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);
				hashMap.Insert(pair2);
				HashMap<Foo, Foo>::iterator oldIt;
				HashMap<Foo, Foo>::iterator it = hashMap.begin();

				Assert::AreNotEqual(hashMap.end(), it);
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::AreNotEqual(hashMap.end(), it);
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });
				// Iterate past end
				oldIt = ++it;
				Assert::AreEqual(oldIt, it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { ++defaultIt; });
			}

			// operator++
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				const HashMap<Foo, Foo>::PairType pair2(Foo(2), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);
				hashMap.Insert(pair2);
				HashMap<Foo, Foo>::iterator oldIt;
				HashMap<Foo, Foo>::iterator it = hashMap.begin();

				Assert::AreNotEqual(hashMap.end(), it);
				oldIt = it++;
				Assert::AreNotEqual(oldIt, it);
				Assert::AreNotEqual(hashMap.end(), it);
				Assert::AreEqual(hashMap.begin(), oldIt);
				oldIt = it++;
				Assert::AreNotEqual(oldIt, it);
				Assert::AreEqual(hashMap.end(), it);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });
				// Iterate past end
				oldIt = it++;
				Assert::AreEqual(oldIt, it);
				Assert::AreEqual(hashMap.end(), it);
				Assert::AreEqual(hashMap.end(), oldIt);
				Assert::ExpectException<runtime_error>([&it]() { const auto& pair = *it; UNREFERENCED_LOCAL(pair); });

				// Default constructed
				HashMap<Foo, Foo>::iterator defaultIt;
				Assert::ExpectException<runtime_error>([&defaultIt]() { ++defaultIt; });
			}

			// operator== && !=
			{
				const HashMap<Foo, Foo>::PairType pair1(Foo(1), Foo(2));
				HashMap<Foo, Foo> hashMap;
				hashMap.Insert(pair1);

				Assert::AreEqual(hashMap.begin(), hashMap.begin());
				Assert::AreEqual(hashMap.end(), hashMap.end());
				Assert::IsTrue(hashMap.begin() != hashMap.end());

				Assert::AreEqual(HashMap<Foo, Foo>::iterator{}, HashMap<Foo, Foo>::iterator{});
				Assert::AreNotEqual(HashMap<Foo, Foo>::iterator{}, hashMap.begin());
				Assert::AreNotEqual(HashMap<Foo, Foo>::iterator{}, hashMap.end());
				Assert::IsTrue(HashMap<Foo, Foo>::iterator{} != hashMap.begin());
				Assert::IsTrue(HashMap<Foo, Foo>::iterator{} != hashMap.end());

				// Another hashmap
				HashMap<Foo, Foo> hashMap2(hashMap);
				Assert::AreNotEqual(hashMap2.end(), hashMap.end());
				Assert::IsTrue(hashMap2.end() != hashMap.end());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}