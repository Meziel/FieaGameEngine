#include "pch.h"

#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "ActionIf.h"
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
	TEST_CLASS(ActionIfTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionIfFactory>());
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
			ActionIf actionIf;

			// Check prescribed attributes
			Assert::IsTrue(actionIf.IsPrescribedAttribute("Condition"));
			Assert::IsTrue(actionIf.IsPrescribedAttribute("IfBlock"));
			Assert::IsTrue(actionIf.IsPrescribedAttribute("ElseBlock"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::Boolean, actionIf.Find("Condition")->Type());
			Assert::AreEqual(DatumTypes::Table, actionIf.Find("IfBlock")->Type());
			Assert::AreEqual(DatumTypes::Table, actionIf.Find("ElseBlock")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionIf.At("Condition").Size());
			Assert::AreEqual(0_z, actionIf.At("IfBlock").Size());
			Assert::AreEqual(0_z, actionIf.At("ElseBlock").Size());
			
			// Check condition value
			Assert::AreEqual(false, actionIf.Condition);
			Assert::AreEqual(false, actionIf.At("Condition").Get<bool>());
			Assert::AreSame(actionIf.Condition, actionIf.At("Condition").Get<bool>());
		}

		TEST_METHOD(CopySemanatics)
		{
			// Copy constructor
			{
				ActionIf actionIf;
				actionIf.Condition = true;
				Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
				Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");

				ActionIf actionIfCopy = actionIf;

				// Check prescribed attributes
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("Condition"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("IfBlock"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("ElseBlock"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::Boolean, actionIfCopy.Find("Condition")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("IfBlock")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("ElseBlock")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIfCopy.At("Condition").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("IfBlock").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("ElseBlock").Size());

				// Check condition value
				Assert::AreEqual(true, actionIfCopy.Condition);
				Assert::AreEqual(true, actionIfCopy.At("Condition").Get<bool>());
				Assert::AreSame(actionIfCopy.Condition, actionIfCopy.At("Condition").Get<bool>());

				// Check if block value
				Assert::AreNotSame(ifBlock, *actionIfCopy.At("IfBlock").Get<shared_ptr<Scope>>());

				// Check else block value
				Assert::AreNotSame(elseBlock, *actionIfCopy.At("ElseBlock").Get<shared_ptr<Scope>>());
			}

			// Copy assignment
			{
				ActionIf actionIf;
				actionIf.Condition = true;
				Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
				Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");

				ActionIf actionIfCopy;
				actionIfCopy = actionIf;

				// Check prescribed attributes
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("Condition"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("IfBlock"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("ElseBlock"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::Boolean, actionIfCopy.Find("Condition")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("IfBlock")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("ElseBlock")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIfCopy.At("Condition").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("IfBlock").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("ElseBlock").Size());

				// Check condition value
				Assert::AreEqual(true, actionIfCopy.Condition);
				Assert::AreEqual(true, actionIfCopy.At("Condition").Get<bool>());
				Assert::AreSame(actionIfCopy.Condition, actionIfCopy.At("Condition").Get<bool>());

				// Check if block value
				Assert::AreNotSame(ifBlock, *actionIfCopy.At("IfBlock").Get<shared_ptr<Scope>>());

				// Check else block value
				Assert::AreNotSame(elseBlock, *actionIfCopy.At("ElseBlock").Get<shared_ptr<Scope>>());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				ActionIf actionIf;
				actionIf.Condition = true;
				Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
				Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");

				ActionIf actionIfCopy = std::move(actionIf);

				// Check prescribed attributes
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("Condition"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("IfBlock"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("ElseBlock"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::Boolean, actionIfCopy.Find("Condition")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("IfBlock")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("ElseBlock")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIfCopy.At("Condition").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("IfBlock").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("ElseBlock").Size());

				// Check condition value
				Assert::AreEqual(true, actionIfCopy.Condition);
				Assert::AreEqual(true, actionIfCopy.At("Condition").Get<bool>());
				Assert::AreSame(actionIfCopy.Condition, actionIfCopy.At("Condition").Get<bool>());

				// Check if block value
				Assert::AreSame(ifBlock, *actionIfCopy.At("IfBlock").Get<shared_ptr<Scope>>());

				// Check else block value
				Assert::AreSame(elseBlock, *actionIfCopy.At("ElseBlock").Get<shared_ptr<Scope>>());
			
				// Check original data was move
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsNull(actionIf.Find("Condition"));
				Assert::IsNull(actionIf.Find("IfBlock"));
				Assert::IsNull(actionIf.Find("ElseBlock"));
#pragma warning(pop)
			}

			// Move assignment
			{
				ActionIf actionIf;
				actionIf.Condition = true;
				Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
				Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");

				ActionIf actionIfCopy;
				actionIfCopy = std::move(actionIf);

				// Check prescribed attributes
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("Condition"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("IfBlock"));
				Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("ElseBlock"));

				// Check attribute types
				Assert::AreEqual(DatumTypes::Boolean, actionIfCopy.Find("Condition")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("IfBlock")->Type());
				Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("ElseBlock")->Type());

				// Check size of attributes
				Assert::AreEqual(1_z, actionIfCopy.At("Condition").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("IfBlock").Size());
				Assert::AreEqual(1_z, actionIfCopy.At("ElseBlock").Size());

				// Check condition value
				Assert::AreEqual(true, actionIfCopy.Condition);
				Assert::AreEqual(true, actionIfCopy.At("Condition").Get<bool>());
				Assert::AreSame(actionIfCopy.Condition, actionIfCopy.At("Condition").Get<bool>());

				// Check if block value
				Assert::AreSame(ifBlock, *actionIfCopy.At("IfBlock").Get<shared_ptr<Scope>>());

				// Check else block value
				Assert::AreSame(elseBlock, *actionIfCopy.At("ElseBlock").Get<shared_ptr<Scope>>());
			}
		}

		TEST_METHOD(Clone)
		{
			ActionIf actionIf;
			actionIf.Condition = true;
			Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
			Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");

			unique_ptr<Scope> actionIfCopyPtr = actionIf.Clone();
			Assert::IsNotNull(actionIfCopyPtr.get());
			Assert::IsTrue(actionIfCopyPtr->Is(ActionIf::TypeIdClass()));
			ActionIf& actionIfCopy = *actionIfCopyPtr->As<ActionIf>();

			// Check prescribed attributes
			Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("Condition"));
			Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("IfBlock"));
			Assert::IsTrue(actionIfCopy.IsPrescribedAttribute("ElseBlock"));

			// Check attribute types
			Assert::AreEqual(DatumTypes::Boolean, actionIfCopy.Find("Condition")->Type());
			Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("IfBlock")->Type());
			Assert::AreEqual(DatumTypes::Table, actionIfCopy.Find("ElseBlock")->Type());

			// Check size of attributes
			Assert::AreEqual(1_z, actionIfCopy.At("Condition").Size());
			Assert::AreEqual(1_z, actionIfCopy.At("IfBlock").Size());
			Assert::AreEqual(1_z, actionIfCopy.At("ElseBlock").Size());

			// Check condition value
			Assert::AreEqual(true, actionIfCopy.Condition);
			Assert::AreEqual(true, actionIfCopy.At("Condition").Get<bool>());
			Assert::AreSame(actionIfCopy.Condition, actionIfCopy.At("Condition").Get<bool>());

			// Check if block value
			Assert::AreNotSame(ifBlock, *actionIfCopy.At("IfBlock").Get<shared_ptr<Scope>>());

			// Check else block value
			Assert::AreNotSame(elseBlock, *actionIfCopy.At("ElseBlock").Get<shared_ptr<Scope>>());
		}

		TEST_METHOD(Update)
		{
			ActionIf actionIf;
			actionIf.Condition = true;
			actionIf["A"].PushBack(0);

			Scope& ifBlock = actionIf.AppendScope("IfBlock", "ActionIncrement");
			Scope& elseBlock = actionIf.AppendScope("ElseBlock", "ActionIncrement");
			ifBlock.As<ActionIncrement>()->Target = "A"s;
			ifBlock.As<ActionIncrement>()->At("Step").PushBack(1);
			elseBlock.As<ActionIncrement>()->Target ="A"s;
			elseBlock.As<ActionIncrement>()->At("Step").PushBack(-1);

			GameTime gameTime;
			actionIf.Update(gameTime);

			Assert::AreEqual(1, actionIf.At("A").Get<int32_t>());

			actionIf.Update(gameTime);

			Assert::AreEqual(2, actionIf.At("A").Get<int32_t>());

			actionIf.Condition = false;

			actionIf.Update(gameTime);

			Assert::AreEqual(1, actionIf.At("A").Get<int32_t>());

			actionIf.Update(gameTime);

			Assert::AreEqual(0, actionIf.At("A").Get<int32_t>());

			actionIf.Update(gameTime);

			Assert::AreEqual(-1, actionIf.At("A").Get<int32_t>());
		}

		TEST_METHOD(Deserialize)
		{
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<GameObject>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.DeserializeObjectFromFile("JsonFiles/ActionIfTest.json"s);

			GameObject& gameObject = static_cast<GameObject&>(*wrapper->Data);
			GameTime gameTime;

			Assert::AreEqual(1_z, gameObject.Actions().Size());
			Assert::AreEqual(DatumTypes::Table, gameObject.Actions().Type());
			Assert::IsTrue(gameObject.Actions().Get<shared_ptr<Scope>>()->Is(ActionIf::TypeIdClass()));
			Assert::IsTrue(gameObject.Actions().Get<shared_ptr<Scope>>()->As<ActionIf>()->Condition);

			gameObject.Update(gameTime);

			Assert::AreEqual(1, gameObject.At("A").Get<int32_t>());

			gameObject.Update(gameTime);

			Assert::AreEqual(2, gameObject.At("A").Get<int32_t>());

			gameObject.Actions().Get<shared_ptr<Scope>>()->As<ActionIf>()->Condition = false;

			gameObject.Update(gameTime);

			Assert::AreEqual(1, gameObject.At("A").Get<int32_t>());

			gameObject.Update(gameTime);

			Assert::AreEqual(0, gameObject.At("A").Get<int32_t>());

			gameObject.Update(gameTime);

			Assert::AreEqual(-1, gameObject.At("A").Get<int32_t>());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}