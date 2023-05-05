#include "pch.h"
#include "Action.h"
#include "memory"

using namespace std;

namespace FieaGameEngine
{
    ATTRIBUTED_DEFINITIONS(Action, FieaGameEngine::Attributed);

    Vector<Signature> Action::Signatures()
    {
        return
        {
            {"Name", DatumTypes::String, true, Signature::ExternalInfo{1, offsetof(Action, Name)}}
        };
    }
}
