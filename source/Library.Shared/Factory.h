#pragma once

#include <string>
#include <memory>
#include "RTTI.h"
#include "HashMap.h"

namespace FieaGameEngine
{
    template <typename T>
    class Factory
    {
    public:
        virtual ~Factory() = default;
        
        /// <summary>
        /// Given a class name, returns a factory that can create an object of that class.
        /// If no such factory exists that is registered, then nullptr is returned.
        /// </summary>
        /// <param name="className">Name of a class</param>
        /// <returns>Factory that can create instances of type className or nullptr if no such factory exists</returns>
        [[nodiscard]] static Factory* Find(const std::string& className);
        
        /// <summary>
        /// Give a class name, creates an object of that type. A factory that can create instances
        /// of that class type needs to be added to the abstract factory before the create can return non-null. 
        /// </summary>
        /// <param name="className">Name of a class to instantiate an object from</param>
        /// <returns>Returns a default construct object of type class name. If the object cannot be created, nullptr is returned</returns>
        [[nodiscard]] static std::unique_ptr<T> Create(const std::string& className);
        
        /// <summary>
        /// Produces an object that this factory creates
        /// </summary>
        /// <returns>Newly created object</returns>
        [[nodiscard]] virtual std::unique_ptr<T> Create() const = 0;
        
        /// <summary>
        /// Returns the class as a string that this factory makes instances of 
        /// </summary>
        /// <returns>Class name as a string</returns>
        [[nodiscard]] virtual std::string ClassName() const = 0;
        
        /// <summary>
        /// Adds a factory to the abstract factory
        /// </summary>
        /// <param name="factory">Factory to add</param>
        static void Add(std::unique_ptr<Factory> factory);
        
        /// <summary>
        /// Removes a factory from the abstract factory
        /// </summary>
        /// <param name="factory">Factory to remove</param>
        static void Remove(const Factory& factory);
        
        /// <summary>
        /// Removes all factories from the abstract factory
        /// </summary>
        static void Clear();
    private:
        inline static HashMap<std::string, std::unique_ptr<Factory>> _factories;
    };
}                                        

#define ConcreteFactory(ConcreteProductType, AbstractProductType)                                     \
class ConcreteProductType##Factory final : public FieaGameEngine::Factory<AbstractProductType>        \
{                                                                                                     \
public:                                                                                               \
    [[nodiscard]] inline std::unique_ptr<AbstractProductType> Create() const override                 \
    {                                                                                                 \
        return std::unique_ptr<AbstractProductType>(std::make_unique<ConcreteProductType>());         \
    }                                                                                                 \
                                                                                                      \
    [[nodiscard]] inline std::string ClassName() const override                                       \
    {                                                                                                 \
        return #ConcreteProductType;                                                                  \
    }                                                                                                 \
};

#include "Factory.inl"
