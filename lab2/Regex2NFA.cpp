//
// Created by oleglevin on 17.02.17.
//

#include "Regex2NFA.h"

Regex2NFA::Regex2NFA(const std::string& regex) :
        mRegex(regex), mCurId(0)
{  }

void Regex2NFA::pushOp(char c) {
    State::StatePtr_t state0(new State(++mCurId));
    State::StatePtr_t state1(new State(++mCurId));

    state0->connect(state1, c);

    NFAPtr_t nfa(new NFA_t());

    nfa->push_back(state0);
    nfa->push_back(state1);

    mNFAList.push_back(nfa);
}

std::list<NFAPtr_t> Regex2NFA::getNFA() {
    for (auto& c: mRegex) {
        switch (c) {
            case 'a':
        }
        if (c == 'a' || c == 'b' || c == 'e') {
            pushOp(c);
        } else if
    }
}

for (unsigned int i=0; i<__lang->size(); i++) {
char c = __lang->at(i);

if (c == 'a' || c == 'b' || c == 'e') {
push_op(c);
} else if (c == '|') {
if (!or_op()) return NULL;
} else if (c == '*') {
if (!star_op()) return NULL;
} else if (c == '.') {
if (!concat_op()) return NULL;
} else {
return NULL;
}
}

deque<state *> *nfa = __nfa_comp->back();
if (nfa == NULL) return NULL;
__nfa_comp->pop_back();

nfa->back()->set_final();

return nfa;