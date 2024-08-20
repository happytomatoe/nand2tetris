//
// Created by babkamen on 8/18/24.
//

#include "Parser.h"

#include <iostream>
#include <vector>
#include "Lexer.h"
#include <cpptrace/cpptrace.hpp>

/**
 *
 * Rearrange tokens into a tree
 * Line can have asignment, operation and a jump
 */
unique_ptr<TreeNode> Parser::parse() {
    if (tokens->empty()) {
        throw cpptrace::logic_error("empty tokens");
    }

    unique_ptr<TreeNode> root;

    switch (it->category) {
        case AtCategory: {
            root = make_unique<TreeNode>(eat(it, At));
            if (it->category == NumberCategory || it->category == PredefinedConstant) {
                root->right = make_unique<TreeNode>(eat(it, it->type));
            } else {
                string s = "Expected number but got ";
                throw cpptrace::logic_error(s + Token::toString(it->type));
            }
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
            if (hasMoreTokens()) {
                if (it->category == Jump) {
                    root->right = make_unique<TreeNode>(eat(it, it->type));
                } else if (it->category == AssignmentOperation) {
                    root = assigment_statement(root, it);
                } else if (it->category == OtherOperation) {
                    root = operator_statement(it, std::move(root));
                } else {
                    string s = "Unexpected token ";
                    throw cpptrace::logic_error(s + Token::toString(it->type));
                }
            }
            break;
        default:
            string s = "Unexpected operator ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    if (it->type != EOL) {
        string s = "Expected end of line or end of file but got ";
        throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    return root;
}

//TODO: A cant be 1 operand. Did you mean...
unique_ptr<TreeNode> Parser::operator_statement(vector<Token>::const_iterator &it,
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
    } else {
        string s = "Expected identifier or constant after operation but got: ";
        throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    return operation;
}

unique_ptr<TreeNode> Parser::assigment_statement(unique_ptr<TreeNode> &assignmentIdentifier,
                                                 vector<Token>::const_iterator &it) {
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
            if (hasMoreTokens()) {
                if (it->category == Jump) {
                    assignment->right->right = make_unique<TreeNode>(eat(it, it->type));
                } else {
                    string s = "Unexpected token after constant: ";
                    throw cpptrace::logic_error(s + Token::toString(it->type));
                }
            }
            break;
        }
        // M=A
        case Identifier: {
            auto operationLeftIdentifier = make_unique<TreeNode>(eat(it, it->type));


            // M=A
            if (hasMoreTokens()) {
                // M=A; JMP
                if (it->category == Jump) {
                    operationLeftIdentifier->right = make_unique<TreeNode>(eat(it, it->type));
                    // M=A-
                } else if (it->category == OtherOperation) {
                    assignment->right = operator_statement(it, std::move(operationLeftIdentifier));;
                } else {
                    string s = "Unexpected token after identifier: ";
                    throw cpptrace::logic_error(s + Token::toString(it->type));
                }
            } else {
                assignment->right = std::move(operationLeftIdentifier);
            }
            break;
        }
        default:
            string s = "Unexpected token after identifier: ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    return assignment;
}


Token Parser::eat(vector<Token>::const_iterator &it, TokenType type) {
    auto end = tokens->end();
    if (it >= end) {
        string s = "Unexpected end of input, expected ";
        throw cpptrace::logic_error(s + Token::toString(type));
    }
    if (it->type != type) {
        string s = "expected ";
        throw cpptrace::logic_error(s + Token::toString(type) + " but got " + Token::toString(it->type));
    }

    Token res = *it;
    it++;
    return res;
}
