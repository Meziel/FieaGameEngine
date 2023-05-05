#pragma once

#include <string>
#include <glm/glm.hpp>
#include <cstdint>
#include <memory>
#include "HashMap.h"

namespace FieaGameEngine
{
    enum class DatumTypes
    {
        Boolean,
        Integer,
        Float,
        String,
        Vector,
        Matrix,
        Pointer,
        TablePointer,
        Table,
        Unknown
    };

    inline static HashMap<const std::string, DatumTypes> StringToDatumType{
        {"unknown", DatumTypes::Unknown},
        {"bool", DatumTypes::Boolean},
        {"integer", DatumTypes::Integer},
        {"float", DatumTypes::Float},
        {"string", DatumTypes::String},
        {"vector", DatumTypes::Vector},
        {"matrix", DatumTypes::Matrix},
        {"pointer", DatumTypes::Pointer},
        {"rawtable", DatumTypes::TablePointer},
        {"table", DatumTypes::Table},
    };

    class RTTI;
    class Scope;

    template <typename T>
    [[nodiscard]] consteval DatumTypes ToDatumType() = delete;

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<bool>()
    {
        return DatumTypes::Boolean;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<int32_t>()
    {
        return DatumTypes::Integer;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<float>()
    {
        return DatumTypes::Float;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<std::string>()
    {
        return DatumTypes::String;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<glm::vec4>()
    {
        return DatumTypes::Vector;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<glm::mat4>()
    {
        return DatumTypes::Matrix;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<RTTI*>()
    {
        return DatumTypes::Pointer;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<Scope*>()
    {
        return DatumTypes::TablePointer;
    }

    template <>
    [[nodiscard]] consteval DatumTypes ToDatumType<std::shared_ptr<Scope>>()
    {
        return DatumTypes::Table;
    }
}