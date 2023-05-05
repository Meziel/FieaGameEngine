#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "AttributedFoo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace LibraryDesktopTests
{

	TEST_CLASS(FactoryTests)
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
			Factory<RTTI>::Clear();
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

		TEST_METHOD(AddingFacotories)
		{
			// Add Foo factory
			{
				unique_ptr<Factory<RTTI>> factory = make_unique<FooFactory>();
				Factory<RTTI>* factoryPointer = factory.get();

				Factory<RTTI>* factoryPointerFound;

				factoryPointerFound = Factory<RTTI>::Find("Foo");
				Assert::IsNull(factoryPointerFound);

				Factory<RTTI>::Add(std::move(factory));

				factoryPointerFound = Factory<RTTI>::Find("Foo");
				Assert::IsNotNull(factoryPointerFound);
				Assert::AreEqual(factoryPointer, factoryPointerFound);
			}

			// Add AttributedFoo factory
			{
				unique_ptr<Factory<Scope>> factory = make_unique<AttributedFooFactory>();
				Factory<Scope>* factoryPointer = factory.get();

				Factory<Scope>* factoryPointerFound;

				factoryPointerFound = Factory<Scope>::Find("AttributedFoo");
				Assert::IsNull(factoryPointerFound);

				Factory<Scope>::Add(std::move(factory));

				factoryPointerFound = Factory<Scope>::Find("AttributedFoo");
				Assert::IsNotNull(factoryPointerFound);
				Assert::AreEqual(factoryPointer, factoryPointerFound);
			}

			// Add Scope factory
			{
				unique_ptr<Factory<Scope>> factory = make_unique<ScopeFactory>();
				Factory<Scope>* factoryPointer = factory.get();

				Factory<Scope>* factoryPointerFound;

				factoryPointerFound = Factory<Scope>::Find("Scope");
				Assert::IsNull(factoryPointerFound);

				Factory<Scope>::Add(std::move(factory));

				factoryPointerFound = Factory<Scope>::Find("Scope");
				Assert::IsNotNull(factoryPointerFound);
				Assert::AreEqual(factoryPointer, factoryPointerFound);
			}
		}

		TEST_METHOD(RemovingFacotories)
		{
			// Add Foo factory
			{
				unique_ptr<Factory<RTTI>> factory = make_unique<FooFactory>();
				Factory<RTTI>* factoryPointer = factory.get();
				Factory<RTTI>::Add(std::move(factory));
				Factory<RTTI>* factoryPointerFound;

				factoryPointerFound = Factory<RTTI>::Find("Foo");
				Assert::IsNotNull(factoryPointerFound);
				
				Factory<RTTI>::Remove(*factoryPointer);

				factoryPointerFound = Factory<RTTI>::Find("Foo");
				Assert::IsNull(factoryPointerFound);
			}

			// Add AttributedFoo factory
			{
				unique_ptr<Factory<Scope>> factory = make_unique<AttributedFooFactory>();
				Factory<Scope>* factoryPointer = factory.get();
				Factory<Scope>::Add(std::move(factory));
				Factory<Scope>* factoryPointerFound;

				factoryPointerFound = Factory<Scope>::Find("AttributedFoo");
				Assert::IsNotNull(factoryPointerFound);

				Factory<Scope>::Remove(*factoryPointer);

				factoryPointerFound = Factory<Scope>::Find("AttributedFoo");
				Assert::IsNull(factoryPointerFound);
			}

			// Add Scope factory
			{
				unique_ptr<Factory<Scope>> factory = make_unique<ScopeFactory>();
				Factory<Scope>* factoryPointer = factory.get();
				Factory<Scope>::Add(std::move(factory));
				Factory<Scope>* factoryPointerFound;

				factoryPointerFound = Factory<Scope>::Find("Scope");
				Assert::IsNotNull(factoryPointerFound);

				Factory<Scope>::Remove(*factoryPointer);

				factoryPointerFound = Factory<Scope>::Find("Scope");
				Assert::IsNull(factoryPointerFound);
			}
		}

		TEST_METHOD(Create)
		{
			// Add Foo factory
			{
				unique_ptr<RTTI> createdObject;

				createdObject = Factory<RTTI>::Create("Foo");
				Assert::IsNull(createdObject.get());

				Factory<RTTI>::Add(make_unique<FooFactory>());

				createdObject = Factory<RTTI>::Create("Foo");
				Assert::IsNotNull(createdObject.get());
				Assert::AreEqual(Foo::TypeIdClass(), createdObject->TypeIdInstance());
			}

			// Add AttributedFoo factory
			{
				unique_ptr<Scope> createdObject;

				createdObject = Factory<Scope>::Create("AttributedFoo");
				Assert::IsNull(createdObject.get());

				Factory<Scope>::Add(make_unique<AttributedFooFactory>());

				createdObject = Factory<Scope>::Create("AttributedFoo");
				Assert::IsNotNull(createdObject.get());
				Assert::AreEqual(AttributedFoo::TypeIdClass(), createdObject->TypeIdInstance());
			}

			// Add Scope factory
			{
				unique_ptr<Scope> createdObject;

				createdObject = Factory<Scope>::Create("Scope");
				Assert::IsNull(createdObject.get());

				Factory<Scope>::Add(make_unique<ScopeFactory>());

				createdObject = Factory<Scope>::Create("Scope");
				Assert::IsNotNull(createdObject.get());
				Assert::AreEqual(Scope::TypeIdClass(), createdObject->TypeIdInstance());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}