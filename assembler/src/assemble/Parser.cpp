//
// Created by babkamen on 8/18/24.
//

#include "Parser.h"

#include <set>
#include <vector>
#include "Lexer.h"
#include <cpptrace/cpptrace.hpp>
#include "exception.h"

map<string, int> Parser::predefined_symbols = {
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
    {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
    {"SCREEN", 16384}, {"KBD", 24576}
};

const set<array<TokenType, 2> > possible_2_identifiers_permutaions = {{A, M}, {M, D}, {A, D}};


void Parser::parse_only_labels(const vector<Token> &tokens) {
    this->tokens = tokens;
    it = tokens.begin();
    if (!tokens.empty() && it->category == LabelCategory) {
        const auto symbol = eat(it, it->type);
        if (symbol_table.contains(symbol.text)) {
            throw duplicate_label_exception("Duplicate label: " + symbol.text);
        }
        symbol_table.insert({symbol.text, line_number});
    } else {
        line_number++;
    }
}

void Parser::reset() {
    line_number = 0;
}

/**
*
* Rearrange tokens into a tree
* Line can have asignment, operation and a jump
*/
unique_ptr<TreeNode> Parser::parse(const vector<Token> &tokens) {
    //TODO: will this copy struct or no?
    this->tokens = tokens;
    it = tokens.begin();
    if (tokens.empty()) {
        throw cpptrace::logic_error("empty tokens");
    }
    unique_ptr<TreeNode> root;

    switch (it->category) {
        case AtCategory: {
            root = make_unique<TreeNode>(eat(it, At));
            if (it->category == NumberCategory || it->category == PredefinedConstant) {
                root->right = make_unique<TreeNode>(eat(it, it->type));
            } else if (it->category == SymbolCategory) {
                const auto symbol = eat(it, it->type);
                int number;
                if (symbol_table.contains(symbol.text)) {
                    number = symbol_table.at(symbol.text);
                } else {
                    number = free_address;
                    symbol_table.insert({symbol.text, number});
                    free_address++;
                }
                root->right = make_unique<TreeNode>(Token(Number, symbol.startPos, number));
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
        case Identifier:
            root = make_unique<TreeNode>(eat(it, it->type));
            if (hasMoreTokens()) {
                if (it->category == Jump) {
                    root->right = make_unique<TreeNode>(eat(it, it->type));
                } else if (it->category == AssignmentOperation) {
                    root = assigment_statement(root, it);
                } else if (it->category == OtherOperation) {
                    root = operator_statement(it, std::move(root));
                } else if (it->category == Identifier) {
                    //2 identifiers
                    auto identifier = make_unique<TreeNode>(eat(it, it->type));
                    identifier->left = std::move(root);


                    if (it->category == Identifier) {
                        //3 identifiers
                        identifier->right = make_unique<TreeNode>(eat(it, it->type));

                        if (identifier->token.type != M || identifier->left->token.type != A || identifier->right->token
                            .type != D) {
                            //throw exception
                            string s = "Expected AMD but got ";
                            throw invalid_identifiers_order_before_assignment_exception(s +
                                Token::toString(identifier->left->token.type) +
                                Token::toString(identifier->token.type) +
                                +Token::toString(identifier->right->token.type));
                        }
                    } else if (possible_2_identifiers_permutaions.contains({
                        identifier->token.type, identifier->left->token.type
                    })) {
                        //convert possible_2_identifiers_permutaions to string
                        string s1;
                        for (auto t: possible_2_identifiers_permutaions) {
                            s1 += Token::toString(t[0]);
                            s1 += Token::toString(t[1]);
                            s1 += " ,";
                        }
                        const string s2 = "Invalid order of identifiers: ";
                        throw invalid_identifiers_order_before_assignment_exception(
                            s2 + Token::toString(identifier->left->token.type) + Token::toString(identifier->token.type)
                            +
                            ".Possible values " + s1
                        );
                    }

                    if (it->category != AssignmentOperation) {
                        string s = "Expected assignment but got ";
                        throw cpptrace::logic_error(s + Token::toString(it->type));
                    }
                    root = std::move(identifier);
                    root = assigment_statement(root, it);
                } else {
                    string s = "Unexpected token ";
                    throw cpptrace::logic_error(s + Token::toString(it->type));
                }
            }
            break;
        case LabelCategory: {
            eat(it, it->type);
            break;
        }
        default:
            string s = "Unexpected operator ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    if (it->type != Eof && it->type != EOL) {
        string s = "Expected end of line or end of file but got ";
        throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    line_number++;
    return root;
}


unique_ptr<TreeNode> Parser::operator_statement(vector<Token>::const_iterator &it,
                                                unique_ptr<TreeNode> operationLeftIdentifier) const {
    auto operation = make_unique<TreeNode>(eat(it, it->type));
    operation->left = std::move(operationLeftIdentifier);
    if (it->category == Identifier) {
        // M=A-D
        operation->right = make_unique<TreeNode>(eat(it, it->type));
        if (operation->left != nullptr && operation->left->token.type == A) {
            string s = "A cant be the first operand. Did you mean ";
            throw invalid_operand_order_exception(s +
                                                  Token::toString(operation->right->token.type) +
                                                  Token::toString(operation->token.type) +
                                                  Token::toString(operation->left->token.type) + "?"
            );
        }
        // M=A-D ; JMP
        if (it->category == Jump) {
            operation->right->right = make_unique<TreeNode>(eat(it, it->type));
        }
    } else if (it->category == PredefinedConstant) {
        // M = A-1
        operation->right = make_unique<TreeNode>(eat(it, it->type));
        if (operation->right->token.type == NegativeOne || operation->right->token.type == Zero) {
            string s = "Invalid operation: ";
            throw syntax_error_exception(s + Token::toString(operation->token.type) +
                                         Token::toString(operation->right->token.type));
        }
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
        case OtherOperation: {
            if (it->type != Minus && it->type != Not) {
                string s = "Unexpected token after assignment: ";
                throw cpptrace::logic_error(s + Token::toString(it->type));
            }
            assignment->right = operator_statement(it, nullptr);
            break;
        }
        default:
            string s = "Unexpected token after assignment: ";
            throw cpptrace::logic_error(s + Token::toString(it->type));
    }
    return assignment;
}


Token Parser::eat(vector<Token>::const_iterator &it, TokenType type) const {
    auto end = tokens.end();
    if (it == end) {
        string s = "Unexpected end of input, expected ";
        throw cpptrace::logic_error(s + Token::toString(type));
    }
    if (it->type != type) {
        string s = "expected ";
        throw cpptrace::logic_error(s + Token::toString(type) + " but got " + Token::toString(it->type));
    }

    Token res = *it;
    ++it;
    return res;
}
