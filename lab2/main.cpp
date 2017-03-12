#include <iostream>

#include "InfixTransform.h"
#include "ThompsonConstruction.h"
#include "SubsetConstruction.h"

using namespace std;

ostream& operator<< (ostream& str, const State& s) {
    for (auto& conn: s.getConnections()) {
        str << conn->mChar << ":" << conn->mState->getId() << " ";
    }
    str << " {subset:";
    for (auto& state: s.getSubset()) {
         str << state->getId() << " ";
    }
    str << "}";

    return str;
}

ostream& operator<< (ostream& str, const ThompsonConstruction::NFA_t& nfa) {
    for (auto state: nfa) {
        cout << state->getId() << "(" << state->isFinal() << ") ";
        cout << *state;
        cout << endl;
    }

    return str;
}

int main() {
    // std::string s("aa|b*b+");
    std::string s("a|b|a");
    // std::string s("b+|b*ab(bb|ab)+");

    ThompsonConstruction thompsonConstruction(s);
    SubsetConstruction subsetConstruction;

    auto nfa = thompsonConstruction.apply();
    std::cout << "-----------------\n";
    std::cout << "\nNfa: \n" << *nfa;

    auto optimizedNfa = thompsonConstruction.optimize(nfa);
    std::cout << "-----------------\n";
    std::cout << "\nOptimized nfa: \n" << *optimizedNfa;

    auto dfa = subsetConstruction.subsetConstructionEps(nfa);
    std::cout << "-----------------\n";
    std::cout << "\nDfa: \n" << *dfa;

    auto wrongDfa = subsetConstruction.subsetConstruction(optimizedNfa);
    std::cout << "-----------------\n";
    std::cout << "\nWrong dfa: \n" << *wrongDfa;

    return 0;
}