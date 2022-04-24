#pragma once

#include <ostream>
#include <parser/lexer.hpp>
#include <string>

namespace Crust {

class Address {
   public:
    enum class Type : unsigned {
        NAME,
        CONST,
        TEMP,
        LABEL,
        NONE
    };

    Address() {}
    Address(std::string identifier) : identifier(identifier) {}
    Address(std::string identifier, Type type) : identifier(identifier), type(type) {}

    friend std::ostream& operator<<(std::ostream& stream, const Address& addr) {
        switch (addr.type) {
            case Type::NAME:
                stream << "[NAME] ";
                break;

            case Type::CONST:
                stream << "[CONST] ";
                break;

            case Type::TEMP:
                stream << "[TEMP] ";
                break;

            case Type::LABEL:
                stream << "[LABEL] ";
                break;

            case Type::NONE:
                stream << "[NONE] ";
                break;

            default:
                break;
        }
        stream << addr.identifier;
        return stream;
    }

   protected:
    std::string identifier;
    Type type = Type::NONE;
};

class Name : public Address {
   public:
    Name(std::string identifier) : Address(identifier, Address::Type::NAME) {}
};

class Constant : public Address {
   public:
    Constant(int val) : Address(std::to_string(val), Address::Type::CONST) {}
    Constant(float val) : Address(std::to_string(val), Address::Type::CONST) {}
    Constant(bool val) : Address(std::to_string(val), Address::Type::CONST) {}
    Constant(std::string val) : Address(val, Address::Type::CONST) {}
};

class TempVar : public Address {
   public:
    TempVar() : Address("", Address::Type::TEMP) {
        static uint32_t currentCount = 0;
        identifier = "T" + std::to_string(currentCount++);
    }
};

class Label : public Address {
   public:
    Label() : Address("", Address::Type::LABEL) {
        static uint32_t currentCount = 0;
        identifier = "L" + std::to_string(currentCount++);
    }
};

class Operator {
   public:
    enum class Type : unsigned {
        OPERATOR,
        NONE
    };

   public:
    Operator(Lexer::Token token) : instr(Lexer::token_to_str[(size_t)token]), type(Type::OPERATOR) {}
    Operator(std::string unary) : instr(unary), type(Type::OPERATOR) {}
    Operator() : type(Type::NONE) {}

    friend std::ostream& operator<<(std::ostream& stream, const Operator& op) {
        switch (op.type) {
            case Type::OPERATOR:
                stream << "[OPERATOR] ";
                break;

            case Type::NONE:
                stream << "[NONE] ";
                break;

            default:
                break;
        }
        stream << op.instr;
        return stream;
    }

   protected:
    std::string instr = "";
    Type type = Type::NONE;
};

}  // namespace Crust