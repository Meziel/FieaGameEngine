#pragma once

#include <string>
#include <stdexcept>
#include <functional>
#include <gsl/gsl>
#include <memory>
#include "RTTI.h"
#include "Vector.h"
#include "Datum.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Table for building records
	/// </summary>
	class Scope : public RTTI
	{
	RTTI_DECLARATIONS(Scope, RTTI);

	public:

		/// <summary>
		/// Constructor
		/// </summary>
		Scope();

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="capacity">Initial capacity of the scope</param>
		explicit Scope(std::size_t capacity);
		
		/// <summary>
		/// Copy constructor
		/// The parent of the newly constructor object will be null
		/// </summary>
		/// <param name="rhs">Scope to copy</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Move constructor.
		/// If the rhs has a parent, the rhs will be deleted, otherwise
		/// it will be in an unusable state.
		/// The parent of the newly constructor object will be null
		/// </summary>
		/// <param name="rhs">Scope to move</param>
		/// <returns></returns>
		Scope(Scope&& rhs) noexcept;

		virtual ~Scope();

		/// <summary>
		/// Copy assignment. Copies all elements in the scope including
		/// nested scopes to this object.
		/// This does not change the parent of the left hand side.
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move assignment. Moves the elements of the right hand scope
		/// to this object. If the rhs has a parent, the rhs will be deleted, otherwise
		/// it will be in an unusable state.
		/// This does not change the parent of the left hand side.
		/// </summary>
		/// <param name="rhs">Scope to move</param>
		/// <returns></returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Swaps two scopes
		/// </summary>
		/// <param name="left">First scope</param>
		/// <param name="right">Second scope</param>
		friend void swap(Scope& left, Scope& right) noexcept;

		/// <summary>
		/// Gets the address of the Datum associated with the given name in this scope.
		/// If the name does not exist, null is returned.
		/// </summary>
		/// <param name="key">Name of the datum to retrieve</param>
		/// <returns>Datum address associated with the key</returns>
		Datum* Find(const std::string key);

		/// <summary>
		/// Gets the address of the Datum associated with the given name in this scope.
		/// If the name does not exist, null is returned.
		/// </summary>
		/// <param name="key">Name of the datum to retrieve</param>
		/// <returns>Datum address associated with the key</returns>
		const Datum* Find(const std::string key) const;

		/// <summary>
		/// Gets the address of the closest nested Datum associated with
		/// the given name in this scope or its ancestors. This method goes up
		/// the nested Scope heiarchy to find a datum with the matching key.
		/// If there is no datum found with this key, null is returned.
		/// </summary>
		/// <param name="key">Datum key to search</param>
		/// <param name="ptr">Output variable that will be written to.
		/// Will contain the address of the scope object which contains the match</param>
		/// <returns>Address of the datum found or nullptr</returns>
		Datum* Search(const std::string& key, Scope*& ptr);

		/// <summary>
		/// Gets the address of the closest nested Datum associated with
		/// the given name in this scope or its ancestors. This method goes up
		/// the nested Scope heiarchy to find a datum with the matching key.
		/// If there is no datum found with this key, null is returned.
		/// </summary>
		/// <param name="key">Datum key to search</param>
		/// <param name="ptr">Output variable that will be written to.
		/// Will contain the address of the scope object which contains the match</param>
		/// <returns>Address of the datum found or nullptr</returns>
		const Datum* Search(const std::string& key, const Scope*& ptr) const;

		/// <summary>
		/// Gets the address of the closest nested Datum associated with
		/// the given name in this scope or its ancestors. This method goes up
		/// the nested Scope heiarchy to find a datum with the matching key.
		/// If there is no datum found with this key, null is returned.
		/// </summary>
		/// <param name="key">Datum key to search</param>
		/// <returns>Address of the datum found or nullptr</returns>
		Datum* Search(const std::string& key);

		/// <summary>
		/// Gets the address of the closest nested Datum associated with
		/// the given name in this scope or its ancestors. This method goes up
		/// the nested Scope heiarchy to find a datum with the matching key.
		/// If there is no datum found with this key, null is returned.
		/// </summary>
		/// <param name="key">Datum key to search</param>
		/// <returns>Address of the datum found or nullptr</returns>
		const Datum* Search(const std::string& key) const;

		Datum& At(const std::string& key);
		const Datum& At(const std::string& key) const;

		/// <summary>
		/// Adds a datum to the scope with the given name.
		/// If the name already exist, then the prexisting datum will be returned.
		/// </summary>
		/// <param name="datumName">Datum name to append</param>
		/// <returns>Datum at the key</returns>
		Datum& Append(const std::string& datumName);

		/// <summary>
		/// Adds a datum to the scope with the given name and adds a newly created scope to the datum.
		/// If the name already exist, then the newly created scope will be appended to the prexisting datum.
		/// </summary>
		/// <param name="datumName">Datum name to append</param>
		/// <param name="className">Scope derived class to create</param>
		/// <returns>The newly created scope</returns>
		Scope& AppendScope(const std::string& datumName, const std::string& className = "Scope");

		/// <summary>
		/// Adds a scope this scope. If the provided scope already has a parent,
		/// it is orphaned and reparented to this scope.
		/// </summary>
		/// <param name="scope">Scope to nest into this scope</param>
		/// <param name="datumName">Datum name to put the scope</param>
		void Adopt(std::shared_ptr<Scope> scope, std::string datumName);

		/// <summary>
		/// Adds a scope this scope. If the provided scope already has a parent,
		/// it is orphaned and reparented to this scope.
		/// </summary>
		/// <param name="scope">Scope to nest into this scope</param>
		/// <param name="datumName">Datum name to put the scope</param>
		void Adopt(Scope* scope, std::string datumName);

		/// <summary>
		/// Checks if the the provided scope is an ancestor of this scope
		/// </summary>
		/// <param name="scope">Scope to check if is ancestor</param>
		/// <returns>True if scope is ancestor; otherwise false</returns>
		[[nodiscard]] bool IsAncestor(const Scope& scope) const;

		/// <summary>
		/// Check if the provided scope is a descendent of this scope
		/// </summary>
		/// <param name="scope">Scope to check if is descendent</param>
		/// <returns>True if scope is descendent; otherwise false</returns>
		[[nodiscard]] bool IsDescendent(const Scope& scope) const;

		/// <summary>
		/// Gets the parent of the this scope.
		/// If this scope does not have a parent, null is returned.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] Scope* GetParent() const;

		/// <summary>
		/// Wraps the append method
		/// </summary>
		/// <param name="key">Key to append</param>
		/// <returns>Datum at the associated key</returns>
		Datum& operator[](const std::string& key);

		/// <summary>
		/// Gets a reference to the datum at the index the datum was appended
		/// </summary>
		/// <param name="index">Index to get</param>
		/// <returns>Datum at associated index</returns>
		[[nodiscard]] Datum& operator[](const std::size_t index);

		/// <summary>
		/// Compares two scopes.
		/// This method does not compare the parents of the scopes
		/// </summary>
		/// <param name="rhs">Scope to compare against</param>
		/// <returns>True if scopes are equal; otherwise false</returns>
		[[nodiscard]] bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Compares two scopes.
		/// This method does not compare the parents of the scopes
		/// </summary>
		/// <param name="rhs">Scope to compare against</param>
		/// <returns>True if scopes are not equal; otherwise false</returns>
		[[nodiscard]] bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Checks if this obejct is equal to a given RTTI object
		/// </summary>
		/// <param name="rhs">RTTI object to compare against</param>
		/// <returns>True if equal; otherwise false</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Attempts to find a given nested child scope within this object.
		/// If the scope is found, the datum associated with the scope and the index
		/// within the datum is returned; otherwise nullptr is returned and the index is set to 0.
		/// </summary>
		/// <param name="scope">Nested scope to find within this scope</param>
		/// <returns>Datum address and the index within the datum</returns>
		std::pair<Datum*, std::size_t> FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Clones polymorphically the given object
		/// </summary>
		/// <returns>Pointer to the newly created object cloned from this</returns>
		[[nodiscard]] virtual std::unique_ptr<Scope> Clone() const;

	protected:
		Vector<std::pair<const std::string, Datum>*> _orderedList;
		HashMap<const std::string, Datum> _hashMap;

	private:
		Scope* _parent = nullptr;

		void _Orphan(Scope& scope);
		void _FixNestedScopesParents();

		using NestedScopeFunction = std::function<bool(Scope&)>;
		std::pair<Datum*, std::size_t> _ForEachNestedScope(NestedScopeFunction func) const;
	};

	ConcreteFactory(Scope, Scope);

	/// <summary>
	/// Compares two scope pointers
	/// </summary>
	template <>
	struct DefaultEquality<Scope*> final
	{
		bool operator()(const Scope* lhs, const Scope* rhs) const;
	};

	/// <summary>
	/// Compares two scope pointers
	/// </summary>
	template <>
	struct DefaultEquality<const Scope*> final
	{
		bool operator()(const Scope* lhs, const Scope* rhs) const;
	};

	/// <summary>
	/// Compares two scope pointers
	/// </summary>
	template <>
	struct DefaultEquality<Scope* const> final
	{
		bool operator()(const Scope* lhs, const Scope* rhs) const;
	};

	/// <summary>
	/// Compares two scope pointers
	/// </summary>
	template <>
	struct DefaultEquality<const Scope* const> final
	{
		bool operator()(const Scope* lhs, const Scope* rhs) const;
	};

	/// <summary>
	/// Compares two scope pointers
	/// </summary>
	template <>
	struct DefaultEquality<std::shared_ptr<Scope>> final
	{
		bool operator()(const std::shared_ptr<Scope>& lhs, const std::shared_ptr<Scope>& rhs) const;
	};
}

#include "Scope.inl"