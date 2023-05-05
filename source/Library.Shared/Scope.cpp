#include "pch.h"
#include "Scope.h"
#include "Factory.h"
#include <stdexcept>

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope()
	{
	}

	Scope::Scope(std::size_t capacity) :
		_hashMap(capacity),
		_orderedList(capacity)
	{

	}

	Scope::Scope(const Scope& rhs) :
		_orderedList(rhs._orderedList.Size()),
		_hashMap(rhs._hashMap.BucketCount())
	{
		for (auto keyPair : rhs._orderedList)
		{
			Datum& datumToCopy = keyPair->second;
			Datum& newDatum = Append(keyPair->first);
			if (datumToCopy.Type() == DatumTypes::Table)
			{
				for (std::size_t i = 0; i < datumToCopy.Size(); ++i)
				{
					unique_ptr<Scope> newScope = datumToCopy.Get<shared_ptr<Scope>>(i)->Clone();
					newScope->_parent = this;
					newDatum.PushBack<shared_ptr<Scope>>(std::move(newScope));
				}
			}
			else
			{
				newDatum = datumToCopy;
			}
		}
	}

	Scope::Scope(Scope&& rhs) noexcept :
		_orderedList(std::move(rhs._orderedList)),
		_hashMap(std::move(rhs._hashMap))
	{
		_Orphan(rhs);
		_FixNestedScopesParents();
	}

	Scope::~Scope()
	{
	}

	Scope& Scope::operator=(const Scope& rhs)
	{	
		assert(!IsDescendent(rhs));
		assert(!IsAncestor(rhs));

		Scope temp(rhs);
		swap(*this, temp);

		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		assert(!IsDescendent(rhs));
		assert(!IsAncestor(rhs));

		swap(*this, rhs);

		return *this;
	}

	void swap(Scope& left, Scope& right) noexcept
	{
		using std::swap;
		swap(left._hashMap, right._hashMap);
		swap(left._orderedList, right._orderedList);

		left._FixNestedScopesParents();
		right._FixNestedScopesParents();
	}

	Datum* Scope::Find(const std::string key)
	{
		const auto& it = _hashMap.Find(key);
		if (it == _hashMap.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	const Datum* Scope::Find(const std::string key) const
	{
		const auto& it = _hashMap.Find(key);
		if (it == _hashMap.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	Datum* Scope::Search(const std::string& key, Scope*& ptr)
	{
		Datum* datum = Find(key);
		if (datum != nullptr)
		{
			ptr = this;
			return datum;
		}
		if (_parent != nullptr)
		{
			return _parent->Search(key, ptr);
		}
		ptr = nullptr;
		return nullptr;
	}

	const Datum* Scope::Search(const std::string& key, const Scope*& ptr) const
	{
		const Datum* datum = Find(key);
		if (datum != nullptr)
		{
			ptr = this;
			return datum;
		}
		if (_parent != nullptr)
		{
			return _parent->Search(key, ptr);
		}
		ptr = nullptr;
		return nullptr;
	}

	Datum* Scope::Search(const std::string& key)
	{
		Scope* scope;
		return Search(key, scope);
	}

	const Datum* Scope::Search(const std::string& key) const
	{
		const Scope* scope;
		return Search(key, scope);
	}

	Datum& Scope::At(const std::string& key)
	{
		return _hashMap.At(key);
	}

	const Datum& Scope::At(const std::string& key) const
	{
		return _hashMap.At(key);
	}

	Datum& Scope::Append(const std::string& datumName)
	{
		if (datumName == "")
		{
			throw runtime_error("Key cannot be empty");
		}

		auto [it, wasInserted] = _hashMap.Emplace(std::piecewise_construct, std::forward_as_tuple(datumName), std::forward_as_tuple());
		// Add to ordered list if this is a new datum
		if (wasInserted)
		{
			_orderedList.PushBack(&*it);
		}
		return it->second;
	}

	Scope& Scope::AppendScope(const std::string& datumName, const std::string& className)
	{
		if (datumName == "")
		{
			throw runtime_error("Key cannot be empty");
		}

		auto [it, wasInserted] = _hashMap.Emplace(std::piecewise_construct, std::forward_as_tuple(datumName),  std::forward_as_tuple());
		// Add to ordered list if this is a new datum
		if (wasInserted)
		{
			// Datum just created
			it->second.SetType(DatumTypes::Table);
			_orderedList.PushBack(&*it);
		}
		else
		{
			// Datum already exists
			if (it->second.Type() != DatumTypes::Table)
			{
				throw std::runtime_error("Datum is not of type Table");
			}
		}
		// Create scope
		shared_ptr<Scope> newScope(Factory<Scope>::Create(className));
		if (newScope == nullptr)
		{
			throw runtime_error(std::format("Factory of {} does not exist", className));
		}

		newScope->_parent = this;
		// Add scope to datum
		it->second.PushBack<shared_ptr<Scope>>(newScope);
		return *newScope;
	}

	void Scope::Adopt(std::shared_ptr<Scope> scope, std::string datumName)
	{
		if (datumName == "")
		{
			throw runtime_error("Key cannot be empty");
		}

		if (scope.get() == this)
		{
			throw runtime_error("Scope cannot adopt itself");
		}

		if (IsAncestor(*scope))
		{
			throw runtime_error("Scope cannot adopt an ancestor of itself");
		}

		auto [it, wasInserted] = _hashMap.Emplace(std::piecewise_construct, std::forward_as_tuple(datumName), std::forward_as_tuple());
		// Add to ordered list if this is a new datum
		if (wasInserted)
		{
			it->second.SetType(DatumTypes::Table);
			_orderedList.PushBack(&*it);
		}
		else
		{
			if (it->second.Type() != DatumTypes::Table)
			{
				throw runtime_error("Datum is not of type Table");
			}
		}
		_Orphan(*scope);
		scope->_parent = this;

		it->second.PushBack(std::move(scope));
	}

	void Scope::Adopt(Scope* scope, std::string datumName)
	{
		if (datumName == "")
		{
			throw runtime_error("Key cannot be empty");
		}

		if (scope == this)
		{
			throw runtime_error("Scope cannot adopt itself");
		}

		if (IsAncestor(*scope))
		{
			throw runtime_error("Scope cannot adopt an ancestor of itself");
		}

		auto [it, wasInserted] = _hashMap.Emplace(std::piecewise_construct, std::forward_as_tuple(datumName), std::forward_as_tuple());
		// Add to ordered list if this is a new datum
		if (wasInserted)
		{
			it->second.SetType(DatumTypes::TablePointer);
			_orderedList.PushBack(&*it);
		}
		else
		{
			if (it->second.Type() != DatumTypes::TablePointer)
			{
				throw runtime_error("Datum is not of type Table");
			}
		}
		scope->_parent = this;

		it->second.PushBack(scope);
	}

	bool Scope::IsAncestor(const Scope& scope) const
	{
		if (_parent == nullptr)
		{
			return false;
		}
		if (_parent == &scope)
		{
			return true;
		}
		return _parent->IsAncestor(scope);
	}

	bool Scope::IsDescendent(const Scope& scope) const
	{
		return scope.IsAncestor(*this);
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	Datum& Scope::operator[](const std::size_t index)
	{
		return _orderedList[index]->second;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (_orderedList.Size() != rhs._orderedList.Size())
		{
			return false;
		}

		for (std::size_t i = 0; i < _orderedList.Size(); ++i)
		{
			if (*_orderedList[i] != *rhs._orderedList[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}
		return rhs->Is(_typeId) ? *this == *rhs->As<Scope>() : false;
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(const Scope& scope) const
	{
		auto [datum, index] = _ForEachNestedScope(
			[&scope](const Scope& nestedScope)
			{
				return &nestedScope == &scope;
			});
		return { datum, index };
	}

	std::unique_ptr<Scope> Scope::Clone() const
	{
		return std::make_unique<Scope>(*this);
	}

	void Scope::_Orphan(Scope& scope)
	{
		if (scope._parent != nullptr)
		{
			auto [datum, index] = scope._parent->FindContainedScope(scope);
			assert(datum != nullptr);
			if (datum != nullptr)
			{
				datum->Get<shared_ptr<Scope>>(index)->_parent = nullptr;
				datum->RemoveAt(index);
			}
		}
	}

	void Scope::_FixNestedScopesParents()
	{
		_ForEachNestedScope(
			[this](Scope& nestedScope)
			{
				nestedScope._parent = this;
				return false;
			});
	}

	std::pair<Datum*, size_t> Scope::_ForEachNestedScope(NestedScopeFunction func) const
	{
		for (auto keyPair : _orderedList)
		{
			Datum& datum = keyPair->second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t i = 0; i < datum.Size(); ++i)
				{
					Scope& scope = *datum.Get<shared_ptr<Scope>>(i);
					if (func(scope))
					{
						return std::pair(&datum, i);
					}
				}
			}
		}
		return std::pair(nullptr, 0);
	}
}