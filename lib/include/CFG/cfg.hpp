#pragma once

#include <assert.h>

#include <common/sourceloc.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <utils/uid.hpp>
#include <vector>

namespace Crust {

class CFGNode;
using ChildrenNode = std::vector<std::unique_ptr<CFGNode>>;

class CFGNode {
   public:
    enum class NodeKind : uint32_t {
        // Declaration

        PROG_DECL,

        DECL_LIST,
        DECL,

        VAR_DECL,
        ATOMIC_DECL_LIST,
        ATOMIC_DECL,

        FN_DECL,
        ARG_LIST,

        EXPRESSION,
        EXPRESSION_RHS,

        TERM,
        ID_OR_CALL_TERM,
        CALL,
        FORMAL_ARG_LIST,

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

    void addChildNode(std::unique_ptr<CFGNode>&& node) { mChildren.push_back(std::move(node)); }

    // TODO: Write to file directly instead of stdout?
    void generateDotFile() {
        std::cout << "digraph CFG {\n";
        std::cout << "\tgraph [ dpi = 300 ];\n";
        std::cout << "\tfontname=\"Helvetica,Arial,sans-serif\"\n";
        std::cout << "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n";
        std::cout << "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n";
        std::cout << "\tnode [shape = circle];\n";

        _generateDotFileHelper();
        std::cout << "}\n";
    }

    friend std::ostream& operator<<(std::ostream& stream, const NodeKind& kind) {
        switch (kind) {
            case NodeKind::PROG_DECL:
                stream << "PROG_DECL";
                break;
            case NodeKind::DECL_LIST:
                stream << "DECL_LIST";
                break;
            case NodeKind::DECL:
                stream << "DECL";
                break;
            case NodeKind::VAR_DECL:
                stream << "VAR_DECL";
                break;
            case NodeKind::ATOMIC_DECL_LIST:
                stream << "ATOMIC_DECL_LIST";
                break;
            case NodeKind::ATOMIC_DECL:
                stream << "ATOMIC_DECL";
                break;

            case NodeKind::FN_DECL:
                stream << "FN_DECL";
                break;
            case NodeKind::ARG_LIST:
                stream << "ARG_LIST";
                break;
            case NodeKind::EXPRESSION:
                stream << "EXPRESSION";
                break;
            case NodeKind::EXPRESSION_RHS:
                stream << "EXPRESSION_RHS";
                break;
            case NodeKind::TERM:
                stream << "TERM";
                break;
            case NodeKind::ID_OR_CALL_TERM:
                stream << "ID_OR_CALL_TERM";
                break;
            case NodeKind::CALL:
                stream << "CALL";
                break;

            case NodeKind::FORMAL_ARG_LIST:
                stream << "FORMAL_ARG_LIST";
                break;
            case NodeKind::STMT_LIST:
                stream << "STMT_LIST";
                break;
            case NodeKind::STMT:
                stream << "STMT";
                break;
            case NodeKind::ASSIGNMENT_STMT:
                stream << "ASSIGNMENT_STMT";
                break;
            case NodeKind::CONDITIONAL_STMT:
                stream << "CONDITIONAL_STMT";
                break;
            case NodeKind::LOOP_STMT:
                stream << "LOOP_STMT";
                break;

            case NodeKind::RETURN_STMT:
                stream << "RETURN_STMT";
                break;
            case NodeKind::IF_BLOCK:
                stream << "IF_BLOCK";
                break;
            case NodeKind::ELIF_BLOCKS:
                stream << "ELIF_BLOCKS";
                break;
            case NodeKind::ELIF_BLOCK:
                stream << "ELIF_BLOCK";
                break;
            case NodeKind::ELSE_BLOCK:
                stream << "ELSE_BLOCK";
                break;
            case NodeKind::FOR_LOOP:
                stream << "FOR_LOOP";
                break;

            case NodeKind::LOOP_RANGE:
                stream << "LOOP_RANGE";
                break;
            case NodeKind::LOOP_STEP:
                stream << "LOOP_STEP";
                break;
            case NodeKind::WHILE_LOOP:
                stream << "WHILE_LOOP";
                break;
            case NodeKind::RETURN_VAR:
                stream << "RETURN_VAR";
                break;
            case NodeKind::TYPE:
                stream << "TYPE";
                break;
            case NodeKind::TOKEN:
                stream << "TOKEN";
                break;

            case NodeKind::ERROR:
                stream << "ERROR";
                break;

            default:
                stream << "Default Case";
                break;
        }
        return stream;
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
                stream << "FATAL ERROR: Somethign went wrong when parsing children of node: " << node.getKind() << ". \n";
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
        return static_cast<T*>(mChildren[childIdx].get());
    }

   protected:
    uint64_t mUid;
    NodeKind mKind;
    std::string mName;
    ChildrenNode mChildren;
    SourceLocation mSrcLoc;

   protected:
    void _generateDotFileHelper() {
        for (const auto& child : getChildrenNodes()) {
            std::cout << getName() << "_" << getUID() << "->" << child->getName() << "_" << child->getUID() << "\n";
        }

        for (const auto& child : getChildrenNodes()) {
            child->_generateDotFileHelper();
        }
    }
};
}  // namespace Crust