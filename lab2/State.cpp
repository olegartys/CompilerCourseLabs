//
// Created by oleglevin on 17.02.17.
//

#include "State.h"

State::State(int id) :
        mId(id) {

}

void State::connect(const StatePtr_t& s, char c) {
    mConnList.push_back(ConnPtr_t(new ConnectionWrapper(s, c)));
}