//
// Created by olegartys on 25.02.17.
//

#include <stack>
#include "SubsetConstruction.h"

using namespace std;

void SubsetConstruction::epsilonSteps(const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out) {
    std::stack<State::StatePtr_t> toProcess;

    out.clear();
    out = in;

    for (const auto& state: in) {
        toProcess.push(state);
    }

    while (!toProcess.empty()) {
        auto state = toProcess.top();
        toProcess.pop();

        auto connections = state->getConnections();

        // std::cout << "stack_size=" << toProcess.size() << "\nid=" << state->getId() << "\n";

        for (const auto& conn: connections) {
            // std::cout << conn->mChar << " ";
            if (conn->mChar == 'e') {
                if (out.find(conn->mState) == out.end()) {
                    out.insert(conn->mState);
                    toProcess.push(conn->mState);
                }
            }
        }
    }
}

void SubsetConstruction::step(char c, const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out) {
    out.clear();

    for (const auto& state: in) {
        auto connections = state->getConnections();

        for (const auto& conn: connections) {
            if (conn->mChar == c) {
                out.insert(conn->mState);
            }
        }
    }
}

ThompsonConstruction::DFAPtr_t SubsetConstruction::subsetConstructionEps(const ThompsonConstruction::NFAPtr_t &nfa) {
    ThompsonConstruction::DFAPtr_t dfa(new ThompsonConstruction::DFA_t);

    std::stack<State::StatePtr_t> toProcess;
    std::set<State::StatePtr_t> dfaStartSet;
    std::set<State::StatePtr_t> nfaStartSet;

    mCurId = 0;

    // Form the initial state
    nfaStartSet.insert(nfa->at(0));

    epsilonSteps(nfaStartSet, dfaStartSet);

    State::StatePtr_t dfaStart(new State(mCurId, dfaStartSet));
    dfa->push_back(dfaStart);
    toProcess.push(dfaStart);

    // Process alll the states
    while (!toProcess.empty()) {
        State::StatePtr_t state = toProcess.top();
        toProcess.pop();

        // For the terminal alphabet
        for (int i = 0; i < 2; ++i) {
            char c = i == 0 ? 'a' : 'b';

            std::set<State::StatePtr_t> stepSet, epsStepSet;
            step(c, state->getSubset(), stepSet);
            epsilonSteps(stepSet, epsStepSet);

            bool enclosed = false;
            State::StatePtr_t enclosure;

            for (auto j = 0; j < dfa->size(); ++j) {
                enclosure = dfa->at(j);

                if (enclosure->getSubset() == epsStepSet) {
                    enclosed = true;
                    break;
                }
            }

            if (!epsStepSet.empty()) {
                if (enclosed) {
                    state->connect(enclosure, c);
                } else {
                    State::StatePtr_t join(new State(++mCurId, epsStepSet));
                    toProcess.push(join);
                    dfa->push_back(join);

                    state->connect(join, c);
                }
            }
        }
    }

    return dfa;
}

ThompsonConstruction::DFAPtr_t SubsetConstruction::subsetConstruction(const ThompsonConstruction::NFAPtr_t &nfa) {
    ThompsonConstruction::DFAPtr_t dfa(new ThompsonConstruction::DFA_t);

    *dfa = *nfa;

    // Filling the DFA
    dfa->clear();
    mCurId = 0;
    std::stack<State::StatePtr_t> processStack;
    std::set<int> processedStates;

    std::set<State::StatePtr_t> subsetStateA;
    std::set<State::StatePtr_t> subsetStateB;

    // For initial state of NFA create subsets of transitions for all alphabet symbols.
    // FIXME: several initial states
    auto initNFAstate = nfa->at(0);
    for (const auto& conn: initNFAstate->getConnections()) {
        subsetStateA.insert(conn->mState);
    }

    State::StatePtr_t initDFAstate(new State(++mCurId, subsetStateA));
    dfa->push_back(initDFAstate);
    processStack.push(initDFAstate);

    while (!processStack.empty()) {
        auto curState = processStack.top();
        processStack.pop();

        for (int i = 0; i < 2; ++i) {
            char c = i == 0 ? 'a' : 'b';

            subsetStateA.clear();

            // Fill the subsetConstruction with transitions
            for (const auto& subsState: curState->getSubset()) {
                for (const auto& connSubs: subsState->getConnections()) {
                    if (connSubs->mChar == c) {
                        subsetStateA.insert(connSubs->mState);
                    }
                }
            }

            // Check whether it exists in dfa
            bool isExist = false;
            auto dfaState = *dfa->begin();
            for (auto j = 0; j < dfa->size(); ++j) {
                dfaState = dfa->at(j);

                if (subsetStateA == dfaState->getSubset()) {
                    isExist = true;
                    break;
                }
            }

            if (!subsetStateA.empty()) {
                if (isExist) {
                    curState->connect(dfaState, c);
                } else {
                    State::StatePtr_t newState(new State(++mCurId, subsetStateA));

                    processStack.push(newState);
                    dfa->push_back(newState);

                    curState->connect(newState, c);
                }
            }
        }
    }

//    State::StatePtr_t stateA(new State(++mCurId, subsetStateA));
//    State::StatePtr_t stateB(new State(++mCurId, subsetStateB));
//
//    if (/* stateA->isFinal() || */!stateA->getSubset().empty()) { initDFAstate->connect(stateA, 'a'); processStack.push(stateA); }
//    if (/*stateB->isFinal() || */!stateB->getSubset().empty()) { initDFAstate->connect(stateB, 'b'); processStack.push(stateB); }
//
//    if (stateA->getSubset().empty() && stateB->getSubset().empty()) {
//        if (stateA->isFinal() || stateB->isFinal()) {
//            dfa->push_back(initDFAstate);
//            // processedStates.insert(nfa->at(1)->getId());
//        }
//    } else {
//        dfa->push_back(initDFAstate);
//        // processedStates.insert(nfa->at(1)->getId());
//    }
//
//    while (!processStack.empty()) {
//        auto curState = processStack.top();
//        processStack.pop();
//
//        std::cout << curState->getId() << " " << *curState << "\n";
//        sleep(1);
//        std::cout.flush();
//
//        subsetStateA.clear();
//        subsetStateB.clear();
//
//        for (const auto& subState: curState->getSubset()) {
//            for (const auto& connSubState: subState->getConnections()) {
//                // std::cout << "subId=" << subState->getId() << " ";
//                if (connSubState->mChar == 'a') {
//                    // if (processedStates.find(subState->getId()) != processedStates.end())
//                        subsetStateA.insert(connSubState->mState);
//                } else if (connSubState->mChar == 'b') {
//                    // if (processedStates.find(subState->getId()) != processedStates.end())
//                        subsetStateB.insert(connSubState->mState);
//                } else {
//                    std::cerr << "Undefined symbol\n";
//                }
//            }
//        }
//
//        State::StatePtr_t stateA(new State(++mCurId, subsetStateA));
//        State::StatePtr_t stateB(new State(++mCurId, subsetStateB));
//
//        if (!stateA->getSubset().empty()) { curState->connect(stateA, 'a'); processStack.push(stateA); }
//        if (!stateB->getSubset().empty()) { curState->connect(stateB, 'b'); processStack.push(stateB); }
//
//        bool isExist = false;
//        if (stateA->getSubset().empty() && stateB->getSubset().empty()) {
//            if (stateA->isFinal() || stateB->isFinal()) {
////                for (const auto& s: *dfa) {
////                    if (s->getId() == curState->getId()) {
////                        isExist = true;
////                        break;
////                    }
////                }
////                if (isExist) break;
//                dfa->push_back(curState);
//            }
//        } else {
////            for (const auto& s: *dfa) {
////                if (s->getId() == curState->getId()) {
////                    isExist = true;
////                    break;
////                }
////            }
////            if (isExist) break;
//            dfa->push_back(curState);
//        }
//
//    }

    return dfa;
}
