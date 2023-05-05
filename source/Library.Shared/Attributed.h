#pragma once

#include <concepts>
#include <vector>
#include <string>
#include "Scope.h"
#include "Signature.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Table that allows for both prescribed and auxilary attributes
	/// </summary>
	class Attributed : public Scope
	{
	RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/// <summary>
		/// Checks whether the table contains a member by a given key
		/// </summary>
		/// <param name="attributeName">Key to check</param>
		/// <returns>True if the key is a attribute; otherwise false</returns>
		bool IsAttribute(const std::string& attributeName) const;

		/// <summary>
		/// Checks whether the given key is a member that persists across all
		/// objects of this Attributed class.
		/// The 'this' key is not considered a prescribed attribute 
		/// </summary>
		/// <param name="attributeName">Key to check</param>
		/// <returns>True if the key is a prescribed attribute; otherwise false</returns>
		bool IsPrescribedAttribute(const std::string& attributeName) const;

		/// <summary>
		/// Checks whether the given key is a member of the table but not
		/// a prescribed attribute.
		/// The 'this' key is not considered an auxilary attribute 
		/// </summary>
		/// <param name="attributeName">Key to check</param>
		/// <returns>True if the key is an auxilary attribute; otherwise false</returns>
		bool IsAuxilaryAttribute(const std::string& attributeName) const;

		/// <summary>
		/// Adds an auxilary member to the table. If the key already exists as a prescribed member an exception is thrown.
		/// </summary>
		/// <param name="attributeName">Auxilary key t oadd</param>
		/// <exception cref="runtime_error">A prescribed attribute already exists with this name</exception>
		/// <returns>Datum of the newly created auxilary member or a reference to a prexisting datum if the key already exists</returns>
		Datum& AppendAuxilaryAttribute(const std::string& attributeName);

		using AttributeCallback = std::function<bool(std::pair<const std::string, Datum>*)>;
		using ConstAttributeCallback = std::function<bool(const std::pair<const std::string, Datum>*)>;
		
		/// <summary>
		/// Iterates through all attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each attribute</param>
		void ForEachAttribute(AttributeCallback callback);
		
		/// <summary>
		/// Iterates through all attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each attribute</param>
		void ForEachAttribute(ConstAttributeCallback callback) const;

		/// <summary>
		/// Iterates through all prescribed attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each prescribed attribute</param>
		void ForEachPrescribedAttribute(AttributeCallback callback);
		
		/// <summary>
		/// Iterates through all prescribed attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each prescribed attribute</param>
		void ForEachPrescribedAttribute(ConstAttributeCallback callback) const;

		/// <summary>
		/// Iterates through all auxiliary attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each auxiliary attribute</param>
		void ForEachAuxiliaryAttribute(AttributeCallback callback);
		
		/// <summary>
		/// Iterates through all auxiliary prescribed attributes of this table and calls a callback function provided for each.
		/// If the callback function returns true; the loop will break out.
		/// </summary>
		/// <param name="callback">Callback function to be called for each auxiliary attribute</param>
		void ForEachAuxiliaryAttribute(ConstAttributeCallback callback) const;

		/// <summary>
		/// Compares this attributed with another attributed 
		/// </summary>
		/// <param name="rhs">Attributed to compare against</param>
		/// <returns>True if the two attributed objects are equal; otherwise false</returns>
		[[nodiscard]] bool operator==(const Attributed& rhs) const;

		/// <summary>
		/// Compares this attributed with another attributed
		/// </summary>
		/// <param name="rhs">Attributed to compare against</param>
		/// <returns>True if the two attributed objects are not equal; otherwise false</returns>
		[[nodiscard]] bool operator!=(const Attributed& rhs) const;

		/// <summary>
		/// Checks if this obejct is equal to a given RTTI object
		/// </summary>
		/// <param name="rhs">RTTI object to compare against</param>
		/// <returns>True if equal; otherwise false</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// A list of signatures that describes each prescribed attribute.
		/// This method returns signatures for both this class and signatures dervied from this class's parents
		/// Derived classes MUST implement this method using the ATTRIBUTED_DECLARATIONS macro in the derived class's declaration
		/// </summary>
		/// <returns>Prescibed signatures for this class and parent classes</returns>
		[[nodiscard]] static Vector<Signature> AllSignatures()
		{
			return Signatures();
		}

		/// <summary>
		/// A list of signatures that describes each prescribed attribute for this class only.
		/// Derived classes MUST implement this method in their class declaration.
		/// </summary>
		/// <returns>Prescibed signatures for this class only</returns>
		[[nodiscard]] static Vector<Signature> Signatures()
		{
			return {};
		}
		
		/// <summary>
		/// Populates the scope with prescribed attributes
		/// </summary>
		/// <param name="signatures"></param>
		void Populate(const Vector<Signature>& signatures);

	protected:

		/// <summary>
		/// Constructor.
		/// When inheriting from this class, dervived constructors must call this method
		/// and pass in 'this' into the derivedThis parameter. This parameter allows this class
		/// to called derived class static methods inside the constructor.
		/// </summary>
		/// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
		explicit Attributed(T* derivedThis);

		/// <summary>
		/// Copy constructor.
		/// When inheriting from this class, dervived copy constructors must call this method
		/// and pass in 'this' into the derivedThis parameter. This parameter allows this class
		/// to called derived class static methods inside the constructor.
		/// </summary>
		/// <param name="rhs">Right hand side to copy</param>
		/// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
		Attributed(const Attributed& rhs, T* derivedThis);

		/// <summary>
		/// Move constructor.
		/// When inheriting from this class, dervived copy constructors must call this method
		/// and pass in 'this' into the derivedThis parameter. This parameter allows this class
		/// to called derived class static methods inside the constructor.
		/// </summary>
		/// <param name="rhs">Right hand side to move</param>
		/// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
		Attributed(Attributed&& rhs, T* derivedThis) noexcept;

		/// <summary>
		/// Copy assignment
		/// </summary>
		/// <param name="rhs">Right hand side to copy</param>
		/// <returns>Reference to this</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Move assignment
		/// </summary>
		/// <param name="rhs">Right hand side to move</param>
		/// <returns>Reference to this</returns>
		Attributed& operator=(Attributed&& rhs) noexcept;

		/// <summary>
		/// Virtual version of the static AllSignatures method
		/// </summary>
		/// <returns>Signatures for this class plus parent class signatures</returns>
		[[nodiscard]] virtual Vector<Signature> _AllSignatures() const = 0;

		/// <summary>
		/// Virtual version of the static Signatures method
		/// </summary>
		/// <returns>Signatures for this class only</returns>
		[[nodiscard]] virtual Vector<Signature> _Signatures() const = 0;
	};
}
																										   
#define ATTRIBUTED_DECLARATIONS(Type, ParentType)	   													    \
	RTTI_DECLARATIONS(Type, ParentType);											                        \
    public:																						            \
		[[nodiscard]] static FieaGameEngine::Vector<FieaGameEngine::Signature> AllSignatures();             \
	protected:														        					            \
		[[nodiscard]] virtual FieaGameEngine::Vector<FieaGameEngine::Signature> _AllSignatures() const;		\
		[[nodiscard]] virtual FieaGameEngine::Vector<FieaGameEngine::Signature> _Signatures() const;

#define ATTRIBUTED_DEFINITIONS(Type, ParentType)                                                            \
	RTTI_DEFINITIONS(Type);																			        \
	FieaGameEngine::Vector<FieaGameEngine::Signature> Type::AllSignatures()                                 \
	{																										\
		return ParentType::AllSignatures().Append(Signatures());                                            \
	}																										\
														        											\
	FieaGameEngine::Vector<FieaGameEngine::Signature> Type::_AllSignatures() const		                    \
	{																										\
		return AllSignatures();																				\
	}																										\
																											\
	FieaGameEngine::Vector<FieaGameEngine::Signature> Type::_Signatures() const			                    \
	{																										\
		return Signatures();																				\
	}
																																																																											
#include "Attributed.inl"																					