#include "pch.h"

#include "Attributed.h"
#include <stdexcept>

using namespace std;

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Attributed);

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);
		Populate(_AllSignatures());
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));
		Populate(_AllSignatures());
		return *this;
	}

	bool Attributed::IsAttribute(const string& attributeName) const
	{
		return Find(attributeName) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const string& attributeName) const
	{
		for (Signature& signature : _AllSignatures())
		{
			if (signature.Name == attributeName)
			{
				return true;
			}
		}

		return false;
	}

	bool Attributed::IsAuxilaryAttribute(const string& attributeName) const
	{
		return IsAttribute(attributeName) && !IsPrescribedAttribute(attributeName) && attributeName != "this";
	}

	Datum& Attributed::AppendAuxilaryAttribute(const string& attributedName)
	{
		if (IsPrescribedAttribute(attributedName))
		{
			throw runtime_error("A prescribed attribute already exists with this name");
		}
		return Append(attributedName);
	}

	void Attributed::ForEachAttribute(AttributeCallback callback)
	{
		for (auto stringDatumPointer : _orderedList)
		{
			// Early out if callback returns true
			if (callback(stringDatumPointer))
			{
				return;
			}
		}
	}

	void Attributed::ForEachAttribute(ConstAttributeCallback callback) const
	{
		for (auto stringDatumPointer : _orderedList)
		{
			// Early out if callback returns true
			if (callback(stringDatumPointer))
			{
				return;
			}
		}
	}

	void Attributed::ForEachPrescribedAttribute(AttributeCallback callback)
	{
		size_t prescribedAttributeEnd = _AllSignatures().size() + 1;
		for (size_t i = 1; i < prescribedAttributeEnd; ++i)
		{
			// Early out if callback returns true
			if (callback(_orderedList[i]))
			{
				return;
			}
		}
	}

	void Attributed::ForEachPrescribedAttribute(ConstAttributeCallback callback) const
	{
		size_t prescribedAttributeEnd = _AllSignatures().size() + 1;
		for (size_t i = 1; i < prescribedAttributeEnd; ++i)
		{
			// Early out if callback returns true
			if (callback(_orderedList[i]))
			{
				return;
			}
		}
	}

	void Attributed::ForEachAuxiliaryAttribute(AttributeCallback callback)
	{
		size_t prescribedAttributeEnd = _AllSignatures().size() + 1;
		for (size_t i = prescribedAttributeEnd; i < _orderedList.Size(); ++i)
		{
			// Early out if callback returns true
			if (callback(_orderedList[i]))
			{
				return;
			}
		}
	}

	void Attributed::ForEachAuxiliaryAttribute(ConstAttributeCallback callback) const
	{
		size_t prescribedAttributeEnd = _AllSignatures().size() + 1;
		for (size_t i = prescribedAttributeEnd; i < _orderedList.Size(); ++i)
		{
			// Early out if callback returns true
			if (callback(_orderedList[i]))
			{
				return;
			}
		}
	}

	bool Attributed::operator==(const Attributed& rhs) const
	{
		if (_orderedList.Size() != rhs._orderedList.Size())
		{
			return false;
		}

		// Skip this pointer
		for (std::size_t i = 1; i < _orderedList.Size(); ++i)
		{
			if (*_orderedList[i] != *rhs._orderedList[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Attributed::operator!=(const Attributed& rhs) const
	{
		return !operator==(rhs);
	}

	bool Attributed::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}
		return rhs->Is(_typeId) ? *this == *rhs->As<Attributed>() : false;
	}

	void Attributed::Populate(const Vector<Signature>& signatures)
	{
		// Add 'this' to scope
		Datum& thisDatum = Append("this");
		thisDatum = static_cast<RTTI*>(this);
		// Add each prescribed signature
		for (const Signature& signature : signatures)
		{
			if (signature.IsExternal)
			{
				Signature::ExternalInfo externalInfo = std::get<Signature::ExternalInfo>(signature.SigInfo);
				if (signature.Type == DatumTypes::TablePointer)
				{
					Datum& datum = Append(signature.Name);
					datum.Clear();
					datum.SetType(signature.Type);
					datum.Reserve(externalInfo.Size);
					for (size_t i = 0; i < externalInfo.Size; ++i)
					{
						// TODO: pointer arthimetic will be wrong if derived from scope and i > 0
						Scope* pointerToScope = reinterpret_cast<Scope*>(reinterpret_cast<std::byte*>(this) + externalInfo.Offset) + i;		
						Adopt(pointerToScope, signature.Name);
					}
				}
				else
				{
					Datum& datum = Append(signature.Name);
					datum.SetStorage(
						signature.Type,
						reinterpret_cast<std::byte*>(this) + externalInfo.Offset,
						externalInfo.Size);
				}
			}
			else
			{
				Datum& datum = Append(signature.Name);
				if (signature.Type != DatumTypes::Unknown)
				{
					datum.SetType(signature.Type);
				}
			}
		}
	}
}