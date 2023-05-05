#include "pch.h"

#include <initializer_list>
#include "Literals.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "Vector.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace LibraryDesktopTests
{
	TEST_CLASS(DatumTests)
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
			{
				const Datum datum(DatumTypes::Unknown);
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				Assert::AreEqual(size_t(0), datum.Capacity());
			}

			{
				const Datum datum(DatumTypes::Integer);
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(size_t(0), datum.Capacity());
			}

			// Integer
			{
				const int32_t data = 1;
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<int32_t>());
				Assert::AreNotSame(data, datum.Get<int32_t>());
			}

			// Float
			{
				const float data = 1.0f;
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<float>());
				Assert::AreNotSame(data, datum.Get<float>());
			}

			// String
			{
				const string data = "Test"s;
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<string>(0));
				Assert::AreNotSame(data, datum.Get<string>(0));
			}

			// Vec4
			{
				const glm::vec4 data(1, 2, 3, 4);
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<glm::vec4>(0));
				Assert::AreNotSame(data, datum.Get<glm::vec4>(0));
			}

			// Mat4
			{
				const glm::mat4 data{1.0f};
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<glm::mat4>(0));
				Assert::AreNotSame(data, datum.Get<glm::mat4>(0));
			}

			// RTTI Pointer
			{
				Foo foo;
				RTTI* data = &foo;
				const Datum datum = data;
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(data, datum.Get<RTTI*>(0));
				Assert::AreNotSame(data, datum.Get<RTTI*>(0));
			}
		}

		TEST_METHOD(InitializerList)
		{
			// Integer
			{
				Datum datum = { 1, 2, 3, 4, 5 };
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
			}

			// Float
			{
				Datum datum = { 1, 2, 3};
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(1, datum.Get<std::int32_t>(0));
				Assert::AreEqual(2, datum.Get<std::int32_t>(1));
				Assert::AreEqual(3, datum.Get<std::int32_t>(2));
			}

			// String
			{
				Datum datum = { "test1"s, "test2"s, "test3"s };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual("test1"s, datum.Get<string>(0));
				Assert::AreEqual("test2"s, datum.Get<string>(1));
				Assert::AreEqual("test3"s, datum.Get<string>(2));
			}

			// Vec4
			{
				Datum datum = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(glm::vec4(1), datum.Get<glm::vec4>(0));
				Assert::AreEqual(glm::vec4(2), datum.Get<glm::vec4>(1));
				Assert::AreEqual(glm::vec4(3), datum.Get<glm::vec4>(2));
			}

			// Mat4
			{
				Datum datum = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(glm::mat4(1), datum.Get<glm::mat4>(0));
				Assert::AreEqual(glm::mat4(2), datum.Get<glm::mat4>(1));
				Assert::AreEqual(glm::mat4(3), datum.Get<glm::mat4>(2));
			}

			// RTTI Pointer
			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				RTTI* rttiA = &a;
				RTTI* rttiB = &b;
				RTTI* rttiC = &c;
				Datum datum = { rttiA, rttiB, rttiC };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(rttiA, datum.Get<RTTI*>(0));
				Assert::AreEqual(rttiB, datum.Get<RTTI*>(1));
				Assert::AreEqual(rttiC, datum.Get<RTTI*>(2));
			}
		}

		TEST_METHOD(CopySemantics)
		{
			// Integer
			{
				const int32_t data = 1;
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::Integer, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<int32_t>(), datumCopy.Get<int32_t>());
				Assert::AreNotSame(datum.Get<int32_t>(), datumCopy.Get<int32_t>());
			}

			// Float
			{
				const float data = 1.0f;
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::Float, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<float>(), datumCopy.Get<float>());
				Assert::AreNotSame(datum.Get<float>(), datumCopy.Get<float>());
			}

			// String
			{
				const string data = "test"s;
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::String, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<std::string>(), datumCopy.Get<std::string>());
				Assert::AreNotSame(datum.Get<std::string>(), datumCopy.Get<std::string>());
			}

			// Vec4
			{
				const glm::vec4 data(1, 1, 1, 1);
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::Vector, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<glm::vec4>(), datumCopy.Get<glm::vec4>());
				Assert::AreNotSame(datum.Get<glm::vec4>(), datumCopy.Get<glm::vec4>());
			}

			// Mat4
			{
				const glm::mat4 data(1.0f);
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::Matrix, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<glm::mat4>(), datumCopy.Get<glm::mat4>());
				Assert::AreNotSame(datum.Get<glm::mat4>(), datumCopy.Get<glm::mat4>());
			}

			// RTTI Pointer
			{
				Foo foo(1);
				RTTI* data = &foo;
				const Datum datum = data;
				const Datum datumCopy = datum;

				Assert::AreEqual(DatumTypes::Pointer, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(datum.Get<RTTI*>(), datumCopy.Get<RTTI*>());
				Assert::AreNotSame(datum.Get<RTTI*>(), datumCopy.Get<RTTI*>());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Integer
			{
				const int32_t data = 1;
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::Integer, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<int32_t>());
				Assert::AreNotSame(data, datumCopy.Get<int32_t>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}

			// Float
			{
				const float data = 1.0f;
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::Float, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<float>());
				Assert::AreNotSame(data, datumCopy.Get<float>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}

			// String
			{
				const string data = "test"s;
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::String, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<std::string>());
				Assert::AreNotSame(data, datumCopy.Get<std::string>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}

			// Vec4
			{
				const glm::vec4 data(1, 1, 1, 1);
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::Vector, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<glm::vec4>());
				Assert::AreNotSame(data, datumCopy.Get<glm::vec4>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}

			// Mat4
			{
				const glm::mat4 data(1.0f);
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::Matrix, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<glm::mat4>());
				Assert::AreNotSame(data, datumCopy.Get<glm::mat4>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}

			// RTTI Pointer
			{
				Foo foo(1);
				RTTI* data = &foo;
				Datum datum = data;
				const Datum datumCopy = std::move(datum);

				Assert::AreEqual(DatumTypes::Pointer, datumCopy.Type());
				Assert::AreEqual(size_t(1), datumCopy.Size());
				Assert::AreEqual(data, datumCopy.Get<RTTI*>());
				Assert::AreNotSame(data, datumCopy.Get<RTTI*>());

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
#pragma warning(pop)
			}
		}

		TEST_METHOD(Assignment)
		{
			// Integer
			{
				Datum datum1 = {1, 2};
				const Datum datum2 = { 3 };
				Assert::AreEqual(DatumTypes::Integer, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::Integer, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(3, datum1.Get<int32_t>());
			}

			// Float
			{
				Datum datum1 = {1.0f, 2.0f};
				const Datum datum2 = { 3.0f };
				Assert::AreEqual(DatumTypes::Float, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::Float, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(3.0f, datum1.Get<float>());
			}

			// String
			{
				Datum datum1 = {"test1"s, "test2"s};
				const Datum datum2 = { "test3"s };
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual("test3"s, datum1.Get<std::string>());
			}

			// Vec4
			{
				Datum datum1 = {glm::vec4(1), glm::vec4(2)};
				const Datum datum2 = { glm::vec4(3) };
				Assert::AreEqual(DatumTypes::Vector, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::Vector, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(glm::vec4(3), datum1.Get<glm::vec4>());
			}

			// Mat4
			{
				Datum datum1 = { glm::mat4(1), glm::mat4(2) };
				const Datum datum2 = { glm::mat4(3) };
				Assert::AreEqual(DatumTypes::Matrix, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::Matrix, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(glm::mat4(3), datum1.Get<glm::mat4>());
			}

			// RTTI Pointer
			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				RTTI* rttiA = &a;
				RTTI* rttiB = &b;
				RTTI* rttiC = &c;
				Datum datum1 = {rttiA, rttiB};
				const Datum datum2 = { rttiC };
				Assert::AreEqual(DatumTypes::Pointer, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::Pointer, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(rttiC, datum1.Get<RTTI*>());
			} 

			// Different types from Non-external
			{
				Datum datum1 = {1, 2};
				const Datum datum2 = { "test3"s };
				Assert::AreEqual(DatumTypes::Integer, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual("test3"s, datum1.Get<std::string>());
			}

			// Replace non external with external
			{
				Datum datum1 = {1, 2};
				std::string external[] = {"test3"s};
				Datum datum2(external, 1);
				Assert::AreEqual(DatumTypes::Integer, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(size_t(1), datum1.Capacity());
				Assert::AreEqual("test3"s, datum1.Get<std::string>());
			}

			// Replace external with external
			{
				std::int32_t external1[] = {1, 2};
				std::string external2[] = {"test3"s};
				Datum datum1(external1, 2);
				Datum datum2(external2, 1);
				Assert::AreEqual(DatumTypes::Integer, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				Assert::AreEqual(size_t(2), datum1.Capacity());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual(size_t(1), datum1.Capacity());
				Assert::AreEqual("test3"s, datum1.Get<std::string>());
			}

			// Replace external with non-external
			{
				std::string external[] = {"test1"s, "test2"s};
				Datum datum1(external, 2);
				Datum datum2 = {"test3"s};
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(2), datum1.Size());
				datum1 = datum2;
				Assert::AreEqual(DatumTypes::String, datum1.Type());
				Assert::AreEqual(size_t(1), datum1.Size());
				Assert::AreEqual("test3"s, datum1.Get<std::string>());
			}
		}

		TEST_METHOD(Type)
		{
			// Integer
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::Integer);
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
			}

			// Float
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::Float);
				Assert::AreEqual(DatumTypes::Float, datum.Type());
			}

			// String
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::String);
				Assert::AreEqual(DatumTypes::String, datum.Type());
			}

			// Vec4
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::Vector);
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
			}

			// Mat4
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::Matrix);
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			}

			// RTTI Pointer
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				datum.SetType(DatumTypes::Pointer);
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			}

			// Setting type to unknown from another type
			{
				Datum datum;
				datum.SetType(DatumTypes::Integer);
				Assert::ExpectException<std::runtime_error>([&datum]() { datum.SetType(DatumTypes::Unknown); });
			}

			// Changing type after already set throws
			{
				Datum datum = 1;
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::ExpectException<std::runtime_error>([&datum]() { datum.SetType(DatumTypes::Float); });
			}
		}

		TEST_METHOD(Size)
		{
			Datum datum;
			Assert::AreEqual(size_t(0), datum.Size());
			datum.PushBack(1);
			Assert::AreEqual(size_t(1), datum.Size());
			datum.PushBack(5);
			Assert::AreEqual(size_t(2), datum.Size());

			datum.Clear();
			Assert::AreEqual(size_t(0), datum.Size());
		}

		TEST_METHOD(Resize)
		{
			// Unknown
			{
				Datum datum(DatumTypes::Unknown);
				Assert::ExpectException<std::runtime_error>([&datum]() { datum.Resize(10); });
			}

			// Integer
			{
				Datum datum(DatumTypes::Integer);
				std::size_t resizeAmount1 = 10;
				int32_t defaultData = int32_t{};
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::int32_t>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::int32_t>(i));
				}

				// Resize less
				std::size_t resizeAmount2 = 5;

				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::int32_t>(i));
				}
			}

			// Float
			{
				Datum datum(DatumTypes::Float);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				float defaultData = float{};
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<float>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<float>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<float>(i));
				}
			}

			// String
			{
				Datum datum(DatumTypes::String);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				string defaultData = string{};
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::string>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::string>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<std::string>(i));
				}
			}

			// Vec4
			{
				Datum datum(DatumTypes::Vector);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				glm::vec4 defaultData = glm::vec4{};
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::vec4>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::vec4>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::vec4>(i));
				}
			}

			// Mat4
			{
				Datum datum(DatumTypes::Matrix);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				glm::mat4 defaultData = glm::mat4{};
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::mat4>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::mat4>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<glm::mat4>(i));
				}
			}

			// RTTI Pointer
			{
				Datum datum(DatumTypes::Pointer);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				RTTI* defaultData = nullptr;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<RTTI*>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<RTTI*>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(defaultData, datum.Get<RTTI*>(i));
				}
			}
		}

		TEST_METHOD(ResizeWithPrototype)
		{
			// Unknown
			{
				Datum datum(DatumTypes::Unknown);
				Assert::ExpectException<std::runtime_error>([&datum]() { datum.Resize(10, 1); });
			}

			// Integer
			{
				Datum datum(DatumTypes::Integer);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				int32_t proto = 2;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::int32_t>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::int32_t>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::int32_t>(i));
				}
			}

			// Float
			{
				Datum datum(DatumTypes::Float);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				float proto = 4.0f;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<float>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<float>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<float>(i));
				}
			}

			// String
			{
				Datum datum(DatumTypes::String);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				string proto = "test"s;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::string>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::string>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<std::string>(i));
				}
			}

			// Vec4
			{
				Datum datum(DatumTypes::Vector);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				glm::vec4 proto(1, 2, 3, 4);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::vec4>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::vec4>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::vec4>(i));
				}
			}

			// Mat4
			{
				Datum datum(DatumTypes::Matrix);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				glm::mat4 proto(6);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::mat4>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::mat4>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<glm::mat4>(i));
				}
			}

			// RTTI Pointer
			{
				Datum datum(DatumTypes::Pointer);
				std::size_t resizeAmount1 = 10;
				std::size_t resizeAmount2 = 5;
				Foo foo(1);
				RTTI* proto = &foo;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				// Resize more
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<RTTI*>(i));
				}

				// Resize same
				datum.Resize(resizeAmount1, proto);
				Assert::AreEqual(size_t(resizeAmount1), datum.Size());
				Assert::AreEqual(size_t(resizeAmount1), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount1; ++i)
				{
					Assert::AreEqual(proto, datum.Get<RTTI*>(i));
				}

				// Resize less
				datum.Resize(resizeAmount2, proto);
				Assert::AreEqual(size_t(resizeAmount2), datum.Size());
				Assert::AreEqual(size_t(resizeAmount2), datum.Capacity());
				for (std::size_t i = 0; i < resizeAmount2; ++i)
				{
					Assert::AreEqual(proto, datum.Get<RTTI*>(i));
				}
			}
		}

		TEST_METHOD(Clear)
		{
			// Unknown
			{
				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
			}

			// Integer
			{
				Datum datum = {1, 2};
				Assert::AreEqual(DatumTypes::Integer, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			// Float
			{
				Datum datum = {1.0f, 2.0f};
				Assert::AreEqual(DatumTypes::Float, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			// String
			{
				Datum datum = {"test1"s, "test2"s};
				Assert::AreEqual(DatumTypes::String, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			// Vec4
			{
				Datum datum = {glm::vec4(1), glm::vec4(2)};
				Assert::AreEqual(DatumTypes::Vector, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			// Mat4
			{
				Datum datum = {glm::mat4(1), glm::mat4(2)};
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			// Pointer
			{
				Foo a(1);
				Foo b(2);
				RTTI* rttiA = &a;
				RTTI* rttiB = &b;
				Datum datum = {rttiA, rttiB};
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}
		}

		template <typename T, typename U>
		inline void _SetStorageTest(initializer_list<T> list, initializer_list<U> differentList, DatumTypes expectedTypeAfterSetting)
		{
			{
				std::vector<T> external = list;
				std::vector<T> external2 = list;

				std::vector<U> differentTypeExternal = differentList;

				Datum datum;
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				datum.SetStorage(external.data(), list.size());
				Assert::AreEqual(expectedTypeAfterSetting, datum.Type());
				Assert::AreEqual(list.size(), datum.Size());
				Assert::AreEqual(list.size(), datum.Capacity());

				datum.SetStorage(external2.data(), list.size());
				Assert::AreEqual(expectedTypeAfterSetting, datum.Type());
				Assert::AreEqual(list.size(), datum.Size());
				Assert::AreEqual(list.size(), datum.Capacity());

				Assert::ExpectException<runtime_error>([&datum, &differentTypeExternal, &differentList]() { datum.SetStorage(differentTypeExternal.data(), differentList.size()); });
				
			}

			// Internal to external
			{
				std::vector<T> external = list;
				Datum datum = list;
				Assert::ExpectException<runtime_error>([&datum, &external, &list]() { datum.SetStorage(external.data(), list.size()); });
			}
		}

		TEST_METHOD(SetStorage)
		{
			_SetStorageTest<std::int32_t, float>({ 1, 2, 3, 4 }, { 1.0f, 2.0f, 3.0f, 4.0f }, DatumTypes::Integer);
			_SetStorageTest<float, std::int32_t>({ 1.0f, 2.0f, 3.0f, 4.0f }, { 1, 2, 3, 4 }, DatumTypes::Float);
			_SetStorageTest<std::string, std::int32_t>({ "1"s, "2"s, "3"s, "4"s }, { 1, 2, 3, 4 }, DatumTypes::String);
			_SetStorageTest<glm::vec4, std::int32_t>({ vec4(1), vec4(2), vec4(3), vec4(4) }, { 1, 2, 3, 4 }, DatumTypes::Vector);
			_SetStorageTest<glm::mat4, std::int32_t>({ mat4(1), mat4(2), mat4(3), mat4(4) }, { 1, 2, 3, 4 }, DatumTypes::Matrix);
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Foo e(10);
			_SetStorageTest<RTTI*, std::int32_t>({
				reinterpret_cast<RTTI*>(&a),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&d) }, { 1, 2, 3, 4 }, DatumTypes::Pointer);
		}

		template <typename T, typename U>
		inline void _OperatorComparisonsTest(initializer_list<T> list, initializer_list<T> differentList, initializer_list<U> differentType)
		{
			Datum datum = list;
			Datum datum2 = list;
			Datum datum3 = differentList;
			Datum datum4 = differentType;
			Assert::IsTrue(datum == datum2);

			Assert::IsTrue(datum != datum3);

			Assert::IsTrue(datum != datum4);
		}

		TEST_METHOD(OperatorComparisons)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			_OperatorComparisonsTest<int32_t, float>({ 1, 2, 3, 4 }, { 4, 3, 2, 1 }, { 1.0f, 2.0f, 3.0f, 4.0f });
			_OperatorComparisonsTest<float, int32_t>({ 1, 2, 3, 4 }, { 4, 3, 2, 1 }, { 1, 2, 3, 4 });
			_OperatorComparisonsTest<string, float>({ "test1"s, "test2"s, "test3"s, "test4"s}, {"test4"s, "test3"s, "test2"s, "test1"s}, { 1.0f, 2.0f, 3.0f, 4.0f });
			_OperatorComparisonsTest<vec4, float>({ vec4(1), vec4(2), vec4(3), vec4(4) }, { vec4(4), vec4(3), vec4(2), vec4(1) }, { 1.0f, 2.0f, 3.0f, 4.0f });
			_OperatorComparisonsTest<mat4, float>({ mat4(1), mat4(2), mat4(3), mat4(4) }, { mat4(4), mat4(3), mat4(2), mat4(1) }, { 1.0f, 2.0f, 3.0f, 4.0f });
			_OperatorComparisonsTest<RTTI*, float>({
				reinterpret_cast<RTTI*>(&a),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&d) }, {
				reinterpret_cast<RTTI*>(&d),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&a) }, {1.0f, 2.0f, 3.0f, 4.0f});
		}

		template <typename T, typename U>
		inline void _SetTest(initializer_list<T> list, const T& setValue, const U& differentValue, size_t index)
		{
			// LValue
			{
				Datum datum = list;
				Assert::AreEqual(*(list.begin() + index), datum.Get<T>(index));
				datum.Set<T>(setValue, index);
				Assert::AreEqual(setValue, datum.Get<T>(index));
				Assert::AreNotSame(setValue, datum.Get<T>(index));

				// Out of bounds
				Assert::ExpectException<runtime_error>([&datum, &setValue, &list]() { datum.Set<T>(setValue, list.size()); });
			
				// Different type
				Assert::ExpectException<runtime_error>([&datum, &differentValue, &list]() { datum.Set<U>(differentValue, 0); });
			}

			// Rvalue
			{
				Datum datum = list;
				T setValueCopy = setValue;
				Assert::AreEqual(*(list.begin() + index), datum.Get<T>(index));
				datum.Set<T>(std::move(setValueCopy), index);
				Assert::AreEqual(setValue, datum.Get<T>(index));
				Assert::AreNotSame(setValue, datum.Get<T>(index));

				// Out of bounds
				T setValueCopy2 = setValue;
				Assert::ExpectException<runtime_error>([&datum, &setValueCopy2, &list]() { datum.Set<T>(std::move(setValueCopy2), list.size()); });
			
				// Different type
				U setDifferentValueCopy = differentValue;
				Assert::ExpectException<runtime_error>([&datum, &setDifferentValueCopy, &list]() { datum.Set<U>(std::move(setDifferentValueCopy), 0); });
			}
		}

		template <typename T, typename U>
		inline void _SetTestDifferentType(initializer_list<T> list, const U& setValue, size_t index)
		{
			Datum datum = list;
			Assert::AreEqual(*(list.begin() + index), datum.Get<T>(index));
			Assert::ExpectException<runtime_error>([&datum, &setValue, &index]() { datum.Set<U>(setValue, index); });
		}

		template <typename T>
		inline void _SetTestExternal(T* external, size_t externalSize, const T& setValue, size_t index)
		{
			Datum datum(external, externalSize);
			Assert::AreEqual(external[index], datum.Get<T>(index));
			datum.Set<T>(setValue, index);
			Assert::AreEqual(setValue, datum.Get<T>(index));
			Assert::AreNotSame(setValue, datum.Get<T>(index));
		}

		TEST_METHOD(Set)
		{
			// Non external
			{
				_SetTest<int32_t, float>({ 1, 2, 3, 4 }, 10, 10.f, 1);
				_SetTest<float, int32_t>({ 1.0f, 2.0f, 3.0f, 4.0f }, 10.0f, 10, 1);
				_SetTest<string, int32_t>({ "1"s, "2"s, "3"s, "4"s }, "10"s, 10, 1);
				_SetTest<vec4, int32_t>({ vec4(1), vec4(2), vec4(3), vec4(4) }, vec4(10), 10, 1);
				_SetTest<mat4, int32_t>({ mat4(1), mat4(2), mat4(3), mat4(4) }, mat4(10), 10, 1);
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);
				Foo e(10);
				_SetTest<RTTI*>({
					reinterpret_cast<RTTI*>(&a),
					reinterpret_cast<RTTI*>(&b),
					reinterpret_cast<RTTI*>(&c),
					reinterpret_cast<RTTI*>(&d) }, reinterpret_cast<RTTI*>(&e), 10, 1);
			}

			// External
			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);
				Foo e(10);
				int32_t external1[] = {1, 2, 3, 4};
				float external2[] = { 1.0f, 2.0f, 3.0f, 4.0f };
				string external3[] = { "1"s, "2"s, "3"s, "4"s};
				vec4 external4[] = { vec4(1), vec4(2), vec4(3), vec4(4) };
				mat4 external5[] = { mat4(1), mat4(2), mat4(3), mat4(4) };
				RTTI* external6[] = {
					reinterpret_cast<RTTI*>(&a),
					reinterpret_cast<RTTI*>(&b),
					reinterpret_cast<RTTI*>(&c),
					reinterpret_cast<RTTI*>(&d)};
				_SetTestExternal<int32_t>(external1, 4, 10, 1);
				_SetTestExternal<float>(external2, 4, 10.0f, 1);
				_SetTestExternal<string>(external3, 4, "10"s, 1);
				_SetTestExternal<vec4>(external4, 4, vec4(10), 1);
				_SetTestExternal<mat4>(external5, 4, mat4(10), 1);
				_SetTestExternal<RTTI*>(external6, 4, reinterpret_cast<RTTI*>(&e), 1);
			}

			// Set different type
			{
				_SetTestDifferentType<std::int32_t>({ 1, 2, 3, 4 }, 10.0f, 1);
				_SetTestDifferentType<float>({ 1.0f, 2.0f, 3.0f, 4.0f }, 1, 1);
				_SetTestDifferentType<std::string>({ "1"s, "2"s, "3"s, "4"s }, 1, 1);
				_SetTestDifferentType<glm::vec4>({ vec4(1), vec4(2), vec4(3), vec4(4) }, 1, 1);
				_SetTestDifferentType<glm::mat4>({ mat4(1), mat4(2), mat4(3), mat4(4) }, 1, 1);
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);
				Foo e(10);
				_SetTestDifferentType<RTTI*>({
					reinterpret_cast<RTTI*>(&a),
					reinterpret_cast<RTTI*>(&b),
					reinterpret_cast<RTTI*>(&c),
					reinterpret_cast<RTTI*>(&d) }, 1, 1);
			}
		}

		template <typename T, typename U>
		inline void _GetTest(initializer_list<T> list, size_t index)
		{
			// Normal test
			{
				Datum datum = list;
				Assert::AreEqual(*(list.begin() + index), datum.Get<T>(index));
			}

			// Out of bounds
			{
				Datum datum = list;
				Assert::ExpectException<runtime_error>([&datum, &list]() { T& value = datum.Get<T>(list.size()); UNREFERENCED_LOCAL(value); });
			}

			// Different type
			{
				Datum datum = list;
				Assert::ExpectException<runtime_error>([&datum, &index]() { U& value = datum.Get<U>(index); UNREFERENCED_LOCAL(value); });
			}

			// Const

			// Normal test
			{
				const Datum datum = list;
				Assert::AreEqual(*(list.begin() + index), datum.Get<T>(index));
			}

			// Out of bounds
			{
				const Datum datum = list;
				Assert::ExpectException<runtime_error>([&datum, &list]() { const T& value = datum.Get<T>(list.size()); UNREFERENCED_LOCAL(value); });
			}

			// Different type
			{
				const Datum datum = list;
				Assert::ExpectException<runtime_error>([&datum, &index]() { const U& value = datum.Get<U>(index); UNREFERENCED_LOCAL(value); });
			}
		}
		TEST_METHOD(Get)
		{
			_GetTest<int32_t, float>({ 1, 2, 3, 4 }, 1);
			_GetTest<float, int32_t>({ 1.0f, 2.0f, 3.0f, 4.0f }, 1);
			_GetTest<std::string, int32_t>({ "1"s, "2"s, "3"s, "4"s }, 1);
			_GetTest<glm::vec4, int32_t>({ vec4(1), vec4(2), vec4(3), vec4(4) }, 1);
			_GetTest<glm::mat4, int32_t>({ mat4(1), mat4(2), mat4(3), mat4(4) }, 1);
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Foo e(10);
			_GetTest<RTTI*, int32_t>({
				reinterpret_cast<RTTI*>(&a),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&d) }, 1);
		}

		template <typename T, typename U>
		inline void _PushBackTest(const T& value, U otherValue)
		{
			// Const reference
			{
				Datum datum;
				Assert::AreEqual(size_t(0), datum.Size());

				datum.PushBack(value);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(value, datum.Get<T>(0));
				Assert::AreNotSame(value, datum.Get<T>(0));

				datum.PushBack(value);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(value, datum.Get<T>(0));
				Assert::AreEqual(value, datum.Get<T>(1));
				Assert::AreNotSame(value, datum.Get<T>(0));
				Assert::AreNotSame(value, datum.Get<T>(1));

				Assert::ExpectException<runtime_error>([&datum, &otherValue]() { datum.PushBack<U>(otherValue); });
			}

			// R value reference
			{
				Datum datum;
				Assert::AreEqual(size_t(0), datum.Size());

				const T valueCopy = value;
				datum.PushBack(std::move(value));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(valueCopy, datum.Get<T>(0));
				Assert::AreNotSame(valueCopy, datum.Get<T>(0));

				Assert::ExpectException<runtime_error>([&datum, &otherValue]() { datum.PushBack<U>(std::move(otherValue)); });
			}
		}
		TEST_METHOD(PushBack)
		{
			Foo a(1);
			_PushBackTest<int32_t, float>(1, 1.0f);
			_PushBackTest<float, int>(1.0f, 1);
			_PushBackTest<string, float>("test"s, 1.0f);
			_PushBackTest<vec4, float>(vec4(1), 1.0f);
			_PushBackTest<mat4, float>(mat4(1), 1.0f);
			_PushBackTest<RTTI*, float>(reinterpret_cast<RTTI*>(&a), 1.0f);
		}

		template <typename T>
		inline void _PopBackTest(const T value)
		{
			Datum datum = value;
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(value, datum.Get<T>());
			
			datum.PopBack();
			Assert::AreEqual(size_t(0), datum.Size());

			Assert::ExpectException<runtime_error>([&datum]() { datum.PopBack(); });
		}
		TEST_METHOD(PopBack)
		{
			Foo a(1);
			_PopBackTest<int32_t>(1);
			_PopBackTest<float>(1.0f);
			_PopBackTest<string>("test"s);
			_PopBackTest<vec4>(vec4(1));
			_PopBackTest<mat4>(mat4(1));
			_PopBackTest<RTTI*>(reinterpret_cast<RTTI*>(&a));
		}

		template <typename T, typename U>
		inline void _FrontBackTest(initializer_list<T> list)
		{
			// Non-const
			{
				Datum datum = list;
				Assert::AreEqual(list.size(), datum.Size());
				Assert::AreEqual(*list.begin(), datum.Front<T>());
				Assert::AreEqual(*(list.end() - 1), datum.Back<T>());

				Assert::ExpectException<runtime_error>([&datum]() { const U& data = datum.Front<U>(); UNREFERENCED_LOCAL(data); });
				Assert::ExpectException<runtime_error>([&datum]() { const U& data = datum.Back<U>(); UNREFERENCED_LOCAL(data); });

				datum.Clear();

				Assert::ExpectException<runtime_error>([&datum]() { const T& data = datum.Front<T>(); UNREFERENCED_LOCAL(data); });
				Assert::ExpectException<runtime_error>([&datum]() { const T& data = datum.Back<T>(); UNREFERENCED_LOCAL(data); });

				// Const with 0 delements
				Assert::ExpectException<runtime_error>([&datum]() { const T& data = const_cast<const Datum&>(datum).Front<T>(); UNREFERENCED_LOCAL(data); });
				Assert::ExpectException<runtime_error>([&datum]() { const T& data = const_cast<const Datum&>(datum).Back<T>(); UNREFERENCED_LOCAL(data); });
			}

			// Const
			{
				const Datum datum = list;
				Assert::AreEqual(list.size(), datum.Size());
				Assert::AreEqual(*list.begin(), datum.Front<T>());
				Assert::AreEqual(*(list.end() - 1), datum.Back<T>());

				Assert::ExpectException<runtime_error>([&datum]() { const U& data = datum.Front<U>(); UNREFERENCED_LOCAL(data); });
				Assert::ExpectException<runtime_error>([&datum]() { const U& data = datum.Back<U>(); UNREFERENCED_LOCAL(data); });
			}
		}

		TEST_METHOD(FrontBack)
		{
			_FrontBackTest<int32_t, float>({ 1, 2, 3, 4 });
			_FrontBackTest<float, int32_t>({ 1.0f, 2.0f, 3.0f, 4.0f });
			_FrontBackTest<std::string, int32_t>({ "1"s, "2"s, "3"s, "4"s });
			_FrontBackTest<glm::vec4, int32_t>({ vec4(1), vec4(2), vec4(3), vec4(4) });
			_FrontBackTest<glm::mat4, int32_t>({ mat4(1), mat4(2), mat4(3), mat4(4) });
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Foo e(10);
			_FrontBackTest<RTTI*, int32_t>({
				reinterpret_cast<RTTI*>(&a),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&d) });
		}

		template <typename T, typename U>
		inline void _RemoveTest(initializer_list<T> list, const T& valueToRemove, const T& valueNotThere, size_t indexToRemove, const U& badValueToRemove)
		{
			{
				Datum datum = list;
				Assert::AreEqual(list.size(), datum.Size());

				bool removed = datum.Remove(valueToRemove);
				Assert::IsTrue(removed);
				Assert::AreEqual(list.size() - 1, datum.Size());

				bool removed2 = datum.Remove(valueNotThere);
				Assert::IsFalse(removed2);
				Assert::AreEqual(list.size() - 1, datum.Size());

				bool removed3 = datum.RemoveAt(indexToRemove);
				Assert::IsTrue(removed3);
				Assert::AreEqual(list.size() - 2, datum.Size());

				// Out of bounds returns false
				Assert::IsFalse(datum.RemoveAt(datum.Size()));

				Assert::ExpectException<runtime_error>([&datum, &badValueToRemove]() { datum.Remove<U>(badValueToRemove); });
			}

			{
				// External
				std::vector<T> external = list;
				Datum datum(external.data(), list.size());
				Assert::ExpectException<runtime_error>([&datum, &valueToRemove]() { datum.Remove<T>(valueToRemove); });
			}
			
		}

		TEST_METHOD(Remove)
		{
			_RemoveTest<int32_t, float>({ 1, 2, 3, 4 }, 2, 5, 0, 10.f);
			_RemoveTest<float, int32_t>({ 1.0f, 2.0f, 3.0f, 4.0f }, 2.0f, 5.0, 0, 1);
			_RemoveTest<std::string, int32_t>({ "1"s, "2"s, "3"s, "4"s }, "2"s, "5"s, 0, 1);
			_RemoveTest<glm::vec4, int32_t>({ vec4(1), vec4(2), vec4(3), vec4(4) }, vec4(2), vec4(5), 0, 1);
			_RemoveTest<glm::mat4, int32_t>({ mat4(1), mat4(2), mat4(3), mat4(4) }, mat4(2), mat4(5), 0, 1);
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Foo e(10);
			_RemoveTest<RTTI*, int32_t>({
				reinterpret_cast<RTTI*>(&a),
				reinterpret_cast<RTTI*>(&b),
				reinterpret_cast<RTTI*>(&c),
				reinterpret_cast<RTTI*>(&d) }, reinterpret_cast<RTTI*>(&b), reinterpret_cast<RTTI*>(&e), 0, 1);
		}
		
	private:
		inline static _CrtMemState _startMemState;
	};
}