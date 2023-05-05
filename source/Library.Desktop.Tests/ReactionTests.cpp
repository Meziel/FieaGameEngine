#include "pch.h"

#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "Game.h"
#include "GameObject.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "ActionIncrement.h"
#include "ActionEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace LibraryDesktopTests
{
	TEST_CLASS(ReactionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(std::make_unique<GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<ActionEventFactory>());
			Factory<Scope>::Add(std::make_unique<ReactionAttributedFactory>());
			Factory<Scope>::Add(std::make_unique<ActionListFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			// TODO: why do i need to reset twice??
			Game::Reset();
			//Game::Reset();
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
			//// Reaction Attributed
			//ReactionAttributed reactionAttributed;
			//Assert::AreEqual(0, reactionAttributed.Subtype);

			//// ActionEvent
			//ActionEvent actionEvent;
			//Assert::AreEqual(0, actionEvent.Subtype);
			//Assert::AreEqual(0, actionEvent.Delay);
		}

		//TEST_METHOD(CopySemantics)
		//{
		//	//Reaction Attributed
		//	ReactionAttributed reactionAttributed;
		//	reactionAttributed.Subtype = 5;

		//	ReactionAttributed reactionAttributedCopy = reactionAttributed;
		//	Assert::AreEqual(5, reactionAttributedCopy.Subtype);

		//	// ActionEvent
		//	ActionEvent actionEvent;
		//	actionEvent.Subtype = 5;
		//	actionEvent.Delay = 20;

		//	ActionEvent actionEventCopy = actionEvent;

		//	Assert::AreEqual(5, actionEventCopy.Subtype);
		//	Assert::AreEqual(20, actionEventCopy.Delay);
		//}

		TEST_METHOD(Update)
		{
			Game& game = Game::Instance();

			game.Root = make_unique<GameObject>();
			GameObject& gameObject = *game.Root;

			// Add 'A' to root
			Datum& a = gameObject.AppendAuxilaryAttribute("A");
			a = 0;

			// Create action event
			ActionEvent& actionEvent = *game.Root->CreateAction(ActionEvent::TypeName(), "Enqueue event").As<ActionEvent>();
			actionEvent.Subtype = 0;
			Datum& step = actionEvent.AppendAuxilaryAttribute("Step");
			step = 5;

			// Create reaction
			ReactionAttributed& reaction = *gameObject.AppendScope("Reactions", ReactionAttributed::TypeName()).As<ReactionAttributed>();
			ActionIncrement& actionIncrement = *reaction.CreateAction(ActionIncrement::TypeName(), "Increment A").As<ActionIncrement>();
			actionIncrement.Target = "A"s;
			reaction.Subtype = 0;

			gameObject.AppendScope("Reactions", ReactionAttributed::TypeName());

			Assert::AreEqual(0, a.Get<int32_t>());
			Assert::AreEqual(0_z, actionIncrement.At("Step").Size());

			game.Update();

			Assert::AreEqual(0, a.Get<int32_t>());
			Assert::AreEqual(0_z, actionIncrement.At("Step").Size());

			game.Update();

			Assert::AreEqual("A"s, actionIncrement.Target);
			Assert::AreEqual(1_z, actionIncrement.At("Step").Size());
			Assert::AreEqual(5, actionIncrement.At("Step").Get<int32_t>());
			Assert::AreEqual(5, a.Get<int32_t>());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}