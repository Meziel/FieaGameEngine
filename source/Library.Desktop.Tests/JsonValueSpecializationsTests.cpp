#include "pch.h"
#include "Literals.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include "JsonValueSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonValueSpecializationsTests)
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

		TEST_METHOD(JsonValueToInt32)
		{
			const string inputString = R"({
				"var": 1,
			})";

			istringstream iss(inputString);
			Json::Value root;
			iss >> root;

			Assert::IsTrue(root["var"].isInt());
			Assert::AreEqual(1, FieaGameEngine::FromJsonValue<int32_t>(root["var"]));
		}

		TEST_METHOD(JsonValueToFloat)
		{
			const string inputString = R"({
				"var": 1.0,
			})";

			istringstream iss(inputString);
			Json::Value root;
			iss >> root;

			Assert::IsTrue(root["var"].isNumeric());
			Assert::AreEqual(1.0f, FieaGameEngine::FromJsonValue<float>(root["var"]));
		}

		TEST_METHOD(JsonValueToString)
		{
			const string inputString = R"({
				"var": "test",
			})";

			istringstream iss(inputString);
			Json::Value root;
			iss >> root;

			Assert::IsTrue(root["var"].isString());
			Assert::AreEqual("test"s, FieaGameEngine::FromJsonValue<string>(root["var"]));
		}

		TEST_METHOD(JsonValueToVector)
		{
			const string inputString = R"({
				"var": [1, 2, 3, 4],
			})";

			istringstream iss(inputString);
			Json::Value root;
			iss >> root;

			Assert::IsTrue(root["var"].isArray());
			Assert::AreEqual(glm::vec4(1,2,3,4), FieaGameEngine::FromJsonValue<glm::vec4>(root["var"]));
		}

		TEST_METHOD(JsonValueToMatrix)
		{
			const string inputString = R"({
				"var": [
					[01, 02, 03, 04],
					[05, 06, 07, 08],
                    [09, 10, 11, 12],
					[13, 14, 15, 16]
				],
			})";

			istringstream iss(inputString);
			Json::Value root;
			iss >> root;

			Assert::IsTrue(root["var"].isArray());
			Assert::AreEqual(
				glm::mat4({
					{ 1,2,3,4 },
					{ 5,6,7,8 },
					{ 9,10,11,12 },
					{ 13,14,15,16 }}),
					FieaGameEngine::FromJsonValue<glm::mat4>(root["var"]));
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}