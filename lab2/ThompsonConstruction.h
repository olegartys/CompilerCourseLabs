//
// Created by oleglevin on 17.02.17.
//

#ifndef REGEX2NFA_REGEX2NFA_H
#define REGEX2NFA_REGEX2NFA_H


#include <string>
#include <deque>
#include <set>
#include <iostream>

#include "State.h"

extern std::ostream& operator<< (std::ostream& str, const State& s);

class ThompsonConstruction final {
public:
    using NFA_t = std::deque<State::StatePtr_t>;
    using NFAPtr_t = std::shared_ptr<NFA_t>;

    using DFA_t = NFA_t;
    using DFAPtr_t = NFAPtr_t;

    ThompsonConstruction(const std::string& regex);

    NFAPtr_t apply();
    NFAPtr_t optimize(const NFAPtr_t&);

    DFAPtr_t powerset(const NFAPtr_t&);

private:
    std::deque<NFAPtr_t> mNFAList;
    int mCurId;
    std::string mRegex;

private:
    void pushOp(char c);
    bool orOp();
    bool starOp();
    bool plusOp();
    bool concatOp();

    void epsilon_steps(const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out);
    void step(char c, const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out);

};


#endif //REGEX2NFA_REGEX2NFA_H
