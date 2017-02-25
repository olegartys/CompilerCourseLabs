//
// Created by oleglevin on 17.02.17.
//

#include <iostream>

#include "ThompsonConstruction.h"
#include "InfixTransform.h"

using namespace std;

ThompsonConstruction::ThompsonConstruction(const std::string& regex) :
        mRegex(regex), mCurId(0)
{ }

void ThompsonConstruction::pushOp(char c) {
    State::StatePtr_t state0(new State(++mCurId));
    State::StatePtr_t state1(new State(++mCurId));

    state0->connect(state1, c);

    NFAPtr_t nfa(new NFA_t());

    nfa->push_back(state0);
    nfa->push_back(state1);

    mNFAList.push_back(nfa);
}

bool ThompsonConstruction::concatOp() {
    NFAPtr_t nfa1, nfa2;

    nfa2 = mNFAList.back();
    if (!nfa2) {
        return false;
    }
    mNFAList.pop_back();

    nfa1 = mNFAList.back();
    if (!nfa1) {
        return false;
    }
    mNFAList.pop_back();

    State::StatePtr_t qB0 = nfa2->front();
    State::StatePtr_t qAn = nfa1->back();

    // std::cout << "idB0=" << qB0->getId() << " qAnid=" << qAn->getId() << "\n";

    qAn->connect(qB0, 'e');

    for (auto i = 0; i < nfa2->size(); ++i) {
        nfa1->push_back(nfa2->at(i));
    }

    mNFAList.push_back(nfa1);

    return true;
}

bool ThompsonConstruction::orOp() {
    NFAPtr_t nfa1, nfa2;

    nfa1 = mNFAList.back();
    if (!nfa1) {
        return false;
    }
    mNFAList.pop_back();

    nfa2 = mNFAList.back();
    if (!nfa2) {
        return false;
    }
    mNFAList.pop_back();

    State::StatePtr_t q0(new State(++mCurId));
    State::StatePtr_t q1(new State(++mCurId));
    State::StatePtr_t qA0 = nfa1->front();
    State::StatePtr_t qB0 = nfa2->front();
    State::StatePtr_t qAn = nfa1->back();
    State::StatePtr_t qBn = nfa2->back();

    q0->connect(qA0, 'e');
    q0->connect(qB0, 'e');
    qAn->connect(q1, 'e');
    qBn->connect(q1, 'e');

    nfa1->push_front(q0);
    nfa2->push_back(q1);

    for (auto i = 0; i < nfa2->size(); ++i) {
        nfa1->push_back(nfa2->at(i));
    }

    mNFAList.push_back(nfa1);

    return true;
}

bool ThompsonConstruction::starOp() {
//    NFAPtr_t nfa;
//
//    nfa = mNFAList.back();
//    if (!nfa) {
//        return false;
//    }
//    mNFAList.pop_back();
//
//    State::StatePtr_t qA0 = nfa->front();
//    State::StatePtr_t qAn = nfa->back();
//
////    std::cout << "idA0=" << qA0->getId() << " qAnid=" << qAn->getId() << "\n";
//
//    qA0->connect(qAn, 'e');
//    qAn->connect(qA0, 'e');
//
//    mNFAList.push_back(nfa);

    NFAPtr_t nfaA, nfaB;

    nfaA = mNFAList.back();
    if (!nfaA) {
        return false;
    }
    mNFAList.pop_back();

    State::StatePtr_t qN0(new State(++mCurId));
    State::StatePtr_t qNN(new State(++mCurId));

    State::StatePtr_t qA0 = nfaA->front();
    State::StatePtr_t qAn = nfaA->back();

    qAn->connect(qA0, 'e');

    qN0->connect(qA0, 'e');
    qN0->connect(qNN, 'e');
    qAn->connect(qNN, 'e');

    nfaA->push_front(qN0);
    nfaA->push_back(qNN);

    mNFAList.push_back(nfaA);

    return true;
}

bool ThompsonConstruction::plusOp() {
    NFAPtr_t nfa;

    nfa = mNFAList.back();
    if (!nfa) {
        return false;
    }
    mNFAList.pop_back();

    State::StatePtr_t qN0(new State(++mCurId));
    State::StatePtr_t qNN(new State(++mCurId));

    State::StatePtr_t qA0 = nfa->front();
    State::StatePtr_t qAn = nfa->back();

    qN0->connect(qA0, 'e');
    qAn->connect(qNN, 'e');
    qAn->connect(qA0, 'e');

    nfa->push_front(qN0);
    nfa->push_back(qNN);

    mNFAList.push_back(nfa);

    return true;
}

ThompsonConstruction::NFAPtr_t ThompsonConstruction::apply() {
    mRegex = *InfixTransform::toPostfix(mRegex);

    std::cout << "Postfix regex: " << mRegex << std::endl;

    for (auto& c: mRegex) {
        switch (c) {
            case 'a':
            case 'b':
            case 'e':
                pushOp(c);
                break;

            case '|':
                if (!orOp()) {
                    return nullptr;
                }
                break;

            case '*':
                if (!starOp()) {
                    return nullptr;
                }
                break;

            case '+':
                if (!plusOp()) {
                    return nullptr;
                }
                break;

            case '.':
                if (!concatOp()) {
                    return nullptr;
                }
                break;

            default:
                return nullptr;
        }
    }

    NFAPtr_t nfa = mNFAList.back();
    if (!nfa) {
        return nullptr;
    }

    mNFAList.pop_back();
    nfa->back()->setFinal();

    return nfa;
}

ThompsonConstruction::NFAPtr_t ThompsonConstruction::optimize(const ThompsonConstruction::NFAPtr_t& nfa) {
    ThompsonConstruction::NFAPtr_t optimizedNfa(new ThompsonConstruction::NFA_t);

    *optimizedNfa = *nfa;

    // Need to make 2 iterations because of dependency between e-transition states (e.x. ba*)
    for (int i = 0; i < 2; ++i) {
        // Delete e
        for (const auto& state: *optimizedNfa) {
            // Check whether there are E-transitions from this state
            for (const auto& conn: state->getConnections()) {
                if (conn->mChar == 'e') {
                    // If it is e-transition to the final state - make this state final
                    if (conn->mState->isFinal()) {
                        state->setFinal();
                    }

                    // Delete e-transition
                    if (i == 1) state->deleteConnection(conn);

                    // Go to this state and copy all the transitions from it to the current state
                    // FIXME: need copy e-transitions or not?
                    auto eTransitionState = conn->mState;
                    for (const auto& eTransStateConn: eTransitionState->getConnections()) {
                        if (eTransStateConn->mChar != 'e') {
                            state->connect(eTransStateConn->mState, eTransStateConn->mChar);
                        }
                    }
                }
            }
        }
    }

    // Delete unreachable states (algorithm 3 lecture, 22 slide)
    std::set<State::StatePtr_t> stateSet;
    stateSet.insert(optimizedNfa->front());

    bool f = true;
    while(f) {
        auto size0 = stateSet.size();
        for (auto s: stateSet) {
            for (auto& conn: s->getConnections()) {
                stateSet.insert(conn->mState);
            }
        }
        auto size1 = stateSet.size();

        if (size1 == size0) f = false;
    }

    optimizedNfa->clear();

    for (auto& s: stateSet) {
        optimizedNfa->push_back(s);
    }

    return optimizedNfa;
}

//void ThompsonConstruction::epsilon_steps(const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out) {
//    // std::shared_ptr<std::deque<State::ConnectionWrapper>> connections;
//    std::stack<State::StatePtr_t> toProcess;
//
//    out.clear();
//    out = in;
//
//    for (const auto& state: in) {
//        toProcess.push(state);
//    }
//
//    while (!toProcess.empty()) {
//        auto state = toProcess.top();
//        toProcess.pop();
//
//        auto connections = state->getConnections();
//
//        std::cout << "stack_size=" << toProcess.size() << "\nid=" << state->getId() << "\n";
//
//        for (const auto& conn: connections) {
//            std::cout << conn->mChar << " ";
//            if (conn->mChar != 'e') continue;
//
//            if (out.find(conn->mState) == out.end()) {
//                out.insert(conn->mState);
//                toProcess.push(conn->mState);
//            }
//        }
//    }
//
//    for (const auto& state: in) {
//        // Get connections for the current state
//        auto connections = state->getConnections();
//
//
//    }
//}
//
//void ThompsonConstruction::step(char c, const std::set<State::StatePtr_t>& in, std::set<State::StatePtr_t>& out) {
//    out.clear();
//
//    for (const auto& state: in) {
//        auto connections = state->getConnections();
//
//        for (const auto& conn: connections) {
//            if (conn->mChar == c) {
//                out.insert(conn->mState);
//            }
//        }
//    }
//}

//ThompsonConstruction::DFAPtr_t ThompsonConstruction::powerset(const NFAPtr_t &nfa) {
//    DFAPtr_t dfa(new DFA_t);
//
//    *dfa = *nfa;
//
//    // Need to make 2 iterations because of dependency between e-transition states (e.x. ba*)
//    for (int i = 0; i < 2; ++i) {
//        // Delete e
//        for (const auto& state: *dfa) {
//            // Check whether there are E-transitions from this state
//            for (const auto& conn: state->getConnections()) {
//                if (conn->mChar == 'e') {
//                    // If it is e-transition to the final state - make this state final
//                    if (conn->mState->isFinal()) {
//                        state->setFinal();
//                    }
//
//                    // Delete e-transition
//                    if (i == 1) state->deleteConnection(conn);
//
//                    // Go to this state and copy all the transitions from it to the current state
//                    // FIXME: need copy e-transitions or not?
//                    auto eTransitionState = conn->mState;
//                    for (const auto& eTransStateConn: eTransitionState->getConnections()) {
//                        if (eTransStateConn->mChar != 'e') {
//                            state->connect(eTransStateConn->mState, eTransStateConn->mChar);
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    // Delete unreachable states (algorithm 3 lecture, 22 slide)
//    std::set<State::StatePtr_t> stateSet;
//    stateSet.insert(dfa->front());
//
//    bool f = true;
//    while(f) {
//        auto size0 = stateSet.size();
//        for (auto s: stateSet) {
//            for (auto& conn: s->getConnections()) {
//                stateSet.insert(conn->mState);
//            }
//        }
//        auto size1 = stateSet.size();
//
//        if (size1 == size0) f = false;
//    }
//
//    NFAPtr_t optimizedNFA(new NFA_t());
//    for (auto& s: stateSet) {
//        optimizedNFA->push_back(s);
//    }
//
//    std::cout << "\nOptimized NFA: \n";
//    for (auto state: *optimizedNFA) {
//        cout << state->getId() << "(" << state->isFinal() << ") ";
//        cout << *state;
//        cout << endl;
//    }
//
//    // Filling the DFA
//    dfa->clear();
//    mCurId = 0;
//    std::stack<State::StatePtr_t> processStack;
//    std::set<int> processedStates;
//
//    std::set<State::StatePtr_t> subsetStateA;
//    std::set<State::StatePtr_t> subsetStateB;
//
//    // For initial state of NFA create subsets of transitions for all alphabet symbols.
//    auto initNFAstate = optimizedNFA->at(0);
//    for (const auto& conn: initNFAstate->getConnections()) {
//        subsetStateA.insert(conn->mState);
//    }
//
//    State::StatePtr_t initDFAstate(new State(++mCurId, subsetStateA));
//    dfa->push_back(initDFAstate);
//    processStack.push(initDFAstate);
//
//    while (!processStack.empty()) {
//        auto curState = processStack.top();
//        processStack.pop();
//
//        subsetStateA.clear();
//
//        for (int i = 0; i < 2; ++i) {
//            char c = i == 0 ? 'a' : 'b';
//
//            // Fill the subsetConstruction with transitions
//            for (const auto& subsState: curState->getSubset()) {
//                for (const auto& connSubs: subsState->getConnections()) {
//                    if (connSubs->mChar == c) {
//                        subsetStateA.insert(connSubs->mState);
//                    }
//                }
//            }
//
//            // Check whether it exists in dfa
//            bool isExist = false;
//            auto dfaState = *dfa->begin();
//            for (auto j = 0; j < dfa->size(); ++j) {
//                dfaState = dfa->at(j);
//
//                if (subsetStateA == dfaState->getSubset()) {
//                    isExist = true;
//                    break;
//                }
//            }
//
//            if (isExist) {
//                curState->connect(dfaState, c);
//            } else {
//                std::cout << "here\n";
//                State::StatePtr_t newState(new State(++mCurId, subsetStateA));
//
//                processStack.push(newState);
//                dfa->push_back(newState);
//
//                curState->connect(newState, c);
//            }
//        }
//    }






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

//    return dfa;
//}

//ThompsonConstruction::DFAPtr_t ThompsonConstruction::powerset(const NFAPtr_t& nfa) {
//    DFAPtr_t dfa(new DFA_t);
//    std::stack<State::StatePtr_t> toProcess;
//    std::set<State::StatePtr_t> dfaStartSet;
//    std::set<State::StatePtr_t> nfaStartSet;
//
//    mCurId = 0;
//
//    nfaStartSet.insert(nfa->at(0));
//
//    epsilon_steps(nfaStartSet, dfaStartSet);
//    for (auto state: *nfa) {
//        cout << state->getId() << " ";
//        cout << *state;
//        cout << endl;
//    }
//    cout << "DFA=" << endl;
//    for (auto state: dfaStartSet) {
//        cout << state->getId() << " ";
//        cout << *state;
//        cout << endl;
//    }
//
//    State::StatePtr_t dfaStart(new State(mCurId, dfaStartSet));
//    dfa->push_back(dfaStart);
//    toProcess.push(dfaStart);
//
//    while (!toProcess.empty()) {
//        State::StatePtr_t state = toProcess.top();
//        toProcess.pop();
//
//        // For the terminal alphabet
//        for (int i = 0; i < 2; ++i) {
//            char c = i == 0 ? 'a' : 'b';
//
//            set<State::StatePtr_t> stepSet, epsStepSet;
//            step(c, state->getSubset(), stepSet);
//            epsilon_steps(stepSet, epsStepSet);
//
//            bool enclosed = false;
//            State::StatePtr_t enclosure;
//
//            for (auto j = 0; j < dfa->size(); ++j) {
//                enclosure = dfa->at(j);
//
//                if (enclosure->getSubset() == epsStepSet) {
//                    enclosed = true;
//                    break;
//                }
//            }
//
//            if (!epsStepSet.empty()) {
//                if (enclosed) {
//                    state->connect(enclosure, c);
//                } else {
//                    State::StatePtr_t join(new State(++mCurId, epsStepSet));
//                    toProcess.push(join);
//                    dfa->push_back(join);
//
//                    state->connect(join, c);
//                }
//            }
//        }
//    }
//
//    return dfa;
//}