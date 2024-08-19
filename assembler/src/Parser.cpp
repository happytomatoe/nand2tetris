//
// Created by babkamen on 8/18/24.
//

#include "Parser.h"

#include <vector>
#include "Lexer.h"
#include <cpptrace/cpptrace.hpp>


Token eat(vector<Token>::iterator t, TokenType type);

unique_ptr<TreeNode> Parser::operator_statement(vector<Token>::iterator it,
                                                unique_ptr<TreeNode> operationLeftIdentifier) {
    auto operation = make_unique<TreeNode>(eat(it, it->type));
    operation->left = std::move(operationLeftIdentifier);
    if (it->category == Identifier) {
        // M=A-D
        operation->right = make_unique<TreeNode>(eat(it, it->type));
        // M=A-D ; JMP
        if (it->category == Jump) {
            operation->right->right = make_unique<TreeNode>(eat(it, it->type));
        }
    } else if (it->category == PredefinedConstant) {
        // M = A-1
        operation->right = make_unique<TreeNode>(eat(it, it->type));
        // M = A -1; JMP
        if (it->category == Jump) {
            operation->right->right = make_unique<TreeNode>(eat(it, it->type));
        }
    }
    return operation;
}

void Parser::assigment_statement(unique_ptr<TreeNode> &assignmentIdentifier, vector<Token>::iterator it) {
    // assignment
    unique_ptr<TreeNode> assignment = make_unique<TreeNode>(eat(it, it->type));
    assignment->left = std::move(assignmentIdentifier);

    // D=A-1
    // D=M&-1;JGE
    // D=1;JMP
    switch (it->category) {
        // M=1
        case PredefinedConstant: {
            assignment->right = make_unique<TreeNode>(eat(it, it->type));
            // M=1;JMP
            if (it->category == Jump) {
                assignment->right->right = make_unique<TreeNode>(eat(it, it->type));
            } else {
                string s = "Unexpected token after constant: ";
                throw cpptrace::logic_error(s + Token::toString(it->type));
            }
            break;
        }
        // M=A
        case Identifier: {
            auto operationLeftIdentifier = make_unique<TreeNode>(eat(it, it->type));
            // M=A; JMP
            if (it->category == Jump) {
                operationLeftIdentifier->right = make_unique<TreeNode>(eat(it, it->type));
                operationLeftIdentifier->left = std::move(assignment->right);
                // M=A-
            } else if (it->category == OtherOperation) {
                assignment->right = operator_statement(it, std::move(operationLeftIdentifier));;
            } else {
                string s = "Unexpected token after identifier: ";
                throw cpptrace::logic_error(s + Token::toString(it->type));
            }
            break;
        }
        default:
            string s = "Unexpected token after identifier: ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
}

/**
 *
 * Rearrange tokens into a tree
 * Line can have asignment, operation and a jump
 */
TreeNode *Parser::parse(vector<Token> *tokens) {
    if (tokens->empty()) {
        throw cpptrace::logic_error("empty tokens");
    }

    unique_ptr<TreeNode> root;
    // a instruction
    auto it = tokens->begin();
    switch (it->category) {
        case AtCategory: {
            root = make_unique<TreeNode>(eat(it, At));
            root->right = make_unique<TreeNode>(eat(it, Number));
            break;
        }
        // c instruction
        //constant
        case PredefinedConstant: {
            root = make_unique<TreeNode>(eat(it, it->type));
            // constant and jump
            if (it->category == Jump) {
                root->right = make_unique<TreeNode>(eat(it, it->type));
            }
            break;
        }
        //identifier
        //TODO: what about a case with multiple identifiers?
        case Identifier:
            root = make_unique<TreeNode>(eat(it, it->type));
            if (it->category == Jump) {
                root->right = make_unique<TreeNode>(eat(it, it->type));
                break;
            }
            if (it->category == AssignmentOperation) {
                assigment_statement(root, it);
                break;
            }
        case OtherOperation:
            // operation
            // operation and jump
            root->right = make_unique<TreeNode>(eat(it, it->type));
        default:
            string s = "Unexpected operator ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    if (it->type != Eof && it->type != EOL) {
        string s = "Expected end of line or end of file but got ";
        throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    return root.release();
}

Token eat(vector<Token>::iterator t, TokenType type) {
    if (t == nullptr) {
        string s = "Unexpected end of input, expected ";
        throw new cpptrace::logic_error(s + Token::toString(type));
    }
    if (t->type != type) {
        string s = "expected ";
        throw cpptrace::logic_error(s + Token::toString(type) + " but got " + Token::toString(t->type));
    }

    Token res = *t;
    t++;
    return res;
}
