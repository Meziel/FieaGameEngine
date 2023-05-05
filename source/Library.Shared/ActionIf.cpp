#include "pch.h"
#include "ActionIf.h"
#include <memory>

using namespace std;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(ActionIf, FieaGameEngine::Action);

    ActionIf::ActionIf() : Action(this)
    {
    }

    ActionIf::ActionIf(const ActionIf& rhs) :
        Condition(rhs.Condition),
        Action(rhs, this)
    {
    }

    ActionIf::ActionIf(ActionIf&& rhs) noexcept :
        Condition(rhs.Condition),
        Action(std::move(rhs), this)
    {
    }

    std::unique_ptr<Scope> ActionIf::Clone() const
    {
        return make_unique<ActionIf>(*this);
    };

    Vector<Signature> ActionIf::Signatures()
    {
        return
        {
            {"Condition", DatumTypes::Boolean, true, Signature::ExternalInfo{1, offsetof(ActionIf, Condition)}},
            {"IfBlock", DatumTypes::Table, false},
            {"ElseBlock", DatumTypes::Table, false}
        };
    }

    void ActionIf::Update(const GameTime& time)
    {
        if (Condition)
        {
            Scope& ifBlock = *At("IfBlock").Get<shared_ptr<Scope>>();
            assert(ifBlock.Is(Action::TypeIdClass()));
            static_cast<Action&>(ifBlock).Update(time);
        }
        else
        {
            Datum* elseBlockDatum = Find("ElseBlock");
            if (elseBlockDatum != nullptr)
            {
                Scope& elseBlock = *elseBlockDatum->Get<shared_ptr<Scope>>();
                assert(elseBlock.Is(Action::TypeIdClass()));
                static_cast<Action&>(elseBlock).Update(time);
            }
        }
    }
}