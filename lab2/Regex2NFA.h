//
// Created by oleglevin on 17.02.17.
//

#ifndef REGEX2NFA_REGEX2NFA_H
#define REGEX2NFA_REGEX2NFA_H


#include <string>
#include "State.h"

class Regex2NFA final {
public:
    using NFA_t = std::list<State::StatePtr_t>;
    using NFAPtr_t = std::shared_ptr<NFA_t>;

    Regex2NFA(const std::string& regex);

    std::list<NFAPtr_t> getNFA();

private:
    std::list<NFAPtr_t> mNFAList;
    int mCurId;
    std::string mRegex;

private:
    void pushOp(char c);
    void orOp();
    void starOp();
    void plusOp();

};


#endif //REGEX2NFA_REGEX2NFA_H
