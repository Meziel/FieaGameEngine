#include "pch.h"
#include "ActionList.h"
#include <memory>

using namespace std;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(ActionList, FieaGameEngine::Action);

    ActionList::ActionList() : Action(this)
    {
    }

    ActionList::ActionList(const ActionList& rhs) :
        Action(rhs, this)
    {
    }

    ActionList::ActionList(ActionList&& rhs) noexcept :
        Action(std::move(rhs), this)
    {
    }

    Datum& ActionList::Actions()
    {
        return At("Actions"s);
    }

    const Datum& ActionList::Actions() const
    {
        return At("Actions"s);
    }

    Action& ActionList::CreateAction(std::string actionClass, std::string name)
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

    std::unique_ptr<Scope> ActionList::Clone() const
    {
        return make_unique<ActionList>(*this);
    };

    Vector<Signature> ActionList::Signatures()
    {
        return
        {
            {"Actions", DatumTypes::Table, false}
        };
    }

    void ActionList::Update(const GameTime& time)
    {
        Datum& actions = (*this)["Actions"s];
        for (size_t i = 0; i < actions.Size(); ++i)
        {
            Scope& action = *actions.Get<shared_ptr<Scope>>(i);
            assert(action.Is(Action::TypeIdClass()));
            static_cast<Action&>(action).Update(time);
        }
    }
}