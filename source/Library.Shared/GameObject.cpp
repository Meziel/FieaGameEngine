#include "pch.h"

#include <memory>
#include "GameObject.h"
#include "Action.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(GameObject, FieaGameEngine::Attributed);

    GameObject::GameObject() : GameObject(this)
    {
    }

    GameObject::GameObject(const GameObject& rhs) :
        Name(rhs.Name),
        Transform(rhs.Transform),
        Attributed(rhs, this)
    {
    }

    GameObject::GameObject(GameObject&& rhs) noexcept :
        Name(std::move(rhs.Name)),
        Transform(std::move(rhs.Transform)),
        Attributed(std::move(rhs), this)
    {
    }

    Datum& GameObject::Children()
    {
        return At("Children"s);
    }

    const Datum& GameObject::Children() const
    {
        return At("Children"s);
    }

    Datum& GameObject::Actions()
    {
        return At("Actions"s);
    }

    const Datum& GameObject::Actions() const
    {
        return At("Actions"s);
    }

    Datum& GameObject::Reactions()
    {
        return At("Reactions"s);
    }

    const Datum& GameObject::Reactions() const
    {
        return At("Reactions"s);
    }

    Action& GameObject::CreateAction(std::string actionClass, std::string name)
    {
        shared_ptr<Scope> newAction = Factory<Scope>::Create(actionClass);
        if (newAction == nullptr)
        {
            throw runtime_error(std::format("Action {} does not exist", actionClass));
        }
        assert(newAction->Is(Action::TypeIdClass()));
        Action& newActionRef = static_cast<Action&>(*newAction);
        newActionRef.Name = name;
        Adopt(newAction, "Actions"s);
        return newActionRef;
    }

    std::unique_ptr<Scope> GameObject::Clone() const
    {
        return make_unique<GameObject>(*this);
    };

    Vector<Signature> GameObject::Signatures()
    {
        return
        {
            {"Name", DatumTypes::String, true, Signature::ExternalInfo{1, offsetof(GameObject, Name)}},
            {"Transform", DatumTypes::TablePointer, true, Signature::ExternalInfo{1, offsetof(GameObject, Transform)}},
            {"Children", DatumTypes::Table, false},
            {"Actions", DatumTypes::Table, false},
            {"Reactions", DatumTypes::Table, false}
        };
    }

    void GameObject::Update(const GameTime& time)
    {
        // Call update on all children
        Datum& children = Children();
        for (size_t i = 0; i < children.Size(); ++i)
        {
            Scope& childScope = *children.Get<shared_ptr<Scope>>(i);
            assert(childScope.Is(GameObject::TypeIdClass()));
            static_cast<GameObject&>(childScope).Update(time);
        }

        // Call update on all actions
        Datum& action = Actions();
        for (size_t i = 0; i < action.Size(); ++i)
        {
            Scope& actionScope = *action.Get<shared_ptr<Scope>>(i);
            assert(actionScope.Is(Action::TypeIdClass()));
            static_cast<Action&>(actionScope).Update(time);
        }
    }
}