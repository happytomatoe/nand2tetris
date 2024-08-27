#pragma once
#include <set>
#include <vector>

#include "exception.h"
#include "Token.h"

using namespace std;
using namespace token;

class Validator {
public:
    static set<string> scanLabels(const std::vector<token::Token> &tokens) {
        set<string> labels;
        auto line_number = 0;
        for (int i = 0; i < tokens.size(); ++i, line_number++) {
            if (tokens[i].category == LabelCategory) {
                const auto label = tokens[i].label;
                if(label.empty()) {
                   throw EmptyLabelException(line_number);
                }
                if (labels.contains(label)) {
                    throw DuplicateIdentifierException(line_number, label);
                } else {
                    labels.insert(label);
                }
            }
        }

        return labels;
    }

    static void checkOrder(const vector<Token> &tokens) {
        set<string> labels = scanLabels(tokens);

        auto line_number = 0;
        auto size = tokens.size();
        for (int i = 0; i < size; ++i, line_number++) {
            switch (tokens[i].category) {
                case Terminal:
                    continue;
                // 2 types of operations
                //move operation+memory segment+ number
                case MoveOperation: {
                    const auto memory_segment = next(MemorySegment, tokens, i, line_number);
                    i++;
                    const auto number = next(NumberCategory, tokens, i, line_number);
                    i++;
                    if (memory_segment.type != Constant && number.number < 0) {
                        throw NumberOutOfRangeException(line_number,
                                                        "Expected a non negative number but got " + to_string(
                                                            number.number));
                    }
                    break;
                }
                //already handled
                case LabelCategory:
                // AL operation
                case ArithmeticOrLogicOperation: {
                    break;
                }
                case GoToCategory: {
                    const auto label = tokens[i].label;
                    if(label.empty()) {
                        throw EmptyLabelException(line_number);
                    }
                    if (!labels.contains(label)) {
                        throw GoToUndeclaredLabel(line_number, label);
                    }
                    break;
                }
                case IfGoToCategory: {
                    const auto label = tokens[i].label;
                    if(label.empty()) {
                        throw EmptyLabelException(line_number);
                    }
                    if (!labels.contains(label)) {
                        throw IfGoToUndeclaredLabel(line_number, label);
                    }
                    break;
                }
                default:
                    throw InvalidOperation(line_number);
            }
            if (++i < tokens.size()) {
                if (tokens[i].category != Terminal) {
                    const string value = tokens[i].type == Number
                                             ? to_string(tokens[i].number)
                                             : toString(tokens[i].type);

                    const string s = "Expected end of line or end of file but got ";
                    throw UnexpectedToken(line_number, s + value);
                }
            }
        }
    };

    static Token next(const Category category, const vector<Token> &tokens, int i, const int &line_number) {
        if (++i == tokens.size()) {
            const string s = "Unexpected end of input, expected ";
            throw UnexpectedToken(line_number, s + toString(category));
        }
        const auto &token = tokens[i];
        if (token.category != category) {
            const string s = "Expected ";
            throw UnexpectedToken(
                line_number, s + toString(category) + " but got " + toString(token.type));
        }
        return token;
    }
};
