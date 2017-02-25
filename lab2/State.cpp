//
// Created by oleglevin on 17.02.17.
//

#include "State.h"

State::State(int id) :
        mId(id), mFinal(false) {

}

State::State(int id, const std::set<State::StatePtr_t> &subset) : State(id) {
    for (const auto state: subset) {
        if (state->isFinal()) {
            mFinal = true;
            break;
        }
    }

    mSubset = subset;
}

void State::connect(const StatePtr_t& s, char c) {
    bool isExist = false;

    // Guarantee connections to be unique
    for (const auto& conn: mConnList) {
        if ((conn->mState == s) && (conn->mChar == c)) {
            isExist = true;
        }
    }

    if (!isExist) {
        mConnList.push_back(ConnPtr_t(new ConnectionWrapper(s, c)));
    }
}