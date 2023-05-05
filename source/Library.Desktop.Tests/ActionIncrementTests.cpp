#include "pch.h"

#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "ActionIncrement.h"
#include "GameObject.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace LibraryDesktopTests
{
	TEST_CLASS(ActionIncrementTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
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
			ActionIncrement actionIncrement;

			// Check prescribed attributes
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Target"));
			Assert::IsTrue(actionIncrement.IsPrescribedAttribute("Step"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::String, actionIncrement.Find("Name")->Type());
			Assert::AreEqual(DatumTypes::String, actionIncrement.Find("Target")->Type());
			Assert::AreEqual(DatumTypes::Unknown, actionIncrement.Find("Step")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionIncrement.At("Name").Size());
			Assert::AreEqual(1_z, actionIncrement.At("Target").Size());
			Assert::AreEqual(0_z, actionIncrement.At("Step").Size());

			// Check name value
			Assert::AreEqual(""s, actionIncrement.Name);
			Assert::AreEqual(""s, actionIncrement.At("Name").Get<string>());
			Assert::AreSame(actionIncrement.Name, actionIncrement.At("Name").Get<string>());

			// Check target value
			Assert::AreEqual(""s, actionIncrement.Target);
			Assert::AreEqual(""s, actionIncrement.At("Target").Get<string>());
			Assert::AreSame(actionIncrement.Target, actionIncrement.At("Target").Get<string>());
		}

		TEST_METHOD(CopySemantics)
		{
			// Copy constructor
			{
				ActionIncrement actionIncrement;

				// Check size of attributes
				actionIncrement.At("Name").Set<string>("ActionIncrement");
				actionIncrement.At("Target").Set<string>("A");
				actionIncrement.At("Step").PushBack<int32_t>(2);

				ActionIncrement actionIncrementCopy = actionIncrement;

				// Check prescribed attributes
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Target"));
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Step"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Target")->Type());
				Assert::AreEqual(DatumTypes::Integer, actionIncrementCopy.Find("Step")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIncrementCopy.At("Name").Size());
				Assert::AreEqual(1_z, actionIncrementCopy.At("Target").Size());
				Assert::AreEqual(1_z, actionIncrementCopy.At("Step").Size());

				// Check name value
				Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.Name);
				Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.At("Name").Get<string>());
				Assert::AreSame(actionIncrementCopy.Name, actionIncrementCopy.At("Name").Get<string>());

				// Check Target value
				Assert::AreEqual("A"s, actionIncrementCopy.Target);
				Assert::AreEqual("A"s, actionIncrementCopy.At("Target").Get<string>());
				Assert::AreSame(actionIncrementCopy.Target, actionIncrementCopy.At("Target").Get<string>());

				// Check Step value
				Assert::AreEqual(2, actionIncrementCopy.At("Step").Get<int32_t>());
			}

			// Copy assignment
			{
				ActionIncrement actionIncrement;

				// Check size of attributes
				actionIncrement.At("Name").Set<string>("ActionIncrement");
				actionIncrement.At("Target").Set<string>("A");
				actionIncrement.At("Step").PushBack<int32_t>(2);

				ActionIncrement actionIncrementCopy;
				actionIncrementCopy = actionIncrement;

				// Check prescribed attributes
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Target"));
				Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Step"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Target")->Type());
				Assert::AreEqual(DatumTypes::Integer, actionIncrementCopy.Find("Step")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIncrementCopy.At("Name").Size());
				Assert::AreEqual(1_z, actionIncrementCopy.At("Target").Size());
				Assert::AreEqual(1_z, actionIncrementCopy.At("Step").Size());

				// Check name value
				Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.Name);
				Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.At("Name").Get<string>());
				Assert::AreSame(actionIncrementCopy.Name, actionIncrementCopy.At("Name").Get<string>());

				// Check Target value
				Assert::AreEqual("A"s, actionIncrementCopy.Target);
				Assert::AreEqual("A"s, actionIncrementCopy.At("Target").Get<string>());
				Assert::AreSame(actionIncrementCopy.Target, actionIncrementCopy.At("Target").Get<string>());

				// Check Step value
				Assert::AreEqual(2, actionIncrementCopy.At("Step").Get<int32_t>());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				ActionIncrement actionIncrement;

				// Check size of attributes
				actionIncrement.At("Name").Set<string>("ActionIncrement");
				actionIncrement.At("Target").Set<string>("A");
				actionIncrement.At("Step").PushBack<int32_t>(2);

				ActionIncrement actionIncrement2 = std::move(actionIncrement);

				// Check prescribed attributes
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Target"));
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Step"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionIncrement2.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::String, actionIncrement2.Find("Target")->Type());
				Assert::AreEqual(DatumTypes::Integer, actionIncrement2.Find("Step")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIncrement2.At("Name").Size());
				Assert::AreEqual(1_z, actionIncrement2.At("Target").Size());
				Assert::AreEqual(1_z, actionIncrement2.At("Step").Size());

				// Check name value
				Assert::AreEqual("ActionIncrement"s, actionIncrement2.Name);
				Assert::AreEqual("ActionIncrement"s, actionIncrement2.At("Name").Get<string>());
				Assert::AreSame(actionIncrement2.Name, actionIncrement2.At("Name").Get<string>());

				// Check Target value
				Assert::AreEqual("A"s, actionIncrement2.Target);
				Assert::AreEqual("A"s, actionIncrement2.At("Target").Get<string>());
				Assert::AreSame(actionIncrement2.Target, actionIncrement2.At("Target").Get<string>());

				// Check Step value
				Assert::AreEqual(2, actionIncrement2.At("Step").Get<int32_t>());

				// Check original data was move
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(""s, actionIncrement.Name);
				Assert::AreEqual(""s, actionIncrement.Target);
				Assert::IsNull(actionIncrement.Find("Name"));
				Assert::IsNull(actionIncrement.Find("Target"));
				Assert::IsNull(actionIncrement.Find("Step"));
#pragma warning(pop)
			}

			// Move assignment
			{
				ActionIncrement actionIncrement;

				// Check size of attributes
				actionIncrement.At("Name").Set<string>("ActionIncrement");
				actionIncrement.At("Target").Set<string>("A");
				actionIncrement.At("Step").PushBack<int32_t>(2);

				ActionIncrement actionIncrement2;
				actionIncrement2 = std::move(actionIncrement);

				// Check prescribed attributes
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Target"));
				Assert::IsTrue(actionIncrement2.IsPrescribedAttribute("Step"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionIncrement2.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::String, actionIncrement2.Find("Target")->Type());
				Assert::AreEqual(DatumTypes::Integer, actionIncrement2.Find("Step")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIncrement2.At("Name").Size());
				Assert::AreEqual(1_z, actionIncrement2.At("Target").Size());
				Assert::AreEqual(1_z, actionIncrement2.At("Step").Size());

				// Check name value
				Assert::AreEqual("ActionIncrement"s, actionIncrement2.Name);
				Assert::AreEqual("ActionIncrement"s, actionIncrement2.At("Name").Get<string>());
				Assert::AreSame(actionIncrement2.Name, actionIncrement2.At("Name").Get<string>());

				// Check Target value
				Assert::AreEqual("A"s, actionIncrement2.Target);
				Assert::AreEqual("A"s, actionIncrement2.At("Target").Get<string>());
				Assert::AreSame(actionIncrement2.Target, actionIncrement2.At("Target").Get<string>());

				// Check Step value
				Assert::AreEqual(2, actionIncrement2.At("Step").Get<int32_t>());
			}
		}

		TEST_METHOD(Clone)
		{
			ActionIncrement actionIncrement;

			// Check size of attributes
			actionIncrement.At("Name").Set<string>("ActionIncrement");
			actionIncrement.At("Target").Set<string>("A");
			actionIncrement.At("Step").PushBack<int32_t>(2);

			unique_ptr<Scope> actionIncrementCopyPtr = actionIncrement.Clone();
			Assert::IsNotNull(actionIncrementCopyPtr.get());
			Assert::IsTrue(actionIncrementCopyPtr->Is(ActionIncrement::TypeIdClass()));
			ActionIncrement& actionIncrementCopy = *actionIncrementCopyPtr->As<ActionIncrement>();

			// Check prescribed attributes
			Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Target"));
			Assert::IsTrue(actionIncrementCopy.IsPrescribedAttribute("Step"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Name")->Type());
			Assert::AreEqual(DatumTypes::String, actionIncrementCopy.Find("Target")->Type());
			Assert::AreEqual(DatumTypes::Integer, actionIncrementCopy.Find("Step")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionIncrementCopy.At("Name").Size());
			Assert::AreEqual(1_z, actionIncrementCopy.At("Target").Size());
			Assert::AreEqual(1_z, actionIncrementCopy.At("Step").Size());

			// Check name value
			Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.Name);
			Assert::AreEqual("ActionIncrement"s, actionIncrementCopy.At("Name").Get<string>());
			Assert::AreSame(actionIncrementCopy.Name, actionIncrementCopy.At("Name").Get<string>());

			// Check Target value
			Assert::AreEqual("A"s, actionIncrementCopy.Target);
			Assert::AreEqual("A"s, actionIncrementCopy.At("Target").Get<string>());
			Assert::AreSame(actionIncrementCopy.Target, actionIncrementCopy.At("Target").Get<string>());
		}

		TEST_METHOD(Update)
		{
			// Nominal
			{
				ActionIncrement actionIncrement;
				actionIncrement["A"].PushBack(0);
				actionIncrement.Target = "A"s;
				actionIncrement.At("Step").PushBack(1);

				GameTime gameTime;

				actionIncrement.Update(gameTime);
				Assert::AreEqual(1, actionIncrement.At("A").Get<int32_t>());

				actionIncrement.Update(gameTime);
				Assert::AreEqual(2, actionIncrement.At("A").Get<int32_t>());

				actionIncrement.Update(gameTime);
				Assert::AreEqual(3, actionIncrement.At("A").Get<int32_t>());
			}

			// Bad target
			{
				ActionIncrement actionIncrement;
				actionIncrement.Target = "A"s;
				actionIncrement.At("Step").PushBack(1);

				GameTime gameTime;

				Assert::ExpectException<runtime_error>([&actionIncrement, &gameTime]() { actionIncrement.Update(gameTime); });
				actionIncrement["A"].PushBack(""s);
				Assert::ExpectException<runtime_error>([&actionIncrement, &gameTime]() { actionIncrement.Update(gameTime); });
			}

			// Bad step
			{
				ActionIncrement actionIncrement;
				actionIncrement["A"].PushBack(0);
				actionIncrement.Target = "A"s;
				actionIncrement.At("Step").PushBack(""s);

				GameTime gameTime;

				Assert::ExpectException<runtime_error>([&actionIncrement, &gameTime]() { actionIncrement.Update(gameTime); });
			}
		}

		TEST_METHOD(Deserialize)
		{
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<GameObject>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.DeserializeObjectFromFile("JsonFiles/ActionIncrementTest.json"s);

			GameObject& gameObject = static_cast<GameObject&>(*wrapper->Data);
			GameTime gameTime;

			gameObject.Update(gameTime);

			Assert::AreEqual(1, gameObject.At("IntA").Get<int32_t>());
			Assert::AreEqual(1, gameObject.At("IntB").Get<int32_t>());
			Assert::AreEqual(1, gameObject.At("IntC").Get<int32_t>());
			Assert::AreEqual(1.0f, gameObject.At("FloatA").Get<float>());
			Assert::AreEqual(1.0f, gameObject.At("FloatB").Get<float>());
			Assert::AreEqual(1.5f, gameObject.At("FloatC").Get<float>());

			gameObject.Update(gameTime);

			Assert::AreEqual(2, gameObject.At("IntA").Get<int32_t>());
			Assert::AreEqual(2, gameObject.At("IntB").Get<int32_t>());
			Assert::AreEqual(2, gameObject.At("IntC").Get<int32_t>());
			Assert::AreEqual(2.0f, gameObject.At("FloatA").Get<float>());
			Assert::AreEqual(2.0f, gameObject.At("FloatB").Get<float>());
			Assert::AreEqual(3.0f, gameObject.At("FloatC").Get<float>());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}