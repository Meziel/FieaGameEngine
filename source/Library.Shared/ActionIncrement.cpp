#include "pch.h"
#include "ActionIncrement.h"
#include <memory>

using namespace std;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(ActionIncrement, FieaGameEngine::Action);

    ActionIncrement::ActionIncrement() : Action(this)
    {
    }

    ActionIncrement::ActionIncrement(const ActionIncrement& rhs) :
        Target(rhs.Target),
        Action(rhs, this)
    {
    }

    ActionIncrement::ActionIncrement(ActionIncrement&& rhs) noexcept :
        Target(std::move(rhs.Target)),
        Action(std::move(rhs), this)
    {
    }

    std::unique_ptr<Scope> ActionIncrement::Clone() const
    {
        return make_unique<ActionIncrement>(*this);
    };

    Vector<Signature> ActionIncrement::Signatures()
    {
        return
        {
            {"Target", DatumTypes::String, true, Signature::ExternalInfo{1, offsetof(ActionIncrement, Target)}},
            {"Step", DatumTypes::Unknown, false}
        };
    }

    void ActionIncrement::Update(const GameTime&)
    {
        // Get target
        string targetName = At("Target"s).Get<string>();
        Datum* targetDatum = Search(targetName);
        if (targetDatum == nullptr)
        {
            throw runtime_error(format("Target {} does not exist", targetName));
        }
        if (targetDatum->Type() != DatumTypes::Integer && targetDatum->Type() != DatumTypes::Float)
        {
            throw runtime_error(format("Target {} is not a numeric type", targetName));
        }

        // Get step amount
        Datum& stepDatum = this->Append("Step"s);
        if (stepDatum.Type() == DatumTypes::Unknown)
        {
            // If step doesn't exist, default to 1
            stepDatum.PushBack(1);
        }
        if (stepDatum.Type() != DatumTypes::Integer && stepDatum.Type() != DatumTypes::Float)
        {
            throw runtime_error(format("Step {} is not a numeric type", targetName));
        }

        // Increment target
        switch (targetDatum->Type())
        {
            case DatumTypes::Integer:
                IncrementAs<int32_t>(*targetDatum, stepDatum);
                break;
            case DatumTypes::Float:
                IncrementAs<float>(*targetDatum, stepDatum);
                break;
        }
    }

    template <typename T>
    void ActionIncrement::IncrementAs(Datum& targetDatum, const Datum& stepDatum)
    {
        // Get step as target type
        T incrementAmount;
        switch (stepDatum.Type())
        {
            case DatumTypes::Integer:
                incrementAmount = static_cast<T>(stepDatum.Get<int32_t>());
                break;
            case DatumTypes::Float:
                incrementAmount = static_cast<T>(stepDatum.Get<float>());
                break;
            default:
                incrementAmount = static_cast<T>(1);
                break;
        }

        // Increment target by step
        targetDatum.Set<T>(targetDatum.Get<T>() + incrementAmount);
    }
}