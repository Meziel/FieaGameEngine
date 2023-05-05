#include "pch.h"

#include <string>
#include <set>
#include "Literals.h"
#include "ToStringSpecializations.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedFooTests)
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
			AttributedFoo attributedFoo;

			Assert::IsTrue(attributedFoo.IsAttribute("this"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalScope"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
		
			Datum& thisDatum = attributedFoo["this"];
			Datum& externalIntegerDatum = attributedFoo["ExternalInteger"];
			Datum& externalFloatDatum = attributedFoo["ExternalFloat"];
			Datum& externalStringDatum = attributedFoo["ExternalString"];
			Datum& externalVectorDatum = attributedFoo["ExternalVector"];
			Datum& externalMatrixDatum = attributedFoo["ExternalMatrix"];
			Datum& externalScopeDatum = attributedFoo["ExternalScope"];

			Datum& externalIntegerArrayDatum = attributedFoo["ExternalIntegerArray"];
			Datum& externalFloatArrayDatum = attributedFoo["ExternalFloatArray"];
			Datum& externalStringArrayDatum = attributedFoo["ExternalStringArray"];
			Datum& externalVectorArrayDatum = attributedFoo["ExternalVectorArray"];
			Datum& externalMatrixArrayDatum = attributedFoo["ExternalMatrixArray"];

			Assert::AreEqual(1_z, thisDatum.Size());
			Assert::AreEqual(1_z, externalIntegerDatum.Size());
			Assert::AreEqual(1_z, externalFloatDatum.Size());
			Assert::AreEqual(1_z, externalStringDatum.Size());
			Assert::AreEqual(1_z, externalVectorDatum.Size());
			Assert::AreEqual(1_z, externalMatrixDatum.Size());
			Assert::AreEqual(1_z, externalScopeDatum.Size());

			Assert::AreEqual(2_z, externalIntegerArrayDatum.Size());
			Assert::AreEqual(2_z, externalFloatArrayDatum.Size());
			Assert::AreEqual(2_z, externalStringArrayDatum.Size());
			Assert::AreEqual(2_z, externalVectorArrayDatum.Size());
			Assert::AreEqual(2_z, externalMatrixArrayDatum.Size());

			Assert::AreEqual(static_cast<RTTI*>(&attributedFoo), thisDatum.Get<RTTI*>());
			Assert::AreSame(attributedFoo.ExternalInteger, externalIntegerDatum.Get<int32_t>(0));
			Assert::AreSame(attributedFoo.ExternalFloat, externalFloatDatum.Get<float>(0));
			Assert::AreSame(attributedFoo.ExternalString, externalStringDatum.Get<string>(0));
			Assert::AreSame(attributedFoo.ExternalVector, externalVectorDatum.Get<glm::vec4>(0));
			Assert::AreSame(attributedFoo.ExternalMatrix, externalMatrixDatum.Get<glm::mat4>(0));

			Assert::AreEqual(attributedFoo.ExternalIntegerArray.data(), &externalIntegerArrayDatum.Get<int32_t>(0));
			Assert::AreEqual(attributedFoo.ExternalFloatArray.data(), &externalFloatArrayDatum.Get<float>(0));
			Assert::AreEqual(attributedFoo.ExternalStringArray.data(), &externalStringArrayDatum.Get<string>(0));
			Assert::AreEqual(attributedFoo.ExternalVectorArray.data(), &externalVectorArrayDatum.Get<glm::vec4>(0));
			Assert::AreEqual(attributedFoo.ExternalMatrixArray.data(), &externalMatrixArrayDatum.Get<glm::mat4>(0));
		}

		TEST_METHOD(CopySemantics)
		{
			// Copy constructor
			{
				AttributedFoo attributedFoo;
				Datum& aux1Datum = attributedFoo.AppendAuxilaryAttribute("Aux1");
				Datum& aux2Datum = attributedFoo.AppendAuxilaryAttribute("Aux2");

				aux1Datum.PushBack(1);
				aux2Datum.PushBack(1.0f);

				AttributedFoo attributedFooCopy = attributedFoo;

				Assert::IsTrue(attributedFoo == attributedFooCopy);
				Assert::AreEqual(aux1Datum, attributedFoo["Aux1"]);
				Assert::AreEqual(aux2Datum, attributedFoo["Aux2"]);
			}

			// Copy assignment
			{
				AttributedFoo attributedFoo;
				Datum& foo1Aux1Datum = attributedFoo.AppendAuxilaryAttribute("Foo1Aux1");
				Datum& foo1Aux2Datum = attributedFoo.AppendAuxilaryAttribute("Foo1Aux2");

				foo1Aux1Datum.PushBack(1);
				foo1Aux2Datum.PushBack(1.0f);

				Assert::IsNotNull(attributedFoo.Find("Foo1Aux1"));
				Assert::IsNotNull(attributedFoo.Find("Foo1Aux2"));

				AttributedFoo attributedFoo2;
				Datum& foo2Aux1Datum = attributedFoo2.AppendAuxilaryAttribute("Foo2Aux1");
				Datum& foo2Aux2Datum = attributedFoo2.AppendAuxilaryAttribute("Foo2Aux2");
				
				foo2Aux1Datum.PushBack(2);
				foo2Aux2Datum.PushBack(2.0f);

				attributedFoo = attributedFoo2;

				Assert::IsTrue(attributedFoo == attributedFoo2);
				Assert::IsNull(attributedFoo.Find("Foo1Aux1"));
				Assert::IsNull(attributedFoo.Find("Foo1Aux2"));
				Assert::IsNotNull(attributedFoo.Find("Foo2Aux1"));
				Assert::IsNotNull(attributedFoo.Find("Foo2Aux2"));
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			// Move constructor
			{
				AttributedFoo attributedFoo;
				Datum& aux1Datum = attributedFoo.AppendAuxilaryAttribute("Aux1");
				Datum& aux2Datum = attributedFoo.AppendAuxilaryAttribute("Aux2");

				aux1Datum.PushBack(1);
				aux2Datum.PushBack(1.0f);

				AttributedFoo attributedFooCopy = std::move(attributedFoo);

				Assert::IsTrue(attributedFooCopy.IsAuxilaryAttribute("Aux1"));
				Assert::IsTrue(attributedFooCopy.IsAuxilaryAttribute("Aux2"));

				Assert::AreEqual(aux1Datum, attributedFooCopy.At("Aux1"));
				Assert::AreEqual(aux2Datum, attributedFooCopy.At("Aux2"));

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsNull(attributedFoo.Find("Aux1"));
				Assert::IsNull(attributedFoo.Find("Aux2"));
#pragma warning(pop)
			}

			// Move assignment
			{
				// Create attributedFoo
				AttributedFoo attributedFoo;
				Datum& foo1Aux1Datum = attributedFoo.AppendAuxilaryAttribute("Foo1Aux1");
				Datum& foo1Aux2Datum = attributedFoo.AppendAuxilaryAttribute("Foo1Aux2");

				foo1Aux1Datum.PushBack(1);
				foo1Aux2Datum.PushBack(1.0f);

				Assert::IsNotNull(attributedFoo.Find("Foo1Aux1"));
				Assert::IsNotNull(attributedFoo.Find("Foo1Aux2"));

				// Create attributedFoo2
				AttributedFoo attributedFoo2;
				Datum& foo2Aux1Datum = attributedFoo2.AppendAuxilaryAttribute("Foo2Aux1");
				Datum& foo2Aux2Datum = attributedFoo2.AppendAuxilaryAttribute("Foo2Aux2");

				foo2Aux1Datum.PushBack(2);
				foo2Aux2Datum.PushBack(2.0f);

				// Move attributedFoo
				attributedFoo = std::move(attributedFoo2);

				Assert::IsNull(attributedFoo.Find("Foo1Aux1"));
				Assert::IsNull(attributedFoo.Find("Foo1Aux2"));
				Assert::IsNotNull(attributedFoo.Find("Foo2Aux1"));
				Assert::IsNotNull(attributedFoo.Find("Foo2Aux2"));

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsNull(attributedFoo.Find("Foo1Aux1"));
				Assert::IsNull(attributedFoo.Find("Foo1Aux2"));
#pragma warning(pop)
			}
		}

		TEST_METHOD(IsAttribute)
		{
			AttributedFoo attributedFoo;

			Assert::IsTrue(attributedFoo.IsAttribute("this"));

			// Prescribed
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalString"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalVector"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalScope"));

			Assert::IsTrue(attributedFoo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("ExternalMatrixArray"));

			// Check non-existent
			Assert::IsFalse(attributedFoo.IsAttribute("NonExistent"));
			Assert::IsFalse(attributedFoo.IsAttribute("AuxInt"));

			// Add aux
			attributedFoo.AppendAuxilaryAttribute("AuxInt");
			Assert::IsTrue(attributedFoo.IsAttribute("AuxInt"));
		}

		TEST_METHOD(IsPrescribedAttribute)
		{
			AttributedFoo attributedFoo;

			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("this"));

			// Prescribed
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalScope"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));

			// Check non-existent
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("NonExistent"));
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("AuxInt"));

			// Add aux
			attributedFoo.AppendAuxilaryAttribute("AuxInt");
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("AuxInt"));
		}

		TEST_METHOD(IsAuxiliaryAttribute)
		{
			AttributedFoo attributedFoo;

			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("this"));

			// Prescribed
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalInteger"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalFloat"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalString"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalVector"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalMatrix"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalScope"));

			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalStringArray"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("ExternalScopeArray"));

			// Check non-existent
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("NonExistent"));
			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("AuxInt"));

			// Add aux
			attributedFoo.AppendAuxilaryAttribute("AuxInt");
			Assert::IsTrue(attributedFoo.IsAuxilaryAttribute("AuxInt"));
		}

		TEST_METHOD(AppendAuxiliaryAttribute)
		{
			AttributedFoo attributedFoo;

			Assert::IsFalse(attributedFoo.IsAuxilaryAttribute("AuxInt"));

			// Add aux
			Datum& auxDatum = attributedFoo.AppendAuxilaryAttribute("AuxInt");
			Assert::IsTrue(attributedFoo.IsAuxilaryAttribute("AuxInt"));

			Assert::AreSame(auxDatum, attributedFoo["AuxInt"]);

			// Try to add it again
			Datum& auxDatum2 = attributedFoo.AppendAuxilaryAttribute("AuxInt");

			Assert::AreSame(auxDatum, auxDatum2);

			// Try to add aux with same name as prescribed
			Assert::ExpectException<runtime_error>([&attributedFoo]() { attributedFoo.AppendAuxilaryAttribute("ExternalInteger"); });

			// Try to add aux with empty name
			Assert::ExpectException<runtime_error>([&attributedFoo]() { attributedFoo.AppendAuxilaryAttribute(""); });
		}

		TEST_METHOD(ForEachAttribute)
		{
			// Non-const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes;
				for (Signature signature : AttributedFoo::AllSignatures())
				{
					attributedFooExpectedAttributes.insert(signature.Name);
				}
				attributedFooExpectedAttributes.insert("AuxInt");
				attributedFooExpectedAttributes.insert("this");

				// Get attributes from foreach
				set<std::string> attributes;
				attributedFoo.ForEachAttribute(
					[&attributes, &attributedFoo](std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes;
				for (Signature signature : AttributedFoo::AllSignatures())
				{
					attributedFooExpectedAttributes.insert(signature.Name);
				}
				attributedFooExpectedAttributes.insert("AuxInt");
				attributedFooExpectedAttributes.insert("this");

				// Get attributes from foreach
				set<std::string> attributes;
				const_cast<const AttributedFoo&>(attributedFoo).ForEachAttribute(
					[&attributes, &attributedFoo](const std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Test Early out
			{
				std::size_t counter = 0;
				AttributedFoo attributedFoo;

				attributedFoo.ForEachAttribute(
					[&counter](std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);

				counter = 0;

				const_cast<const AttributedFoo&>(attributedFoo).ForEachAttribute(
					[&counter](const std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);
			}
		}

		TEST_METHOD(ForEachPrescribedAttribute)
		{
			// Non-const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes;
				for (Signature signature : AttributedFoo::AllSignatures())
				{
					attributedFooExpectedAttributes.insert(signature.Name);
				}

				// Get attributes from foreach
				set<std::string> attributes;
				attributedFoo.ForEachPrescribedAttribute(
					[&attributes, &attributedFoo](std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsPrescribedAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes;
				for (Signature signature : AttributedFoo::AllSignatures())
				{
					attributedFooExpectedAttributes.insert(signature.Name);
				}

				// Get attributes from foreach
				set<std::string> attributes;
				const_cast<const AttributedFoo&>(attributedFoo).ForEachPrescribedAttribute(
					[&attributes, &attributedFoo](const std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsPrescribedAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Test Early out
			{
				std::size_t counter = 0;
				AttributedFoo attributedFoo;

				attributedFoo.ForEachPrescribedAttribute(
					[&counter](std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);

				counter = 0;

				const_cast<const AttributedFoo&>(attributedFoo).ForEachPrescribedAttribute(
					[&counter](const std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);
			}
		}

		TEST_METHOD(ForEachAuxiliaryAttribute)
		{
			// Non-const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes = {"AuxInt"};

				// Get attributes from foreach
				set<std::string> attributes;
				attributedFoo.ForEachAuxiliaryAttribute(
					[&attributes, &attributedFoo](std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsAuxilaryAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Const
			{
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("AuxInt");

				// Build expected attribute list
				set<std::string> attributedFooExpectedAttributes = {"AuxInt"};

				// Get attributes from foreach
				set<std::string> attributes;
				const_cast<const AttributedFoo&>(attributedFoo).ForEachAuxiliaryAttribute(
					[&attributes, &attributedFoo](const std::pair<const string, Datum>* attributeKeyPair)
					{
						attributes.insert(attributeKeyPair->first);
						Assert::IsTrue(attributedFoo.IsAuxilaryAttribute(attributeKeyPair->first));
						return false;
					});

				// Compare sets
				Assert::AreEqual(attributedFooExpectedAttributes.size(), attributes.size());
				Assert::IsTrue(equal(attributes.begin(), attributes.end(), attributedFooExpectedAttributes.begin()));
			}

			// Test Early out
			{
				std::size_t counter = 0;
				AttributedFoo attributedFoo;
				attributedFoo.AppendAuxilaryAttribute("Aux1");
				attributedFoo.AppendAuxilaryAttribute("Aux2");
				attributedFoo.AppendAuxilaryAttribute("Aux3");

				attributedFoo.ForEachAuxiliaryAttribute(
					[&counter](std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);

				counter = 0;

				const_cast<const AttributedFoo&>(attributedFoo).ForEachAuxiliaryAttribute(
					[&counter](const std::pair<const string, Datum>*)
					{
						++counter;
						return true;
					});

				Assert::AreEqual(1_z, counter);
			}
		}

		TEST_METHOD(Equality)
		{
			AttributedFoo attributedFoo;
			AttributedFoo attributedFoo2;

			Assert::IsTrue(attributedFoo.Equals(&attributedFoo2));
			Assert::IsTrue(attributedFoo == attributedFoo2);
			Assert::IsFalse(attributedFoo != attributedFoo2);
			
			// Different sizes
			attributedFoo.AppendAuxilaryAttribute("Aux1");
			Assert::IsFalse(attributedFoo.Equals(&attributedFoo2));
			Assert::IsFalse(attributedFoo == attributedFoo2);
			Assert::IsTrue(attributedFoo != attributedFoo2);

			// Same members, different values
			// Different sizes
			attributedFoo2.AppendAuxilaryAttribute("Aux1");
			attributedFoo2["Aux1"] = 1;
			Assert::IsFalse(attributedFoo.Equals(&attributedFoo2));
			Assert::IsFalse(attributedFoo == attributedFoo2);
			Assert::IsTrue(attributedFoo != attributedFoo2);
			
			// Make equal again
			attributedFoo["Aux1"] = 1;
			Assert::IsTrue(attributedFoo.Equals(&attributedFoo2));
			Assert::IsTrue(attributedFoo == attributedFoo2);
			Assert::IsFalse(attributedFoo != attributedFoo2);

			// Same size but key name different
			attributedFoo["Aux2"] = 1;
			attributedFoo2["Aux3"] = 1;
			Assert::IsFalse(attributedFoo.Equals(&attributedFoo2));
			Assert::IsFalse(attributedFoo == attributedFoo2);
			Assert::IsTrue(attributedFoo != attributedFoo2);

			// Equals with null
			Assert::IsFalse(attributedFoo.Equals(nullptr));

			// Equals with bar
			AttributedBar attributedBar;
			Assert::IsFalse(attributedFoo.Equals(&attributedBar));
		}

		TEST_METHOD(Clone)
		{
			AttributedFoo attributedFoo;
			std::unique_ptr<Scope> attributedFooClone = attributedFoo.Clone();
			Assert::AreEqual(attributedFoo.TypeIdInstance(), attributedFooClone->TypeIdInstance());
			Assert::IsTrue(attributedFoo.Equals(attributedFooClone.get()));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}