#include "pch.h"
#include "CppUnitTest.h"
#include "Literals.h"
#include "ToStringSpecializations.h"
#include "Monster.h"
#include "ActionIncrement.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include <stdexcept>

using namespace UnitTests;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace glm;
using namespace std;

namespace LibraryDesktopTests
{

	TEST_CLASS(GameObjectTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		
			Factory<Scope>::Add(make_unique<ScopeFactory>());
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<MonsterFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Clear();
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

		TEST_METHOD(Constructor)
		{
			// GameObject construct
			{
				GameObject gameObject;
				Assert::IsTrue(gameObject.IsPrescribedAttribute("Name"));
				Assert::IsTrue(gameObject.IsPrescribedAttribute("Transform"));
				Assert::IsTrue(gameObject.IsPrescribedAttribute("Children"));
				Assert::IsTrue(gameObject.IsPrescribedAttribute("Actions"));

				Assert::AreEqual(DatumTypes::String, gameObject.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, gameObject.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, gameObject.Find("Children")->Type());
				Assert::AreEqual(DatumTypes::Table, gameObject.Find("Actions")->Type());

				Assert::AreSame(gameObject.At("Children"), gameObject.Children());
				Assert::AreSame(gameObject.At("Actions"), gameObject.Actions());
				Assert::AreSame(gameObject.At("Children"), const_cast<const GameObject&>(gameObject).Children());
				Assert::AreSame(gameObject.At("Actions"), const_cast<const GameObject&>(gameObject).Actions());
			}

			// Monster construct
			{
				Monster monster;
				Assert::IsTrue(monster.IsPrescribedAttribute("Name"));
				Assert::IsTrue(monster.IsPrescribedAttribute("Transform"));
				Assert::IsTrue(monster.IsPrescribedAttribute("Children"));
				Assert::IsTrue(monster.IsPrescribedAttribute("Actions"));

				Assert::AreEqual(DatumTypes::String, monster.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, monster.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, monster.Find("Children")->Type());
				Assert::AreEqual(DatumTypes::Table, monster.Find("Actions")->Type());
				Assert::AreEqual(0_z, monster.Counter);
			}
		}

		TEST_METHOD(CopySemantics)
		{
			// GameObject copy construct
			{
				GameObject gameObject;
				gameObject.Name = "GameObject";
				gameObject.Transform.Position = glm::vec4(1);
				gameObject.Transform.Rotation = glm::vec4(2);
				gameObject.Transform.Scale = glm::vec4(3);

				GameObject gameObjectCopy = gameObject;

				Assert::AreEqual(DatumTypes::String, gameObjectCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, gameObjectCopy.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, gameObjectCopy.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, gameObjectCopy.Name);
				Assert::AreEqual(glm::vec4(1), gameObjectCopy.Transform.Position);
				Assert::AreEqual(glm::vec4(2), gameObjectCopy.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), gameObjectCopy.Transform.Scale);
			}

			// Monster copy construct
			{
				Monster monster;
				monster.Name = "GameObject";
				monster.Transform.Position = glm::vec4(1);
				monster.Transform.Rotation = glm::vec4(2);
				monster.Transform.Scale = glm::vec4(3);
				monster.Counter = 1;

				Monster monsterCopy = monster;

				Assert::AreEqual(DatumTypes::String, monsterCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, monsterCopy.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, monsterCopy.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, monsterCopy.Name);
				Assert::AreEqual(glm::vec4(1), monsterCopy.Transform.Position);
				Assert::AreEqual(glm::vec4(2), monsterCopy.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), monsterCopy.Transform.Scale);
				Assert::AreEqual(1_z, monsterCopy.Counter);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// GameObject move construct
			{
				GameObject gameObject;
				gameObject.Name = "GameObject";
				gameObject.Transform.Position = glm::vec4(1);
				gameObject.Transform.Rotation = glm::vec4(2);
				gameObject.Transform.Scale = glm::vec4(3);

				GameObject gameObjectCopy = std::move(gameObject);

				Assert::AreEqual(DatumTypes::String, gameObjectCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, gameObjectCopy.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, gameObjectCopy.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, gameObjectCopy.Name);
				Assert::AreEqual(glm::vec4(1), gameObjectCopy.Transform.Position);
				Assert::AreEqual(glm::vec4(2), gameObjectCopy.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), gameObjectCopy.Transform.Scale);
			}

			// Monster move construct
			{
				Monster monster;
				monster.Name = "GameObject";
				monster.Transform.Position = glm::vec4(1);
				monster.Transform.Rotation = glm::vec4(2);
				monster.Transform.Scale = glm::vec4(3);
				monster.Counter = 1;

				Monster monsterCopy = std::move(monster);

				Assert::AreEqual(DatumTypes::String, monsterCopy.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, monsterCopy.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, monsterCopy.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, monsterCopy.Name);
				Assert::AreEqual(glm::vec4(1), monsterCopy.Transform.Position);
				Assert::AreEqual(glm::vec4(2), monsterCopy.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), monsterCopy.Transform.Scale);
				Assert::AreEqual(1_z, monsterCopy.Counter);
			}
		}

		TEST_METHOD(CreateAction)
		{
			GameObject gameObject;
			Assert::ExpectException<runtime_error>([&gameObject]() { gameObject.CreateAction("BadAction"); });
			
			gameObject.CreateAction("ActionIncrement");
			Assert::AreEqual(1_z, gameObject.Actions().Size());
			Assert::AreEqual(DatumTypes::Table, gameObject.Actions().Type());
			Assert::IsTrue(gameObject.Actions().Get<shared_ptr<Scope>>()->Is(ActionIncrement::TypeIdClass()));
		}

		TEST_METHOD(Update)
		{
			Monster monster;
			Assert::AreEqual(0_z, monster.Counter);
			monster.Update({});
			Assert::AreEqual(1_z, monster.Counter);

			monster.AppendScope("Children", "Monster");
			monster.AppendScope("Children", "Monster");
			monster.AppendScope("Children", "Monster");

			Datum* children = monster.Find("Children");
			Assert::AreEqual(3_z, children->Size());

			for (size_t i = 0; i < children->Size(); ++i)
			{
				Scope& childMonster = *children->Get<std::shared_ptr<Scope>>(i);
				Assert::IsTrue(childMonster.Is(Monster::TypeIdClass()));
				Assert::AreEqual(0_z, childMonster.As<Monster>()->Counter);
			}

			monster.Update({});
			monster.Update({});

			Assert::AreEqual(3_z, monster.Counter);

			for (size_t i = 0; i < children->Size(); ++i)
			{
				Scope& childMonster = *children->Get<std::shared_ptr<Scope>>(i);
				Assert::IsTrue(childMonster.Is(Monster::TypeIdClass()));
				Assert::AreEqual(2_z, childMonster.As<Monster>()->Counter);
			}
		}

		TEST_METHOD(GameObjectClone)
		{
			// GameObject clone
			{
				GameObject gameObject;
				gameObject.Name = "GameObject";
				gameObject.Transform.Position = glm::vec4(1);
				gameObject.Transform.Rotation = glm::vec4(2);
				gameObject.Transform.Scale = glm::vec4(3);			
				Assert::AreEqual(GameObject::TypeIdClass(), gameObject.TypeIdInstance());

				// Clone
				unique_ptr<Scope> gameObjectClone = gameObject.Clone();

				Assert::AreEqual(GameObject::TypeIdClass(), gameObjectClone->TypeIdInstance());
				Assert::AreNotEqual(static_cast<Scope*>(&gameObject), gameObjectClone.get());

				GameObject& gameObjectCloneAsMonster = *gameObjectClone->As<GameObject>();

				Assert::AreEqual(DatumTypes::String, gameObjectCloneAsMonster.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, gameObjectCloneAsMonster.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, gameObjectCloneAsMonster.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, gameObjectCloneAsMonster.Name);
				Assert::AreEqual(glm::vec4(1), gameObjectCloneAsMonster.Transform.Position);
				Assert::AreEqual(glm::vec4(2), gameObjectCloneAsMonster.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), gameObjectCloneAsMonster.Transform.Scale);
			}

			// Monster clone
			{
				Monster monster;
				monster.Name = "GameObject";
				monster.Transform.Position = glm::vec4(1);
				monster.Transform.Rotation = glm::vec4(2);
				monster.Transform.Scale = glm::vec4(3);	
				monster.Counter = 1;
				
				Assert::AreEqual(Monster::TypeIdClass(), monster.TypeIdInstance());

				unique_ptr<Scope> monsterClone = monster.Clone();

				Assert::AreEqual(Monster::TypeIdClass(), monsterClone->TypeIdInstance());
				Assert::AreEqual(1_z, monsterClone->As<Monster>()->Counter);
				Assert::AreNotEqual(static_cast<Scope*>(&monster), monsterClone.get());

				Monster& monsterCloneAsMonster = *monsterClone->As<Monster>();

				Assert::AreEqual(DatumTypes::String, monsterCloneAsMonster.Find("Name")->Type());
				Assert::AreEqual(DatumTypes::TablePointer, monsterCloneAsMonster.Find("Transform")->Type());
				Assert::AreEqual(DatumTypes::Table, monsterCloneAsMonster.Find("Children")->Type());
				Assert::AreEqual("GameObject"s, monsterCloneAsMonster.Name);
				Assert::AreEqual(glm::vec4(1), monsterCloneAsMonster.Transform.Position);
				Assert::AreEqual(glm::vec4(2), monsterCloneAsMonster.Transform.Rotation);
				Assert::AreEqual(glm::vec4(3), monsterCloneAsMonster.Transform.Scale);
				Assert::AreEqual(1_z, monsterCloneAsMonster.Counter);
			}
		}

		TEST_METHOD(Deserialize)
		{
			shared_ptr<JsonTableParseHelper::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>();
			wrapper->Data = make_shared<Monster>();
			JsonParseCoordinator parseCoordinator(wrapper);
			shared_ptr<JsonTableParseHelper> helper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(helper);
			parseCoordinator.DeserializeObjectFromFile("JsonFiles/GameObjectTest.json"s);

			Monster& monster = static_cast<Monster&>(*wrapper->Data);
			Assert::AreEqual("Monster1"s, monster.Name);
			
			// Transform check
			Assert::AreEqual(glm::vec4(1, 2, 3, 0), monster.Transform.Position);
			Assert::AreEqual(glm::vec4(1, 2, 3, 0), monster.Transform.Rotation);
			Assert::AreEqual(glm::vec4(1, 2, 3, 0), monster.Transform.Scale);
			
			// Health check
			Assert::IsNotNull(monster.Find("Health"));
			Assert::AreEqual(DatumTypes::Integer, monster["Health"].Type());
			Assert::AreEqual(1_z, monster["Health"].Size());
			Assert::AreEqual(100, monster["Health"].Front<int32_t>());

			// Children check
			Assert::IsNotNull(monster.Find("Children"));
			Assert::AreEqual(DatumTypes::Table, monster["Children"].Type());
			Assert::AreEqual(2_z, monster["Children"].Size());

			Scope& child1AsScope = *monster["Children"].Get<shared_ptr<Scope>>(0);
			Assert::AreEqual(GameObject::TypeIdClass(), child1AsScope.TypeIdInstance());
			GameObject* childMonster1 = child1AsScope.As<GameObject>();
			Assert::AreEqual("Child1"s, childMonster1->Name);

			Scope& child2AsScope = *monster["Children"].Get<shared_ptr<Scope>>(1);
			Assert::AreEqual(GameObject::TypeIdClass(), child2AsScope.TypeIdInstance());
			GameObject* childMonster2 = child2AsScope.As<GameObject>();
			Assert::AreEqual("Child2"s, childMonster2->Name);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}