#pragma once

#include <TAC/TACNode.hpp>
#include <memory>
#include <ostream>
#include <tuple>

namespace Crust {
class Quad {
    Operator* instr = nullptr;
    Address* arg1 = nullptr;
    Address* arg2 = nullptr;
    Address* res = nullptr;

   public:
    Quad() = default;

    Quad(Operator* instr, Address* arg1, Address* arg2, Address* res)
        : instr{instr},
          arg1{arg1},
          arg2{arg2},
          res{res} {
    }

    void setInstr(Operator* _instr) { instr = _instr; }
    void setArg1(Address* _arg1) { arg1 = _arg1; }
    void setArg2(Address* _arg2) { arg2 = _arg2; }
    void setRes(Address* _res) { res = _res; }

    Operator* getInstr() { return instr; }
    Address* getArg1() { return arg1; }
    Address* getArg2() { return arg2; }
    Address* getRes() { return res; }

    friend std::ostream& operator<<(std::ostream& stream, Quad& quad) {
        Operator* instr = quad.getInstr();
        Address* arg1 = quad.getArg1();
        Address* arg2 = quad.getArg2();
        Address* res = quad.getRes();

        stream << "| ";
        if (instr) {
            stream << *instr << " | ";
        } else {
            stream << " - | ";
        }

        if (arg1) {
            stream << *arg1 << " | ";
        } else {
            stream << " - | ";
        }

        if (arg2) {
            stream << *arg2 << " | ";
        } else {
            stream << " - | ";
        }

        if (res) {
            stream << *res << " |";
        } else {
            stream << " - |";
        }

        return stream;
    }
};
}  // namespace Crust