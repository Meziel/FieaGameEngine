#include "pch.h"
#include "Literals.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "JsonTestParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "JsonTableParseHelper.h"
#include "AttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonParseCoordinatorTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(make_unique<ScopeFactory>());
			Factory<Scope>::Add(make_unique<AttributedFooFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Clear();
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
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::AreEqual(0_z, parseCoordinator.Helpers().Size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());
			
			parseCoordinator.AddHelper(make_shared<JsonTestParseHelper>());
			Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());

			shared_ptr<JsonParseCoordinator::Wrapper> anotherWrapper = make_shared<JsonTestParseHelper::Wrapper>();
			parseCoordinator.SetWrapper(anotherWrapper);
			Assert::AreSame(*anotherWrapper, parseCoordinator.GetWrapper());
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				const string inputString = R"({
				"Health": {
						"type": "integer",
						"value": 100
					}
				})";

				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
				parseCoordinator.AddHelper(helper);
				parseCoordinator.DeserializeObject(inputString);

				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());

				Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());
				Assert::AreEqual(&parseCoordinator, wrapper->GetJsonParseCoordinator());
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(3_z, helper->StartHandlerCount);
				Assert::AreEqual(3_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);

				JsonParseCoordinator newParseCoordinator = std::move(parseCoordinator);
				Assert::AreEqual(1_z, newParseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, newParseCoordinator.GetWrapper());
				Assert::AreEqual(&newParseCoordinator, wrapper->GetJsonParseCoordinator());
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(3_z, helper->StartHandlerCount);
				Assert::AreEqual(3_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);
			}

			// Move assignment
			{
				const string inputString = R"({
				"Health": {
						"type": "integer",
						"value": 100
					}
				})";

				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
				parseCoordinator.AddHelper(helper);
				parseCoordinator.DeserializeObject(inputString);

				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());

				Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());
				Assert::AreEqual(&parseCoordinator, wrapper->GetJsonParseCoordinator());
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(3_z, helper->StartHandlerCount);
				Assert::AreEqual(3_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);

				JsonParseCoordinator newParseCoordinator(make_shared<JsonIntegerParseHelper::Wrapper>());
				newParseCoordinator = std::move(parseCoordinator);
				Assert::AreEqual(1_z, newParseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, newParseCoordinator.GetWrapper());
				Assert::AreEqual(&newParseCoordinator, wrapper->GetJsonParseCoordinator());
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(3_z, helper->StartHandlerCount);
				Assert::AreEqual(3_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);
			}
		}

		TEST_METHOD(Clone)
		{
			const string inputString = R"({
				"Health": {
					"type": "integer",
					"value": 100
				}
			})";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.DeserializeObject(inputString);
			
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());

			Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());
			Assert::AreEqual(&parseCoordinator, wrapper->GetJsonParseCoordinator());
			Assert::AreEqual(1_z, rawWrapper->MaxDepth);
			Assert::AreEqual(3_z, helper->StartHandlerCount);
			Assert::AreEqual(3_z, helper->EndHandlerCount);
			Assert::IsTrue(helper->InitializeCalled);
			Assert::IsTrue(helper->CleanupCalled);

			unique_ptr<JsonParseCoordinator> clonedParseCoordinator = parseCoordinator.Clone();
			Assert::AreNotSame(parseCoordinator, *clonedParseCoordinator);
			
			// Check cloned wrapper
			JsonParseCoordinator::Wrapper& clonedWrapper = clonedParseCoordinator->GetWrapper();
			Assert::IsTrue(clonedWrapper.Is(JsonTestParseHelper::Wrapper::TypeIdClass()));
			JsonTestParseHelper::Wrapper& clonedWrapperDerived = static_cast<JsonTestParseHelper::Wrapper&>(clonedWrapper);
			Assert::AreNotSame(*wrapper, clonedWrapper);
			Assert::AreEqual(clonedParseCoordinator.get(), clonedWrapper.GetJsonParseCoordinator());
			Assert::AreEqual(0_z, clonedWrapperDerived.MaxDepth);

			// Check cloned helper
			Assert::AreEqual(1_z, clonedParseCoordinator->Helpers().Size());
			shared_ptr<IJsonParseHelper>& clonedHelper = clonedParseCoordinator->Helpers().Front();
			Assert::AreNotSame(static_cast<IJsonParseHelper&>(*helper), *clonedHelper);
			Assert::IsTrue(clonedHelper->Is(JsonTestParseHelper::TypeIdClass()));
			JsonTestParseHelper& clonedHelperDerived = static_cast<JsonTestParseHelper&>(*clonedHelper);
			Assert::AreEqual(0_z, clonedHelperDerived.StartHandlerCount);
			Assert::AreEqual(0_z, clonedHelperDerived.EndHandlerCount);
			Assert::IsFalse(clonedHelperDerived.InitializeCalled);
			Assert::IsFalse(clonedHelperDerived.CleanupCalled);
		}

		TEST_METHOD(CannotParse)
		{
			const string inputString = R"({ "integer": "not a int" })";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(0_z, rawWrapper->Data.Size());
		}

		TEST_METHOD(IntegerParsing)
		{
			string inputString = R"({ "integer": 100 })";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1_z, rawWrapper->Data.Size());
			Assert::AreEqual(100, rawWrapper->Data.Front());
		}

		TEST_METHOD(IntegerArrayParsing)
		{
			const string inputString = R"({ "integer": [100, 200, 300, 400] })";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(4_z, rawWrapper->Data.Size());
			Vector<int32_t> expected = {100, 200, 300, 400};
			Assert::IsTrue(std::equal(rawWrapper->Data.begin(), rawWrapper->Data.end(), expected.begin()));
		}

		TEST_METHOD(ScopeParsing)
		{
			// Init
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<Scope>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);


			// Populating empty scope
			{
				const string inputString = R"({
					"IntegerField": {
						"type": "integer",
						"value": 100
					},
					"FloatField": {
						"type": "float",
						"value": 10.0
					},
					"StringField": {
						"type": "string",
						"value": "Hello World!"
					},
					"VectorField": {
						"type": "vector",
						"value": [1,2,3,4]
					},
					"MatrixField": {
						"type": "matrix",
						"value": [
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check int
				Datum* integerDatum = wrapper->Data->Find("IntegerField");
				Assert::IsNotNull(integerDatum);
				Assert::AreEqual(DatumTypes::Integer, integerDatum->Type());
				Assert::AreEqual(1_z, integerDatum->Size());
				Assert::AreEqual(100, integerDatum->Front<int32_t>());

				// Check float
				Datum* floatDatum = wrapper->Data->Find("FloatField");
				Assert::IsNotNull(floatDatum);
				Assert::AreEqual(DatumTypes::Float, floatDatum->Type());
				Assert::AreEqual(1_z, floatDatum->Size());
				Assert::AreEqual(10.0f, floatDatum->Front<float>());

				// Check string
				Datum* stringDatum = wrapper->Data->Find("StringField");
				Assert::IsNotNull(stringDatum);
				Assert::AreEqual(DatumTypes::String, stringDatum->Type());
				Assert::AreEqual(1_z, stringDatum->Size());
				Assert::AreEqual("Hello World!"s, stringDatum->Front<std::string>());

				// Check vector
				Datum* vectorDatum = wrapper->Data->Find("VectorField");
				Assert::IsNotNull(vectorDatum);
				Assert::AreEqual(DatumTypes::Vector, vectorDatum->Type());
				Assert::AreEqual(1_z, vectorDatum->Size());
				Assert::AreEqual(glm::vec4(1,2,3,4), vectorDatum->Front<glm::vec4>());

				// Check matrix
				Datum* matrixDatum = wrapper->Data->Find("MatrixField");
				Assert::IsNotNull(matrixDatum);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatum->Type());
				Assert::AreEqual(1_z, matrixDatum->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Front<glm::mat4>());
			}

			// Nested scopes
			{
				const string inputString = R"({
					"NestedScope1": {
						"type": "table",
						"value": {
							"NestedNestedScope": {
								"type": "table",
								"value": {
									"Health": {
										"type": "integer",
										"value": 100
									}
								}
							}
						}
					},
					"NestedScope2": {
						"type": "table",
						"value": {
							"NestedNestedScope": {
								"type": "table",
								"value": {
									"Health": {
										"type": "integer",
										"value": 50
									}
								}
							}
						}
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check Scope 1
				Datum* nestedScope1Datum = wrapper->Data->Find("NestedScope1");
				Assert::IsNotNull(nestedScope1Datum);
				Assert::AreEqual(DatumTypes::Table, nestedScope1Datum->Type());
				Assert::AreEqual(1_z, nestedScope1Datum->Size());

				// Get Nested Nested Scope
				Scope* nestedScope1 = nestedScope1Datum->Front<shared_ptr<Scope>>().get();
				Datum* nestedNestedScope1Datum = nestedScope1->Find("NestedNestedScope");
				Assert::IsNotNull(nestedNestedScope1Datum);
				Assert::AreEqual(DatumTypes::Table, nestedNestedScope1Datum->Type());
				Assert::AreEqual(1_z, nestedNestedScope1Datum->Size());

				// Get health 1
				Scope* nestedNestedScope1 = nestedNestedScope1Datum->Front<shared_ptr<Scope>>().get();
				Datum* health1Datum = nestedNestedScope1->Find("Health");
				Assert::IsNotNull(health1Datum);
				Assert::AreEqual(DatumTypes::Integer, health1Datum->Type());
				Assert::AreEqual(1_z, health1Datum->Size());
				Assert::AreEqual(100, health1Datum->Front<int32_t>());
			}

			// Array testing
			{
				const string inputString = R"({
					"IntegerField": {
						"type": "integer[]",
						"value": [1,2,3]
					},
					"FloatField": {
						"type": "float[]",
						"value": [1.1,2.2,3.3]
					},
					"StringField": {
						"type": "string[]",
						"value": ["string1", "string2", "string3"]
					},
					"VectorField": {
						"type": "vector[]",
						"value": [[1,2,3,4], [5,6,7,8], [9,10,11,12]]
					},
					"MatrixField": {
						"type": "matrix[]",
						"value": [[
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						],
						[
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12], 
							[13, 14, 15, 16]
						]]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check int
				Datum* integerDatum = wrapper->Data->Find("IntegerField");
				Assert::IsNotNull(integerDatum);
				Assert::AreEqual(DatumTypes::Integer, integerDatum->Type());
				Assert::AreEqual(3_z, integerDatum->Size());
				Assert::AreEqual(1, integerDatum->Get<int32_t>(0));
				Assert::AreEqual(2, integerDatum->Get<int32_t>(1));
				Assert::AreEqual(3, integerDatum->Get<int32_t>(2));

				// Check float
				Datum* floatDatum = wrapper->Data->Find("FloatField");
				Assert::IsNotNull(floatDatum);
				Assert::AreEqual(DatumTypes::Float, floatDatum->Type());
				Assert::AreEqual(3_z, floatDatum->Size());
				Assert::AreEqual(1.1f, floatDatum->Get<float>(0));
				Assert::AreEqual(2.2f, floatDatum->Get<float>(1));
				Assert::AreEqual(3.3f, floatDatum->Get<float>(2));

				// Check string
				Datum* stringDatum = wrapper->Data->Find("StringField");
				Assert::IsNotNull(stringDatum);
				Assert::AreEqual(DatumTypes::String, stringDatum->Type());
				Assert::AreEqual(3_z, stringDatum->Size());
				Assert::AreEqual("string1"s, stringDatum->Get<std::string>(0));
				Assert::AreEqual("string2"s, stringDatum->Get<std::string>(1));
				Assert::AreEqual("string3"s, stringDatum->Get<std::string>(2));

				// Check vector
				Datum* vectorDatum = wrapper->Data->Find("VectorField");
				Assert::IsNotNull(vectorDatum);
				Assert::AreEqual(DatumTypes::Vector, vectorDatum->Type());
				Assert::AreEqual(3_z, vectorDatum->Size());
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatum->Get<glm::vec4>(0));
				Assert::AreEqual(glm::vec4(5, 6, 7, 8), vectorDatum->Get<glm::vec4>(1));
				Assert::AreEqual(glm::vec4(9, 10, 11, 12), vectorDatum->Get<glm::vec4>(2));

				// Check matrix
				Datum* matrixDatum = wrapper->Data->Find("MatrixField");
				Assert::IsNotNull(matrixDatum);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatum->Type());
				Assert::AreEqual(2_z, matrixDatum->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Get<glm::mat4>(0));
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Get<glm::mat4>(1));
			}

			// Nested scope arrays
			{
				const string inputString = R"({
					"Scopes": {
						"type": "table[]",
						"value": [
							{
								"Health": {
									"type": "integer",
									"value": 100
								}
							},
							{
								"Health": {
									"type": "integer",
									"value": 50
								}
							}
						]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check Scope exists
				Datum* scopes = wrapper->Data->Find("Scopes");
				Assert::IsNotNull(scopes);
				Assert::AreEqual(DatumTypes::Table, scopes->Type());
				Assert::AreEqual(2_z, scopes->Size());

				for (size_t i = 0; i < 2; ++i)
				{
					// Get Nested Nested Scope
					Scope* nestedScope = scopes->Get<shared_ptr<Scope>>(i).get();
					Assert::IsNotNull(nestedScope);

					Datum* healthDatum = nestedScope->Find("Health");
					Assert::IsNotNull(healthDatum);
					Assert::AreEqual(DatumTypes::Integer, healthDatum->Type());
					Assert::AreEqual(1_z, healthDatum->Size());

					if (i == 0)
					{
						Assert::AreEqual(100, healthDatum->Front<int32_t>());
					}
					else
					{
						Assert::AreEqual(50, healthDatum->Front<int32_t>());
					}
				}
			}
		}

		TEST_METHOD(AttributedFooTesting)
		{
			// Init
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<AttributedFoo>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);

			// Change type not in attributed foo
			{
				const string inputString = R"({
					"IntegerField": {
						"type": "integer",
						"value": 100
					},
					"FloatField": {
						"type": "float",
						"value": 10.0
					},
					"StringField": {
						"type": "string",
						"value": "Hello World!"
					},
					"VectorField": {
						"type": "vector",
						"value": [1,2,3,4]
					},
					"MatrixField": {
						"type": "matrix",
						"value": [
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);
			}

			{
				// Check int
				Datum* integerDatum = wrapper->Data->Find("IntegerField");
				Assert::IsNotNull(integerDatum);
				Assert::AreEqual(DatumTypes::Integer, integerDatum->Type());
				Assert::AreEqual(1_z, integerDatum->Size());
				Assert::AreEqual(100, integerDatum->Front<int32_t>());

				// Check float
				Datum* floatDatum = wrapper->Data->Find("FloatField");
				Assert::IsNotNull(floatDatum);
				Assert::AreEqual(DatumTypes::Float, floatDatum->Type());
				Assert::AreEqual(1_z, floatDatum->Size());
				Assert::AreEqual(10.0f, floatDatum->Front<float>());

				// Check string
				Datum* stringDatum = wrapper->Data->Find("StringField");
				Assert::IsNotNull(stringDatum);
				Assert::AreEqual(DatumTypes::String, stringDatum->Type());
				Assert::AreEqual(1_z, stringDatum->Size());
				Assert::AreEqual("Hello World!"s, stringDatum->Front<std::string>());

				// Check vector
				Datum* vectorDatum = wrapper->Data->Find("VectorField");
				Assert::IsNotNull(vectorDatum);
				Assert::AreEqual(DatumTypes::Vector, vectorDatum->Type());
				Assert::AreEqual(1_z, vectorDatum->Size());
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatum->Front<glm::vec4>());

				// Check matrix
				Datum* matrixDatum = wrapper->Data->Find("MatrixField");
				Assert::IsNotNull(matrixDatum);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatum->Type());
				Assert::AreEqual(1_z, matrixDatum->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Front<glm::mat4>());
			}

			// Change non-external in attributed foo
			{
				wrapper->Data->Append("IntegerField").PushBack(1);
				wrapper->Data->Append("FloatField").PushBack(5.0f);
				wrapper->Data->Append("StringField").PushBack("test"s);
				wrapper->Data->Append("VectorField").PushBack(glm::vec4(4,3,2,1));
				wrapper->Data->Append("MatrixField").PushBack(glm::mat4(1));

				const string inputString = R"({
					"IntegerField": {
						"type": "integer",
						"value": 100
					},
					"FloatField": {
						"type": "float",
						"value": 10.0
					},
					"StringField": {
						"type": "string",
						"value": "Hello World!"
					},
					"VectorField": {
						"type": "vector",
						"value": [1,2,3,4]
					},
					"MatrixField": {
						"type": "matrix",
						"value": [
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check int
				Datum* integerDatum = wrapper->Data->Find("IntegerField");
				Assert::IsNotNull(integerDatum);
				Assert::AreEqual(DatumTypes::Integer, integerDatum->Type());
				Assert::AreEqual(1_z, integerDatum->Size());
				Assert::AreEqual(100, integerDatum->Front<int32_t>());

				// Check float
				Datum* floatDatum = wrapper->Data->Find("FloatField");
				Assert::IsNotNull(floatDatum);
				Assert::AreEqual(DatumTypes::Float, floatDatum->Type());
				Assert::AreEqual(1_z, floatDatum->Size());
				Assert::AreEqual(10.0f, floatDatum->Front<float>());

				// Check string
				Datum* stringDatum = wrapper->Data->Find("StringField");
				Assert::IsNotNull(stringDatum);
				Assert::AreEqual(DatumTypes::String, stringDatum->Type());
				Assert::AreEqual(1_z, stringDatum->Size());
				Assert::AreEqual("Hello World!"s, stringDatum->Front<std::string>());

				// Check vector
				Datum* vectorDatum = wrapper->Data->Find("VectorField");
				Assert::IsNotNull(vectorDatum);
				Assert::AreEqual(DatumTypes::Vector, vectorDatum->Type());
				Assert::AreEqual(1_z, vectorDatum->Size());
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatum->Front<glm::vec4>());

				// Check matrix
				Datum* matrixDatum = wrapper->Data->Find("MatrixField");
				Assert::IsNotNull(matrixDatum);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatum->Type());
				Assert::AreEqual(1_z, matrixDatum->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Front<glm::mat4>());
			}

			// Change external datums in attributed foo
			{
				const string inputString = R"({
					"ExternalInteger": {
						"type": "integer",
						"value": 100
					},
					"ExternalFloat": {
						"type": "float",
						"value": 10.0
					},
					"ExternalString": {
						"type": "string",
						"value": "Hello World!"
					},
					"ExternalVector": {
						"type": "vector",
						"value": [1,2,3,4]
					},
					"ExternalMatrix": {
						"type": "matrix",
						"value": [
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						]
					},
					"ExternalIntegerArray": {
						"type": "integer[]",
						"value": [100, 100]
					},
					"ExternalFloatArray": {
						"type": "float[]",
						"value": [10.0, 10.0]
					},
					"ExternalStringArray": {
						"type": "string[]",
						"value": ["Hello World!", "Hello World!"]
					},
					"ExternalVectorArray": {
						"type": "vector[]",
						"value": [[1,2,3,4], [1,2,3,4]]
					},
					"ExternalMatrixArray": {
						"type": "matrix[]",
						"value": [
						[
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						],
						[
							[01, 02, 03, 04],
							[05, 06, 07, 08],
							[09, 10, 11, 12],
							[13, 14, 15, 16]
						]]
					}
				})";

				// Deserialize
				parseCoordinator.DeserializeObject(inputString);

				// Check int
				Datum* integerDatum = wrapper->Data->Find("ExternalInteger");
				Assert::IsNotNull(integerDatum);
				Assert::AreEqual(DatumTypes::Integer, integerDatum->Type());
				Assert::AreEqual(1_z, integerDatum->Size());
				Assert::AreEqual(100, integerDatum->Front<int32_t>());

				// Check float
				Datum* floatDatum = wrapper->Data->Find("ExternalFloat");
				Assert::IsNotNull(floatDatum);
				Assert::AreEqual(DatumTypes::Float, floatDatum->Type());
				Assert::AreEqual(1_z, floatDatum->Size());
				Assert::AreEqual(10.0f, floatDatum->Front<float>());

				// Check string
				Datum* stringDatum = wrapper->Data->Find("ExternalString");
				Assert::IsNotNull(stringDatum);
				Assert::AreEqual(DatumTypes::String, stringDatum->Type());
				Assert::AreEqual(1_z, stringDatum->Size());
				Assert::AreEqual("Hello World!"s, stringDatum->Front<std::string>());

				// Check vector
				Datum* vectorDatum = wrapper->Data->Find("ExternalVector");
				Assert::IsNotNull(vectorDatum);
				Assert::AreEqual(DatumTypes::Vector, vectorDatum->Type());
				Assert::AreEqual(1_z, vectorDatum->Size());
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatum->Front<glm::vec4>());

				// Check matrix
				Datum* matrixDatum = wrapper->Data->Find("ExternalMatrix");
				Assert::IsNotNull(matrixDatum);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatum->Type());
				Assert::AreEqual(1_z, matrixDatum->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatum->Front<glm::mat4>());

				// Check int array
				Datum* integerDatumArray = wrapper->Data->Find("ExternalIntegerArray");
				Assert::IsNotNull(integerDatumArray);
				Assert::AreEqual(DatumTypes::Integer, integerDatumArray->Type());
				Assert::AreEqual(2_z, integerDatumArray->Size());
				Assert::AreEqual(100, integerDatumArray->Get<int32_t>(0));
				Assert::AreEqual(100, integerDatumArray->Get<int32_t>(1));

				// Check float array
				Datum* floatDatumArray = wrapper->Data->Find("ExternalFloatArray");
				Assert::IsNotNull(floatDatumArray);
				Assert::AreEqual(DatumTypes::Float, floatDatumArray->Type());
				Assert::AreEqual(2_z, floatDatumArray->Size());
				Assert::AreEqual(10.0f, floatDatumArray->Get<float>(0));
				Assert::AreEqual(10.0f, floatDatumArray->Get<float>(1));

				// Check string array
				Datum* stringDatumArray = wrapper->Data->Find("ExternalStringArray");
				Assert::IsNotNull(stringDatumArray);
				Assert::AreEqual(DatumTypes::String, stringDatumArray->Type());
				Assert::AreEqual(2_z, stringDatumArray->Size());
				Assert::AreEqual("Hello World!"s, stringDatumArray->Get<std::string>(0));
				Assert::AreEqual("Hello World!"s, stringDatumArray->Get<std::string>(1));

				// Check vector array
				Datum* vectorDatumArray = wrapper->Data->Find("ExternalVectorArray");
				Assert::IsNotNull(vectorDatumArray);
				Assert::AreEqual(DatumTypes::Vector, vectorDatumArray->Type());
				Assert::AreEqual(2_z, vectorDatumArray->Size());
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatumArray->Get<glm::vec4>(0));
				Assert::AreEqual(glm::vec4(1, 2, 3, 4), vectorDatumArray->Get<glm::vec4>(1));

				// Check matrix array
				Datum* matrixDatumArray = wrapper->Data->Find("ExternalMatrixArray");
				Assert::IsNotNull(matrixDatumArray);
				Assert::AreEqual(DatumTypes::Matrix, matrixDatumArray->Type());
				Assert::AreEqual(2_z, matrixDatumArray->Size());
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatumArray->Get<glm::mat4>(0));
				Assert::AreEqual(glm::mat4({ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }), matrixDatumArray->Get<glm::mat4>(1));
			}
		}

		TEST_METHOD(BadJsons)
		{
			// Init
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<Scope>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);

			// Type not there
			{
				const string inputString = R"({
					"Health": {
						"not_type": "integer",
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type not a string
			{
				const string inputString = R"({
					"Health": {
						"type": 1,
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type not a string
			{
				const string inputString = R"({
					"Health": {
						"type": 1,
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type that fails regex
			{
				const string inputString = R"({
					"Health": {
						"type": "not valid type",
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type is a valid DatumType
			{
				const string inputString = R"({
					"Health": {
						"type": "MyType",
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type is unknown
			{
				const string inputString = R"({
					"Health": {
						"type": "unknown",
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Type is pointer
			{
				const string inputString = R"({
					"Health": {
						"type": "pointer",
						"value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Value does not exist
			{
				const string inputString = R"({
					"Health": {
						"type": "integer",
						"no_value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Value does not exist
			{
				const string inputString = R"({
					"Health": {
						"type": "integer",
						"no_value": 1
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Class name does not have an associated factory
			{
				const string inputString = R"({
					"Foo": {
						"type": "table",
						"class": "BadClass",
						"value": {
							"IntegerField": {
								"type": "integer",
								"value": 100
							}
						}
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}

			// Class is not a string
			{
				const string inputString = R"({
					"Foo": {
						"type": "table",
						"class": 1,
						"value": {
							"IntegerField": {
								"type": "integer",
								"value": 100
							}
						}
					}
				})";

				// Deserialize
				Assert::ExpectException<runtime_error>([&parseCoordinator, &inputString]() { parseCoordinator.DeserializeObject(inputString); });
			}
		}

		TEST_METHOD(JsonTestParsing)
		{
			{
				const string inputString = R"({
					"Health": {
						"type": "integer",
						"value": 100
					}
				})";

				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
				parseCoordinator.AddHelper(helper);
				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(0_z, rawWrapper->MaxDepth);
				Assert::AreEqual(0_z, helper->StartHandlerCount);
				Assert::AreEqual(0_z, helper->EndHandlerCount);
				Assert::IsFalse(helper->InitializeCalled);
				Assert::IsFalse(helper->CleanupCalled);

				parseCoordinator.DeserializeObject(inputString);
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(3_z, helper->StartHandlerCount);
				Assert::AreEqual(3_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);
			}

			// Array of objects
			{
				const string inputString = R"({
					"Items": [
						{
							"type": "string",
							"value": "mat"
						},
						{
							"type": "string",
							"value": "backpack"
						}
					]
				})";

				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
				parseCoordinator.AddHelper(helper);
				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(0_z, rawWrapper->MaxDepth);
				Assert::AreEqual(0_z, helper->StartHandlerCount);
				Assert::AreEqual(0_z, helper->EndHandlerCount);
				Assert::IsFalse(helper->InitializeCalled);
				Assert::IsFalse(helper->CleanupCalled);

				parseCoordinator.DeserializeObject(inputString);
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(7_z, helper->StartHandlerCount);
				Assert::AreEqual(7_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);
			}

			// Array in arrays
			{
				const string inputString = R"({
					"Items": [
						[{
							"type": "string",
							"value": "mat"
						}],
						[{
							"type": "string",
							"value": "backpack"
						}]
					]
				})";

				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				shared_ptr<JsonTestParseHelper> helper = make_shared<JsonTestParseHelper>();
				parseCoordinator.AddHelper(helper);
				JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(0_z, rawWrapper->MaxDepth);
				Assert::AreEqual(0_z, helper->StartHandlerCount);
				Assert::AreEqual(0_z, helper->EndHandlerCount);
				Assert::IsFalse(helper->InitializeCalled);
				Assert::IsFalse(helper->CleanupCalled);

				parseCoordinator.DeserializeObject(inputString);
				Assert::AreEqual(1_z, rawWrapper->MaxDepth);
				Assert::AreEqual(7_z, helper->StartHandlerCount);
				Assert::AreEqual(7_z, helper->EndHandlerCount);
				Assert::IsTrue(helper->InitializeCalled);
				Assert::IsTrue(helper->CleanupCalled);
			}
		}

		TEST_METHOD(FileParsing)
		{
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(make_shared<JsonIntegerParseHelper>());
			Assert::ExpectException<runtime_error>([&parseCoordinator]() {
				parseCoordinator.DeserializeObjectFromFile("_nonexistent.json");
			});

			parseCoordinator.DeserializeObjectFromFile("JsonFiles/IntegerTest.json");
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1_z, rawWrapper->Data.Size());
			Assert::AreEqual(100, rawWrapper->Data.Front());
		}
		
		TEST_METHOD(AddRemoveHelpers)
		{
			shared_ptr<IJsonParseHelper> helper1 = make_shared<JsonIntegerParseHelper>();
			shared_ptr<IJsonParseHelper> helper2 = make_shared<JsonTestParseHelper>();
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			const JsonParseCoordinator& constParseCoordinator = parseCoordinator;
			
			// Add helpers
			Assert::AreEqual(0_z, parseCoordinator.Helpers().Size());
			Assert::AreEqual(0_z, constParseCoordinator.Helpers().Size());
			parseCoordinator.AddHelper(helper1);
			Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());
			Assert::AreEqual(1_z, constParseCoordinator.Helpers().Size());
			parseCoordinator.AddHelper(helper2);
			Assert::AreEqual(2_z, parseCoordinator.Helpers().Size());
			Assert::AreEqual(2_z, constParseCoordinator.Helpers().Size());
			Vector<shared_ptr<IJsonParseHelper>> expectedHelpers = {helper1, helper2};
			Assert::IsTrue(equal(parseCoordinator.Helpers().begin(), parseCoordinator.Helpers().end(), expectedHelpers.begin()));
			Assert::IsTrue(equal(constParseCoordinator.Helpers().begin(), constParseCoordinator.Helpers().end(), expectedHelpers.begin()));
			
			// Remove helpers
			parseCoordinator.RemoveHelper(helper1);
			Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());
			Assert::AreEqual(1_z, constParseCoordinator.Helpers().Size());
			parseCoordinator.RemoveHelper(helper2);
			Assert::AreEqual(0_z, parseCoordinator.Helpers().Size());
			Assert::AreEqual(0_z, constParseCoordinator.Helpers().Size());
			Assert::IsTrue(parseCoordinator.Helpers().IsEmpty());
			Assert::IsTrue(constParseCoordinator.Helpers().IsEmpty());
		}

		TEST_METHOD(Wrapper)
		{
			shared_ptr<JsonTestParseHelper::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
			shared_ptr<const JsonTestParseHelper::Wrapper> constWrapper = wrapper;
			Assert::AreEqual(0_z, wrapper->Depth());
			Assert::IsNull(wrapper->GetJsonParseCoordinator());
			Assert::IsNull(constWrapper->GetJsonParseCoordinator());

			JsonParseCoordinator parseCoordinator(wrapper);

			Assert::AreEqual(0_z, wrapper->Depth());
			Assert::IsNotNull(wrapper->GetJsonParseCoordinator());
			Assert::IsNotNull(constWrapper->GetJsonParseCoordinator());
			Assert::AreSame(parseCoordinator, *wrapper->GetJsonParseCoordinator());
			Assert::AreSame(parseCoordinator, *constWrapper->GetJsonParseCoordinator());
		}

		TEST_METHOD(ParseTableWithClass)
		{

			// Init
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<Scope>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);

			const string inputString = R"({
				"Foo": {
					"type": "table",
					"class": "AttributedFoo",
					"value": {
						"IntegerField": {
							"type": "integer",
							"value": 100
						}
					}
				}
			})";

			// Deserialize
			parseCoordinator.DeserializeObject(inputString);

			Datum* attributedFooDatum = wrapper->Data->Find("Foo");

			Assert::IsNotNull(attributedFooDatum);
			Assert::AreEqual(DatumTypes::Table, attributedFooDatum->Type());
			Assert::AreEqual(1_z, attributedFooDatum->Size());
			Scope* attributedFoo = attributedFooDatum->Front<shared_ptr<Scope>>().get();
			Assert::IsNotNull(attributedFoo);
			Assert::AreEqual(AttributedFoo::TypeIdClass(), attributedFoo->TypeIdInstance());

			Assert::IsNotNull(attributedFoo->Find("IntegerField"));
			Assert::AreEqual(DatumTypes::Integer, attributedFoo->Find("IntegerField")->Type());
			Assert::AreEqual(1_z, attributedFoo->Find("IntegerField")->Size());
			Assert::AreEqual(100, attributedFoo->Find("IntegerField")->Get<int32_t>(0));
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}