#include "pch.h"

#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "ActionList.h"
#include "GameObject.h"
#include "ActionIncrement.h"
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
	TEST_CLASS(ActionListTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionListFactory>());
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
			ActionList actionList;

			// Check prescribed attributes
			Assert::IsTrue(actionList.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionList.IsPrescribedAttribute("Actions"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::String, actionList.Find("Name")->Type());
			Assert::AreEqual(DatumTypes::Table, actionList.Find("Actions")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionList.At("Name").Size());
			Assert::AreEqual(0_z, actionList.At("Actions").Size());

			// Check name value
			Assert::AreEqual(""s, actionList.Name);
			Assert::AreEqual(""s, actionList.At("Name").Get<string>());
			Assert::AreSame(actionList.Name, actionList.At("Name").Get<string>());

			// Check actions method
			Assert::AreSame(actionList.At("Actions"), actionList.Actions());
			Assert::AreSame(actionList.At("Actions"), const_cast<const ActionList&>(actionList).Actions());
		}

		TEST_METHOD(CopySemantics)
		{
			// Copy constructor
			{
				ActionList actionList;
				actionList.Name = "ActionList";
				shared_ptr<ActionList> innerActionList = make_shared<ActionList>();
				actionList.At("Actions").PushBack<shared_ptr<Scope>>(innerActionList);

				// Create copy
				ActionList actionListCopy = actionList;

				// Check prescribed attributes
				Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Actions"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionListCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::Table, actionListCopy.Find("Actions")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionListCopy.At("Name").Size());
				Assert::AreEqual(1_z, actionListCopy.At("Actions").Size());

				// Check name value
				Assert::AreEqual("ActionList"s, actionListCopy.Name);
				Assert::AreEqual("ActionList"s, actionListCopy.At("Name").Get<string>());
				Assert::AreSame(actionListCopy.Name, actionListCopy.At("Name").Get<string>());

				// Check actions were copied
				Assert::AreNotSame(
					*actionList.At("Actions").Get<shared_ptr<Scope>>(),
					*actionListCopy.At("Actions").Get<shared_ptr<Scope>>());
			}

			// Copy assignment
			{
				ActionList actionList;
				actionList.Name = "ActionList";
				shared_ptr<ActionList> innerActionList = make_shared<ActionList>();
				actionList.At("Actions").PushBack<shared_ptr<Scope>>(innerActionList);

				ActionList actionListCopy;

				// Copy
				actionListCopy = actionList;

				// Check prescribed attributes
				Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Actions"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionListCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::Table, actionListCopy.Find("Actions")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionListCopy.At("Name").Size());
				Assert::AreEqual(1_z, actionListCopy.At("Actions").Size());

				// Check name value
				Assert::AreEqual("ActionList"s, actionListCopy.Name);
				Assert::AreEqual("ActionList"s, actionListCopy.At("Name").Get<string>());
				Assert::AreSame(actionListCopy.Name, actionListCopy.At("Name").Get<string>());

				// Check actions were copied
				Assert::AreNotSame(
					*actionList.At("Actions").Get<shared_ptr<Scope>>(),
					*actionListCopy.At("Actions").Get<shared_ptr<Scope>>());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				ActionList actionList;
				actionList.Name = "ActionList";
				shared_ptr<ActionList> innerActionList = make_shared<ActionList>();
				actionList.At("Actions").PushBack<shared_ptr<Scope>>(innerActionList);

				// Move
				ActionList actionList2 = std::move(actionList);

				// Check prescribed attributes
				Assert::IsTrue(actionList2.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionList2.IsPrescribedAttribute("Actions"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionList2.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::Table, actionList2.Find("Actions")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionList2.At("Name").Size());
				Assert::AreEqual(1_z, actionList2.At("Actions").Size());

				// Check name value
				Assert::AreEqual("ActionList"s, actionList2.Name);
				Assert::AreEqual("ActionList"s, actionList2.At("Name").Get<string>());
				Assert::AreSame(actionList2.Name, actionList2.At("Name").Get<string>());

				// Check original data was moved
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(""s, actionList.Name);
				Assert::IsNull(actionList.Find("Name"));
				Assert::IsNull(actionList.Find("Actions"));
#pragma warning(pop)
			}

			// Move assignment
			{
				ActionList actionList;
				actionList.Name = "ActionList";
				shared_ptr<ActionList> innerActionList = make_shared<ActionList>();
				actionList.At("Actions").PushBack<shared_ptr<Scope>>(innerActionList);

				ActionList actionList2;

				// Move
				actionList2 = std::move(actionList);

				// Check prescribed attributes
				Assert::IsTrue(actionList2.IsPrescribedAttribute("Name"));
				Assert::IsTrue(actionList2.IsPrescribedAttribute("Actions"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::String, actionList2.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::Table, actionList2.Find("Actions")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionList2.At("Name").Size());
				Assert::AreEqual(1_z, actionList2.At("Actions").Size());

				// Check name value
				Assert::AreEqual("ActionList"s, actionList2.Name);
				Assert::AreEqual("ActionList"s, actionList2.At("Name").Get<string>());
				Assert::AreSame(actionList2.Name, actionList2.At("Name").Get<string>());
			}
		}

		TEST_METHOD(Clone)
		{
			ActionList actionList;
			actionList.Name = "ActionList";
			shared_ptr<ActionList> innerActionList = make_shared<ActionList>();
			actionList.At("Actions").PushBack<shared_ptr<Scope>>(innerActionList);

			// Create copy
			unique_ptr<Scope> actionListCopyPtr = actionList.Clone();
			Assert::IsNotNull(actionListCopyPtr.get());
			Assert::IsTrue(actionListCopyPtr->Is(ActionList::TypeIdClass()));
			ActionList& actionListCopy = *actionListCopyPtr->As<ActionList>();

			// Check prescribed attributes
			Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Name"));
			Assert::IsTrue(actionListCopy.IsPrescribedAttribute("Actions"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::String, actionListCopy.Find("Name")->Type());
			Assert::AreEqual(DatumTypes::Table, actionListCopy.Find("Actions")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionListCopy.At("Name").Size());
			Assert::AreEqual(1_z, actionListCopy.At("Actions").Size());

			// Check name value
			Assert::AreEqual("ActionList"s, actionListCopy.Name);
			Assert::AreEqual("ActionList"s, actionListCopy.At("Name").Get<string>());
			Assert::AreSame(actionListCopy.Name, actionListCopy.At("Name").Get<string>());

			// Check actions were copied
			Assert::AreNotSame(
				*actionList.At("Actions").Get<shared_ptr<Scope>>(),
				*actionListCopy.At("Actions").Get<shared_ptr<Scope>>());
		}

		TEST_METHOD(Update)
		{
			ActionList actionList;

			// Add counter
			Datum& counter = actionList.AppendAuxilaryAttribute("Counter");
			counter.PushBack(0);

			// Add 2 action increments to list
			ActionIncrement actionIncrement;
			actionIncrement.Target = "Counter";

			actionList.Adopt(actionIncrement.Clone(), "Actions");
			actionList.Adopt(actionIncrement.Clone(), "Actions");

			// Update
			GameTime gameTime;
			actionList.Update(gameTime);

			Assert::AreEqual(2, actionList.At("Counter").Get<int32_t>());

			actionList.Update(gameTime);

			Assert::AreEqual(4, actionList.At("Counter").Get<int32_t>());
		}

		TEST_METHOD(Deserialize)
		{
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<GameObject>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.DeserializeObjectFromFile("JsonFiles/ActionListTest.json"s);

			GameObject& gameObject = static_cast<GameObject&>(*wrapper->Data);
			GameTime gameTime;

			gameObject.Update(gameTime);

			Assert::AreEqual(2, gameObject.At("A").Get<int32_t>());

			gameObject.Update(gameTime);

			Assert::AreEqual(4, gameObject.At("A").Get<int32_t>());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}