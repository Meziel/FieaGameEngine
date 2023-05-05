#include "Factory.h"

namespace FieaGameEngine
{
    template <typename T>
    inline Factory<T>* Factory<T>::Find(const std::string& className)
    {
        auto it = _factories.Find(className);
        return it != _factories.end() ? it->second.get() : nullptr;
    }

    template <typename T>
    inline std::unique_ptr<T> Factory<T>::Create(const std::string& className)
    {
        Factory* factory = Find(className);
        return factory != nullptr ? factory->Create() : nullptr;
    }

    template <typename T>
    inline void Factory<T>::Add(std::unique_ptr<Factory> factory)
    {
        _factories.Insert({ factory->ClassName(), std::move(factory) });
    }

    template <typename T>
    inline void Factory<T>::Remove(const Factory& factory)
    {
        _factories.Remove(factory.ClassName());
    }

    template <typename T>
    inline void Factory<T>::Clear()
    {
        _factories.Clear();
    }
}
