#pragma once

#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "Scope.h"
#include "Transform.h"
#include "JsonParseCoordinator.h"
#include "Event.h"
#include <memory>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region String
	template<> inline std::wstring ToString<std::string>(std::string* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<> inline std::wstring ToString<std::wstring>(std::wstring* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region Foo
	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region Vec4
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str()); 
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4* const t)
	{
		RETURN_WIDE_STRING(t); 
	}

	template<>
	inline std::wstring ToString<glm::vec4>(glm::vec4* const t)
	{
		RETURN_WIDE_STRING(t); 
	}
#pragma endregion

#pragma region Mat4
	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str()); 
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4* const t)
	{
		RETURN_WIDE_STRING(t); 
	}

	template<>
	inline std::wstring ToString<glm::mat4>(glm::mat4* const t)
	{
		RETURN_WIDE_STRING(t); 
	}
#pragma endregion

#pragma region RTTI
	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(const FieaGameEngine::RTTI* const t)
	{
		RETURN_WIDE_STRING(t); 
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(FieaGameEngine::RTTI* const t)
	{
		RETURN_WIDE_STRING(t); 
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI*>(FieaGameEngine::RTTI** const t)
	{
		RETURN_WIDE_STRING(t); 
	}
#pragma endregion

#pragma region Datum
	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum& /*t*/)
	{
		using namespace std::string_literals;
		return L"Datum"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(FieaGameEngine::Datum* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region Datum
	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(const FieaGameEngine::DatumTypes& t)
	{
		RETURN_WIDE_STRING(static_cast<std::uint32_t>(t));
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(const FieaGameEngine::DatumTypes* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::DatumTypes>(FieaGameEngine::DatumTypes* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region RTTI
	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope&)
	{
		using namespace std::string_literals;
		return L"Scope"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region ParseCoordinator
	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator>(const FieaGameEngine::JsonParseCoordinator&)
	{
		using namespace std::string_literals;
		return L"ParseCoordinator"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator>(const FieaGameEngine::JsonParseCoordinator* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator>(FieaGameEngine::JsonParseCoordinator* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region IJsonParseHelper
	template<>
	inline std::wstring ToString<FieaGameEngine::IJsonParseHelper>(const FieaGameEngine::IJsonParseHelper&)
	{
		using namespace std::string_literals;
		return L"ParseHelper"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::IJsonParseHelper>(const FieaGameEngine::IJsonParseHelper* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::IJsonParseHelper>(FieaGameEngine::IJsonParseHelper* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region ParseCoordinatorWrapper
	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator::Wrapper>(const FieaGameEngine::JsonParseCoordinator::Wrapper&)
	{
		using namespace std::string_literals;
		return L"ParseCoordinator Wrapper"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator::Wrapper>(const FieaGameEngine::JsonParseCoordinator::Wrapper* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator::Wrapper>(FieaGameEngine::JsonParseCoordinator::Wrapper* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region Factory
	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::RTTI>>(const FieaGameEngine::Factory<FieaGameEngine::RTTI>& /*t*/)
	{
		using namespace std::string_literals;
		return L"Factory<RTTI>"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::RTTI>>(const FieaGameEngine::Factory<FieaGameEngine::RTTI>* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::RTTI>>(FieaGameEngine::Factory<FieaGameEngine::RTTI>* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::Scope>>(const FieaGameEngine::Factory<FieaGameEngine::Scope>& /*t*/)
	{
		using namespace std::string_literals;
		return L"Factory<Scope>"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::Scope>>(const FieaGameEngine::Factory<FieaGameEngine::Scope>* const t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::Scope>>(FieaGameEngine::Factory<FieaGameEngine::Scope>* const t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion

#pragma region Transform
	template<>
	inline std::wstring ToString<FieaGameEngine::Transform>(const FieaGameEngine::Transform& /*t*/)
	{
		using namespace std::string_literals;
		return L"Transform"s;
	}
#pragma endregion

#pragma region SharedPtrScope
	template<>
	inline std::wstring ToString<std::shared_ptr<FieaGameEngine::Scope>>(const std::shared_ptr<FieaGameEngine::Scope>& /*t*/)
	{
		using namespace std::string_literals;
		return L"Scope"s;
	}
#pragma endregion

#pragma region Event
	template<>
	inline std::wstring ToString<FieaGameEngine::EventBase>(const FieaGameEngine::EventBase& /*t*/)
	{
		using namespace std::string_literals;
		return L"Event"s;
	}
#pragma endregion
}
