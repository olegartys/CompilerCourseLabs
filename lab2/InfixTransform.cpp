#include <iostream>

#include "InfixTransform.h"

using namespace std;

std::shared_ptr<std::string> InfixTransform::addExtraChar(const string &infixString) {
    std::shared_ptr<std::string> transformed(new string(""));
    size_t size = infixString.size();

    size_t i = 0;

    for (i = 0; i < size; i++) {
        char fst = infixString.at(i);

        if (i+1 < size) {
            char snd = infixString.at(i+1);

            transformed->push_back(fst);

            if (fst != '(' && fst != '|'
             && snd != ')' && snd != '|' && snd != '*' && snd != '+') {
                transformed->push_back('.');
            }
        }
    }

    transformed->push_back(infixString.at(i-1));

    return transformed;
}

std::shared_ptr<std::string> InfixTransform::toPostfix(const std::string& infixString) {
    std::shared_ptr<std::string> postfix(new string(""));
    std::shared_ptr<std::string> t_expression = addExtraChar(infixString);

    // std::cout << *t_expression << "\n";

    stack<char> extra;
    size_t size = t_expression->size();

    for (size_t i = 0; i < size; i++) {
        char c = t_expression->at(i);

        switch (c) {
            case '(':
                extra.push(c);
                break;

            case ')':
                while (extra.top() != '(') {
                    postfix->push_back(extra.top());
                    extra.pop();
                }

                extra.pop();
                break;

            default:
                while (extra.size() > 0) {
                    char top = extra.top();

                    int p_char = PRIORITY(c);
                    int p_extra = PRIORITY(top);

                    if (p_extra >= p_char) {
                        postfix->push_back(top);
                        extra.pop();
                    } else {
                        break;
                    }
                }

                // cout << c << " ";
                extra.push(c);
                break;
        }
    }

    while (extra.size() > 0) {
        postfix->push_back(extra.top());
        extra.pop();
    }

    return postfix;
}
