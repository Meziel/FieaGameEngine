#include "pch.h"

#include <string>
#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;
using namespace std;

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			Factory<Scope>::Add(make_unique<ScopeFactory>());
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
			Scope scope;
			Assert::IsNull(scope.GetParent());

			Scope scope2(20);
			Assert::IsNull(scope2.GetParent());
		}

		TEST_METHOD(CopySemantics)
		{
			// Scope structure
			// Scope
			//   - Scope child 1
			//     - [Scope, Scope]
			//     - Scope
			//   - Scope child 2
			//   - string datum

			// Build out initial scope
			Scope scope;
			// Create children
			Scope& scopeChild1 = scope.AppendScope("childScope1");
			scope.AppendScope("childScope2");
			Datum* scopeChild1Datum = scope.Find("childScope1");
			Datum* scopeChild2Datum = scope.Find("childScope2");
			Assert::IsNotNull(scopeChild1Datum);
			Assert::IsNotNull(scopeChild2Datum);
			Datum& stringChildDatum = scope.Append("string");
			stringChildDatum.PushBack("mystring"s);
			// Create grand children
			scopeChild1.AppendScope("grandChildScope1");
			scopeChild1.AppendScope("grandChildScope1");
			scopeChild1.AppendScope("grandChildScope2");

			// Copy
			Scope scopeCopy(scope);
			Assert::IsTrue(scope == scopeCopy);
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				Scope tempScope;
				Scope& nestedScope = tempScope.AppendScope("scope");
				Scope scope = std::move(tempScope);
				auto [datum, index] = scope.FindContainedScope(nestedScope);
				Assert::IsNotNull(datum);
				Assert::IsNotNull(nestedScope.GetParent());
				Assert::AreEqual(&scope, nestedScope.GetParent());
				// Check tmp was cleared
#pragma warning(push)
#pragma warning(disable:26800)
				auto [tempDatum, tempIndex] = tempScope.FindContainedScope(nestedScope);
				Assert::IsNull(tempDatum);
#pragma warning(pop)
			}

			// Moving something with parent
			{
				Scope parentScope;
				Scope& nestedScope = parentScope.AppendScope("scope");
				Scope scope = std::move(nestedScope);
				Assert::IsNull(scope.GetParent());
				// Check tmp was cleared
#pragma warning(push)
#pragma warning(disable:26800)
				auto [tempDatum, tempIndex] = parentScope.FindContainedScope(nestedScope);
				Assert::IsNull(tempDatum);
#pragma warning(pop)
			}
		}

		TEST_METHOD(CopyAssignment)
		{
			Scope scope{};
			scope.AppendScope("scope1");
			Datum& stringDatum = scope.Append("string");
			stringDatum.PushBack("test"s);

			Scope differentScope;
			Scope& differentNestedScope = differentScope.AppendScope("differentNestedScope");
			Datum& differentStringDatum = differentScope.Append("differentString");
			differentStringDatum.PushBack("test2"s);

			scope = differentScope;
			Datum* result;
			result = scope.Find("scope1");
			Assert::IsNull(result);
			result = scope.Find("string");
			Assert::IsNull(result);
			result = scope.Find("differentNestedScope");
			Assert::IsNotNull(result);
			Assert::AreEqual(1_z, result->Size());
			Assert::AreEqual(differentNestedScope, *result->Get<shared_ptr<Scope>>(0));
			result = scope.Find("differentString");
			Assert::IsNotNull(result);
			Assert::AreEqual(1_z, result->Size());
			Assert::AreEqual("test2"s, result->Get<string>(0));
		}

		TEST_METHOD(MoveAssignment)
		{
			{
				Scope scope{};
				scope.AppendScope("scope1");
				Datum& stringDatum = scope.Append("string");
				stringDatum.PushBack("test"s);

				Scope differentScope;
				Scope& differentNestedScope = differentScope.AppendScope("differentNestedScope");
				Datum& differentStringDatum = differentScope.Append("differentString");
				differentStringDatum.PushBack("test2"s);

				scope = std::move(differentScope);
				Datum* result;
				result = scope.Find("scope1");
				Assert::IsNull(result);
				result = scope.Find("string");
				Assert::IsNull(result);
				result = scope.Find("differentNestedScope");
				Assert::IsNotNull(result);
				Assert::AreEqual(1_z, result->Size());
				Assert::AreEqual(differentNestedScope, *result->Get<shared_ptr<Scope>>(0));
				result = scope.Find("differentString");
				Assert::IsNotNull(result);
				Assert::AreEqual(1_z, result->Size());
				Assert::AreEqual("test2"s, result->Get<string>(0));
			}

			// Move assign something with parent
			{
				Scope scope{};
				scope.AppendScope("scope1");

				Scope differentScope;
				Scope& differentNestedScope = differentScope.AppendScope("differentNestedScope");

				Assert::IsNotNull(differentNestedScope.GetParent());
				scope = std::move(differentNestedScope);
				Assert::IsNull(scope.GetParent());
			}
		}

		TEST_METHOD(Find)
		{
			Scope scope;
			Scope& nestedScope = scope.AppendScope("var1");
			Datum& var2Datum = scope.Append("var2");

			Datum* foundDatum;

			foundDatum = scope.Find("var1");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(DatumTypes::Table, foundDatum->Type());
			Assert::AreEqual(1_z, foundDatum->Size());
			Assert::AreEqual(&nestedScope, foundDatum->Get<shared_ptr<Scope>>().get());

			foundDatum = scope.Find("var2");
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(&var2Datum, foundDatum);

			foundDatum = scope.Find("var3");
			Assert::IsNull(foundDatum);
		}

		TEST_METHOD(Search)
		{
			// With scope pointer
			{
				Scope scope;
				Datum& datum = scope.Append("myInt");
				datum.PushBack(1);
				Scope& nestedScope = scope.AppendScope("nestedScope");

				Scope* foundScope;
				Datum* datumFound;
				datumFound = scope.Search("NotReal", foundScope);
				Assert::IsNull(datumFound);
				Assert::IsNull(foundScope);

				datumFound = scope.Search("myInt", foundScope);
				Assert::IsNotNull(datumFound);
				Assert::IsNotNull(foundScope);
				Assert::AreEqual(&datum, datumFound);

				datumFound = nestedScope.Search("myInt", foundScope);
				Assert::IsNotNull(datumFound);
				Assert::IsNotNull(foundScope);
				Assert::AreEqual(&datum, datumFound);
			}

			// Without scope pointer
			{
				Scope scope;
				Datum& datum = scope.Append("myInt");
				datum.PushBack(1);
				Scope& nestedScope = scope.AppendScope("nestedScope");

				Datum* datumFound;
				datumFound = scope.Search("NotReal");
				Assert::IsNull(datumFound);

				datumFound = scope.Search("myInt");
				Assert::IsNotNull(datumFound);
				Assert::AreEqual(&datum, datumFound);

				datumFound = nestedScope.Search("myInt");
				Assert::IsNotNull(datumFound);
				Assert::AreEqual(&datum, datumFound);
			}
		}

		TEST_METHOD(Append)
		{
			Scope scope;
			
			// Add new datum
			Datum& datumRef = scope.Append("datum");
			Assert::AreEqual(Datum{}, datumRef);

			// Get datum just added
			Datum* datumFound = scope.Find("datum");
			Assert::IsNotNull(datumFound);
			Assert::AreSame(datumRef, *datumFound);

			// Append with same name as before
			Datum& datumRef2 = scope.Append("datum");
			Assert::AreEqual(Datum{}, datumRef);
			Assert::AreSame(datumRef, datumRef2);

			// Append with different name
			Datum& datumRef3 = scope.Append("datum2");
			Assert::AreEqual(Datum{}, datumRef);
			Assert::AreNotSame(datumRef, datumRef3);
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;

			// Add new scope
			Scope& scopeReference = scope.AppendScope("scope1");
			Assert::AreEqual(&scope, scopeReference.GetParent());

			// Test can find it
			auto [scopeDatum, index] = scope.FindContainedScope(scopeReference);
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(0_z, index);
			Assert::AreEqual(1_z, scopeDatum->Size());
			Assert::AreEqual(&scopeReference, scopeDatum->Get<shared_ptr<Scope>>(0).get());

			// Add new scope with same name
			Scope& scopeReference2 = scope.AppendScope("scope1");
			Assert::AreEqual(&scope, scopeReference2.GetParent());
			Assert::AreNotSame(scopeReference, scopeReference2);

			// Test can find it
			auto [scopeDatum2, index2] = scope.FindContainedScope(scopeReference2);
			Assert::IsNotNull(scopeDatum2);
			Assert::AreEqual(scopeDatum, scopeDatum2);
			Assert::AreEqual(1_z, index2);
			Assert::AreEqual(2_z, scopeDatum2->Size());
			Assert::AreEqual(&scopeReference2, scopeDatum->Get<shared_ptr<Scope>>(1).get());

			// Add scope for datum that already exists but isn't type table
			scope.Append("notAScope").SetType(DatumTypes::String);
			Assert::ExpectException<runtime_error>([&scope]() { scope.AppendScope("notAScope"); });
		}

		TEST_METHOD(Adopt)
		{
			shared_ptr<Scope> parentScope1 = make_shared<Scope>();
			shared_ptr<Scope> parentScope2 = make_shared<Scope>();
			shared_ptr<Scope> nestedScope = make_shared<Scope>();

			// Adopt scope with no parent
			parentScope1->Adopt(nestedScope, "scope");
			Datum* nestedDatum1 = parentScope1->Find("scope");
			Assert::IsNotNull(nestedDatum1);
			Assert::AreEqual(DatumTypes::Table, nestedDatum1->Type());
			Assert::AreEqual(1_z, nestedDatum1->Size());
			Assert::AreEqual(nestedScope.get(), nestedDatum1->Get<shared_ptr<Scope>>(0).get());
			Assert::AreEqual(parentScope1.get(), nestedScope->GetParent());

			// Try to adopt self
			Assert::ExpectException<runtime_error>([&parentScope1]() { parentScope1->Adopt(parentScope1, "Self"); });
			// Try to adopt parent
			Assert::ExpectException<runtime_error>([&parentScope1, &nestedScope]() { nestedScope->Adopt(parentScope1, "Parent"); });
			// Try to adopt to same place
			parentScope1->Adopt(nestedScope, "scope");
			Datum* nestedDatum2 = parentScope1->Find("scope");
			Assert::IsNotNull(nestedDatum2);
			Assert::AreEqual(DatumTypes::Table, nestedDatum2->Type());
			Assert::AreEqual(1_z, nestedDatum2->Size());
			Assert::AreEqual(nestedScope.get(), nestedDatum2->Get<shared_ptr<Scope>>(0).get());
			Assert::AreEqual(parentScope1.get(), nestedScope->GetParent());
			// Adopt to different parent
			parentScope2->Adopt(nestedScope, "scope");
			// Check old datum is empty
			Datum* nestedDatum3 = parentScope1->Find("scope");
			Assert::IsNotNull(nestedDatum3);
			Assert::AreEqual(DatumTypes::Table, nestedDatum2->Type());
			Assert::AreEqual(0_z, nestedDatum2->Size());
			Datum* nestedDatum4 = parentScope2->Find("scope");
			Assert::IsNotNull(nestedDatum4);
			Assert::AreEqual(DatumTypes::Table, nestedDatum4->Type());
			Assert::AreEqual(1_z, nestedDatum4->Size());
			Assert::AreEqual(nestedScope.get(), nestedDatum4->Get<shared_ptr<Scope>>(0).get());
			Assert::AreEqual(parentScope2.get(), nestedScope->GetParent());

			// Adopt to a datum that is not a table
			Datum& notTableDatum = parentScope1->Append("notTable");
			notTableDatum.SetType(DatumTypes::Float);
			Assert::ExpectException<runtime_error>([&parentScope1, &nestedScope]() { parentScope1->Adopt(nestedScope, "notTable"); });
		}

		TEST_METHOD(IsAncestor)
		{
			shared_ptr<Scope> parentScope = make_shared<Scope>();
			shared_ptr<Scope> nestedScope = make_shared<Scope>();
			shared_ptr<Scope> nestedNestedScope = make_shared<Scope>();

			Assert::IsFalse(nestedScope->IsAncestor(*parentScope));
			Assert::IsFalse(parentScope->IsAncestor(*parentScope));
			parentScope->Adopt(nestedScope, "scope");
			Assert::IsTrue(nestedScope->IsAncestor(*parentScope));
			Assert::IsFalse(parentScope->IsAncestor(*nestedScope));
			Assert::IsFalse(parentScope->IsAncestor(*parentScope));
			nestedScope->Adopt(nestedNestedScope, "scope");
			Assert::IsTrue(nestedNestedScope->IsAncestor(*nestedScope));
			Assert::IsTrue(nestedNestedScope->IsAncestor(*parentScope));
		}

		TEST_METHOD(IsDescendent)
		{
			shared_ptr<Scope> parentScope = make_shared<Scope>();
			shared_ptr<Scope> nestedScope = make_shared<Scope>();
			shared_ptr<Scope> nestedNestedScope = make_shared<Scope>();

			Assert::IsFalse(parentScope->IsDescendent(*nestedScope));
			Assert::IsFalse(parentScope->IsDescendent(*parentScope));
			parentScope->Adopt(nestedScope, "scope");
			Assert::IsTrue(parentScope->IsDescendent(*nestedScope));
			Assert::IsFalse(nestedScope->IsDescendent(*parentScope));
			Assert::IsFalse(parentScope->IsDescendent(*parentScope));
			nestedScope->Adopt(nestedNestedScope, "scope");
			Assert::IsTrue(nestedScope->IsDescendent(*nestedNestedScope));
			Assert::IsTrue(parentScope->IsDescendent(*nestedNestedScope));
		}

		TEST_METHOD(GetParent)
		{
			shared_ptr<Scope> parentScope = make_shared<Scope>();
			shared_ptr<Scope> nestedScope = make_shared<Scope>();

			Assert::IsNull(parentScope->GetParent());
			Assert::IsNull(nestedScope->GetParent());
			parentScope->Adopt(nestedScope, "scope");
			Assert::IsNull(parentScope->GetParent());
			Assert::IsNotNull(nestedScope->GetParent());
			Assert::AreEqual(parentScope.get(), nestedScope->GetParent());
		}

		TEST_METHOD(OperatorBracketBracket)
		{
			// Key
			{
				Scope scope;

				// Add new datum
				Datum& datumRef = scope["datum"];
				Assert::AreEqual(Datum{}, datumRef);

				// Get datum just added
				Datum* datumFound = scope.Find("datum");
				Assert::IsNotNull(datumFound);
				Assert::AreSame(datumRef, *datumFound);

				// Append with same name as before
				Datum& datumRef2 = scope["datum"];
				Assert::AreEqual(Datum{}, datumRef);
				Assert::AreSame(datumRef, datumRef2);

				// Append with different name
				Datum& datumRef3 = scope["datum2"];
				Assert::AreEqual(Datum{}, datumRef);
				Assert::AreNotSame(datumRef, datumRef3);
			}

			// Index
			{
				Scope scope;
				Scope& nestedScope = scope.AppendScope("var1");
				Datum& var2Datum = scope.Append("var2");
				auto [nestedScopeDatum, nestedScopeIndex] = scope.FindContainedScope(nestedScope);
				Assert::IsNotNull(nestedScopeDatum);

				Assert::AreEqual(nestedScopeDatum, &scope[0]);
				Assert::AreEqual(&var2Datum, &scope[1]);

				Assert::ExpectException<runtime_error>([&scope]() { Datum& temp = scope[2]; UNREFERENCED_LOCAL(temp);  });
			}
		}

		TEST_METHOD(Comparison)
		{
			Scope scope;
			Assert::AreEqual(scope, scope);
			Assert::IsFalse(scope != scope);
			Assert::IsTrue(scope.Equals(&scope));
			Assert::IsFalse(scope.Equals(nullptr));
			Foo foo(1);
			Assert::IsFalse(scope.Equals(&foo));

			Scope scope1;
			Assert::AreEqual(scope, scope1);
			Assert::IsFalse(scope != scope1);
			Assert::IsTrue(scope.Equals(&scope1));

			// Add to scope 1
			Scope& nestedScope = scope.AppendScope("nested");
			Assert::AreNotEqual(scope, scope1);
			Assert::IsTrue(scope != scope1);
			Assert::IsFalse(scope.Equals(&scope1));

			// Add to scope 2
			Scope& nestedScope2 = scope1.AppendScope("nested");
			Assert::AreEqual(scope, scope1);
			Assert::IsFalse(scope != scope1);
			Assert::IsTrue(scope.Equals(&scope1));

			Datum& d1 = nestedScope.Append("test");
			Datum& d2 = nestedScope2.Append("test");
			Assert::AreEqual(scope, scope1);
			Assert::IsFalse(scope != scope1);
			Assert::IsTrue(scope.Equals(&scope1));

			d1.PushBack("string1test"s);
			d2.PushBack("string1test"s);
			Assert::AreEqual(scope, scope1);
			Assert::IsFalse(scope != scope1);
			Assert::IsTrue(scope.Equals(&scope1));

			d1.PushBack("string2test"s);
			Assert::AreNotEqual(scope, scope1);
			Assert::IsTrue(scope != scope1);
			Assert::IsFalse(scope.Equals(&scope1));
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}