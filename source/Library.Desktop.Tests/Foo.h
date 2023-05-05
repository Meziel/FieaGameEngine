#pragma once

#include <cstdint>
#include <RTTI.h>
#include "Factory.h"

namespace UnitTests
{
	class Foo final : public FieaGameEngine::RTTI
	{
	RTTI_DECLARATIONS(Foo,  FieaGameEngine::RTTI);

	public:
		explicit Foo(std::int32_t data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		std::int32_t Data() const;
		void SetData(std::int32_t data);

		virtual bool Equals(const RTTI* rhs) const override;

	private:
		std::int32_t* _data;
	};

	ConcreteFactory(Foo, FieaGameEngine::RTTI);
}