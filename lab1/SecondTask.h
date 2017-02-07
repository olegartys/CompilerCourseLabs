//
// Created by oleglevin on 02.02.17.
//

#ifndef FSM_SECONDTASK_H
#define FSM_SECONDTASK_H

#include <string>
#include <sstream>

#include "Log.h"
#include "IState.h"
#include "ISignal.h"
#include "FSMatrix.h"

struct SecondTaskState : public IState {

    enum {
        STATE_INVALID = -1, STATE_BEG, STATE_L1, STATE_E0 /* even zero */, STATE_NE0 /* not event zero */, STATE_ERR //STATE_BEGIN, STATE_S1, STATE_S2
    };

    SecondTaskState(int state = STATE_INVALID) :
            mState(state) { }

    virtual ~SecondTaskState() override = default;

    virtual bool operator<(const IState& rhs) const override {
        return this->mState < dynamic_cast<const SecondTaskState*>(&rhs)->mState;
    }

    virtual bool operator==(const IState& rhs) const override {
        return this->mState == dynamic_cast<const SecondTaskState*>(&rhs)->mState;
    }

    virtual operator std::string() const override {
        return std::to_string(mState);
    }

    virtual bool isFinish() const override {
        return false;
    }

    int mState;
};

struct SecondTaskSignal : public ISignal {

    enum {
        SIGNAL_INVALID = -1, SIGNAL_0, SIGNAL_1
    };

    SecondTaskSignal(char signal) {
        switch (signal) {
            case '0':
                mSignal = SIGNAL_0;
                break;

            case '1':
                mSignal = SIGNAL_1;
                break;

            default:
                mSignal = SIGNAL_INVALID;
                break;
        }
    }

    virtual ~SecondTaskSignal() override = default;

    virtual bool operator<(const ISignal& rhs) const override {
        return this->mSignal < dynamic_cast<const SecondTaskSignal*>(&rhs)->mSignal;
    }

    virtual bool operator==(const ISignal& rhs) const override {
        return this->mSignal == dynamic_cast<const SecondTaskSignal*>(&rhs)->mSignal;
    }

    virtual operator std::string() const override {
        return std::to_string(mSignal);
    }

    int mSignal;
};

static bool errParse = false;

static const std::string TAG = "SecondTask";

void runSecondTask() {
    std::stringstream str("00100100111000");
    FSMatrix<SecondTaskState, SecondTaskSignal> fsm;

    SecondTaskState stateBeg(SecondTaskState::STATE_BEG);
    SecondTaskState stateL1(SecondTaskState::STATE_L1);
    SecondTaskState stateE0(SecondTaskState::STATE_E0);
    SecondTaskState stateNE0(SecondTaskState::STATE_NE0);
    SecondTaskState stateErr(SecondTaskState::STATE_ERR);

    SecondTaskSignal signal0('0');
    SecondTaskSignal signal1('1');

    auto __even_zero = [](void*) { Log::debug(TAG, "Even zero count"); };
    auto __neven_zero = [](void*) { Log::debug(TAG, "Not even zero count"); };

    fsm.add(stateBeg, signal0, stateBeg, [](void*) { Log::debug(TAG, "Keep searching for 1..."); });
    fsm.add(stateBeg, signal1, stateL1, [](void*) { Log::debug(TAG, "First 1 found"); });
    fsm.add(stateL1, signal0, stateNE0, __neven_zero);
    fsm.add(stateL1, signal1, stateL1, [](void*) { Log::debug(TAG, "Still on 1...");} );
    fsm.add(stateNE0, signal0, stateE0, __even_zero);
    fsm.add(stateNE0, signal1, stateErr, [](void*) { Log::debug(TAG, "ERROR"); /* errParse = true; */ });
    fsm.add(stateE0, signal0, stateNE0, __neven_zero);
    fsm.add(stateE0, signal1, stateL1, [](void*) { Log::debug(TAG, "Second 1 found"); });

    fsm.dump();
    std::cout << str.str() << std::endl;

    SecondTaskState iterState(SecondTaskState::STATE_BEG);
    while(!str.eof()) {
        std::string line;
        std::getline(str, line);
        for (const auto &c: line) {
            //if (!errParse) {
                SecondTaskSignal signal(c);
                SecondTaskState nextState(fsm.getNextState(iterState, signal));

                if (nextState == SecondTaskState(SecondTaskState::STATE_ERR)) {
                    Log::error(TAG, "Parsing error");
                    break;
                }

                fsm.callback(iterState, signal); // Trigger callback for the current state and incoming signal
                iterState = fsm.getNextState(iterState, signal); // Change the state
//            } else {
//                exit(EXIT_FAILURE);
            //}
        }
    }

}

#endif //FSM_SECONDTASK_H
