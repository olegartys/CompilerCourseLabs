#ifndef __INFIX_PARSE_H__
#define __INFIX_PARSE_H__

#include <string>
#include <stack>
#include <memory>

#define PRIORITY(a) ((a == '(') ? 1 : ((a == '|') ? 2 : ((a == '.') ? 3 : (a == '*' || a == '+') ? 4 : 5)))

/**
 * Infix to Postfix parser.
 */
class InfixTransform final {
public:
    static std::shared_ptr<std::string> toPostfix(const std::string &infixString);

private:
    static std::shared_ptr<std::string> addExtraChar(const std::string &);

};

#endif
