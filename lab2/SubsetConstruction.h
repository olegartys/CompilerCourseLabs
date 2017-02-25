//
// Created by olegartys on 25.02.17.
//

#ifndef REGEX2NFA_SUBSETCONSTRUCTION_H
#define REGEX2NFA_SUBSETCONSTRUCTION_H

#include "State.h"
#include "ThompsonConstruction.h"

class SubsetConstruction final {
public:

    SubsetConstruction() :
            mCurId(0) { }

    ThompsonConstruction::DFAPtr_t subsetConstruction(const ThompsonConstruction::NFAPtr_t &nfa);
    ThompsonConstruction::DFAPtr_t subsetConstructionEps(const ThompsonConstruction::NFAPtr_t &nfa);

private:
    void epsilonSteps(const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out);
    void step(char c, const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out);

    int mCurId;
};


#endif //REGEX2NFA_SUBSETCONSTRUCTION_H
