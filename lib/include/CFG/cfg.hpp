#pragma once

#include <assert.h>

#include <TAC/Quad.hpp>
#include <common/sourceloc.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <parser/lexer.hpp>
#include <set>
#include <string>
#include <utils/uid.hpp>
#include <vector>

namespace Crust {

class CFGNode;
using ChildrenNode = std::vector<CFGNode*>;

class CFGNode {
   public:
    enum class NodeKind : uint32_t {
        // Declaration

        PROG_DECL,

        DECL_LIST,
        DECL,

        VAR_DECL,
        VAR_DECL_LIST,
        VAR_DECL_LIST_,

        FN_DECL,
        FN_PARAM_LIST,
        FN_PARAM_LIST_,
        FN_PARAM,

        EXPRESSION,
        EXPRESSION_RHS,

        TERM,
        FLOAT_TERM,
        ARRAY_SUBSCRIPT,
        CALL,
        CALL_PARAM_LIST,
        CALL_PARAM,
        CALL_PARAM_LIST_,

        STMT_LIST,
        STMT,
        ASSIGNMENT_STMT,
        CONDITIONAL_STMT,
        LOOP_STMT,
        RETURN_STMT,

        IF_BLOCK,
        ELIF_BLOCKS,
        ELIF_BLOCK,
        ELSE_BLOCK,

        FOR_LOOP,
        LOOP_RANGE,
        LOOP_STEP,

        WHILE_LOOP,

        RETURN_VAR,

        // Misc
        SEGMENT,
        TYPE,
        TOKEN,
        ERROR
    };

   public:
    // explicit CFGNode(NodeKind kind = NodeKind::ERROR) : mUid{UID::generate()}, mKind{kind}, mName{" "} {}
    explicit CFGNode(NodeKind kind = NodeKind::ERROR, std::string name = "") : mUid{UID::generate()}, mKind{kind}, mName{name} {}

    virtual ~CFGNode() = default;

    CFGNode(const CFGNode&) = default;
    CFGNode(CFGNode&&) = default;
    CFGNode& operator=(const CFGNode&) = default;
    CFGNode& operator=(CFGNode&&) = default;

   public:
    uint64_t getUID() const { return mUid; }
    NodeKind getKind() const { return mKind; }  // Why not const?
    const std::string& getName() const { return mName; }
    const ChildrenNode& getChildrenNodes() const { return mChildren; }
    const SourceLocation& getSourceLocation() const { return mSrcLoc; }
    std::vector<Quad>& getCode() { return mCode; }

    void addChildNode(CFGNode* node) { mChildren.push_back(node); }
    void addCode(std::vector<Quad>& code) { mCode.insert(mCode.end(), code.begin(), code.end()); }
    void addCode(Quad& code) { mCode.push_back(code); }
    void addCode(Operator* instr, Address* arg1, Address* arg2, Address* res) { mCode.emplace_back(instr, arg1, arg2, res); }

    void generateDotFile(std::string fname) {
        std::cout << "Writing dot file to: " << fname << "\n";
        std::ofstream stream(fname);

        stream << "digraph CFG {\n";
        _generateDotFileHelper(stream);
        stream << "}\n";
    }

    void writeTAC(std::string fname) {
        std::cout << "Writing Intermediate Code to: " << fname << "\n";
        std::ofstream stream(fname);
        stream << "| Operation | Arg1 | Arg2 | Res |\n";
        stream << "|---|---|---|---|\n";
        for (auto& quad : mCode) {
            stream << quad << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, const CFGNode& node) {
        static int indentLevel = 0;

        stream << node.getName();

        if (node.getKind() == CFGNode::NodeKind::TOKEN) {
            stream << "\n";
            return stream;
        }

        stream << "(\n";

        for (const auto& child : node.getChildrenNodes()) {
            indentLevel += 2;
            for (int i = 0; i < indentLevel; ++i) stream << " ";
            if (!child) {
#ifndef NDEBUG
                stream << "FATAL ERROR: Somethign went wrong when parsing children of node: " << (uint32_t)node.getKind() << ". \n";
#endif
                continue;
            }
            stream << *child;
            indentLevel -= 2;
        }

        for (int i = 0; i < indentLevel; ++i) stream << " ";
        stream << ")\n";
        return stream;
    }

   protected:
    template <class T>
    T* getChildNodeAs(std::size_t childIdx) const {
        assert(childIdx < mChildren.size());
        return mChildren[childIdx];
    }

    void _generateDotFileHelper(std::ofstream& stream) {
        for (const auto& child : getChildrenNodes()) {
            stream << "\t" << getName() << "_" << getUID() << "->" << child->getName() << "_" << child->getUID() << "\n";
        }

        for (const auto& child : getChildrenNodes()) {
            child->_generateDotFileHelper(stream);
        }
    }

   public:
    // TODO: Use CRTP to convert this to a static variable
    std::set<Lexer::Token> first;

   protected:
    uint64_t mUid;
    NodeKind mKind;
    std::string mName;
    ChildrenNode mChildren;
    SourceLocation mSrcLoc;

    std::vector<Quad> mCode;
};
}  // namespace Crust