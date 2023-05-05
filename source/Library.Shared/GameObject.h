#pragma once

#include <vector>
#include <cstdint>
#include "Attributed.h"
#include "Factory.h"
#include "Transform.h"
#include "GameTime.h"

namespace FieaGameEngine
{
    class Action;

	class GameObject : public Attributed
	{
		ATTRIBUTED_DECLARATIONS(GameObject, FieaGameEngine::Attributed);
    
    public:
        GameObject();
        GameObject(const GameObject& rhs);
        GameObject(GameObject&& rhs) noexcept;
        GameObject& operator=(const GameObject& rhs) = default;
        GameObject& operator=(GameObject&& rhs) noexcept = default;

        /// <summary>
        /// Name of the GameObject
        /// </summary>
        std::string Name;
        
        /// <summary>
        /// Position, Rotation, and Scale of the GameObject
        /// </summary>
        Transform Transform;

        /// <summary>
        /// Gets the children of the GameObject
        /// </summary>
        /// <returns>GameObject children</returns>
        Datum& Children();

        /// <summary>
        /// Gets the children of the GameObject
        /// </summary>
        /// <returns>GameObject children</returns>
        const Datum& Children() const;

        /// <summary>
        /// Gets the actions of the GameObject
        /// </summary>
        /// <returns>GameObject actions</returns>
        Datum& Actions();

        /// <summary>
        /// Gets the actions of the GameObject
        /// </summary>
        /// <returns>GameObject actions</returns>
        const Datum& Actions() const;

        /// <summary>
        /// Gets the reactions of the GameObject
        /// </summary>
        /// <returns>GameObject reactions</returns>
        Datum& Reactions();

        /// <summary>
        /// Gets the reactions of the GameObject
        /// </summary>
        /// <returns>GameObject reactions</returns>
        const Datum& Reactions() const;

        /// <summary>
        /// Creates an action from a string and adds it the Actions attribute.
        /// Action string requires a a factory of the action type
        /// </summary>
        /// <param name="actionName">Action class to instantiate</param>
        Action& CreateAction(std::string actionClass, std::string name = "");

        /// <summary>
        /// Polymorphicly creates a copy of the GameObject
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] std::unique_ptr<Scope> Clone() const override; 
        
        /// <summary>
        /// Lists of prescribed attributes for this GameObject
        /// </summary>
        /// <returns></returns>
        [[nodiscard]] static Vector<Signature> Signatures();

        /// <summary>
        /// Game loop called every tick
        /// </summary>
        /// <param name="time"></param>
        virtual void Update(const GameTime& time);
    
    protected:
        /// <summary>
        /// Constructor.
        /// When inheriting from this class, dervived constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
		explicit GameObject(T* derivedThis);

        /// <summary>
        /// Copy constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to copy</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
        GameObject(const GameObject& rhs, T* derivedThis);

        /// <summary>
        /// Move constructor.
        /// When inheriting from this class, dervived copy constructors must call this method
        /// and pass in 'this' into the derivedThis parameter. This parameter allows this class
        /// to called derived class static methods inside the constructor.
        /// </summary>
        /// <param name="rhs">Right hand side to move</param>
        /// <param name="derivedThis">Pass in 'this' as an argument to this param</param>
		template<std::derived_from<Attributed> T>
        GameObject(GameObject&& rhs, T* derivedThis) noexcept;
	};

    ConcreteFactory(GameObject, Scope);
}

#include "GameObject.inl"