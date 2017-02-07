//
// Created by oleglevin on 02.02.17.
//

#ifndef FSM_FIRSTTASK_H
#define FSM_FIRSTTASK_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "FSMatrix.h"
#include "ISignal.h"
#include "IState.h"

extern std::vector<std::string> split(const std::string &s, char delim);

struct FirstTaskState : public IState {
public:

    enum {
        STATE_INVALID = -1, STATE_A, STATE_B, STATE_C, STATE_D
    };

    FirstTaskState() :
            mState(STATE_INVALID), mIsFinish(false) { }

    FirstTaskState(char value, bool isFinish = false) :
            mIsFinish(isFinish) {
        switch (value) {
            case 'A':
                mState = STATE_A;
                break;

            case 'B':
                mState = STATE_B;
                break;

            case 'C':
                mState = STATE_C;
                break;

            case 'D':
                mState = STATE_D;
                break;

            default:
                mState = STATE_INVALID;
                break;
        }
    }

    ~FirstTaskState() = default;

    bool operator< (const IState& rhs) const override { return this->mState < dynamic_cast<const FirstTaskState*>(&rhs)->mState; }
    bool operator== (const IState& rhs) const override { return this->mState == dynamic_cast<const FirstTaskState*>(&rhs)->mState; }

    operator std::string() const override {
        switch (mState) {
            case STATE_A:
                return "A";
            case STATE_B:
                return "B";
            case STATE_C:
                return "C";
            case STATE_D:
                return "D";
            default:
                return "-1";
        }
    }

    bool isFinish() const override {
        return mIsFinish;
    }

    int mState;
    bool mIsFinish;

};

struct FirstTaskSignal : public ISignal {
public:

    enum {
        SIGNAL_INVALID = -1, SIGNAL_0, SIGNAL_1
    };

    FirstTaskSignal() :
            mSignal(SIGNAL_INVALID) { }

    FirstTaskSignal(char value) {
        switch (value) {
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

    ~FirstTaskSignal() = default;

    bool operator< (const ISignal& rhs) const override { return this->mSignal < dynamic_cast<const FirstTaskSignal*>(&rhs)->mSignal; }
    bool operator== (const ISignal& rhs) const override { return this->mSignal == dynamic_cast<const FirstTaskSignal*>(&rhs)->mSignal; }
    operator std::string() const override { return std::to_string(mSignal); }

    int mSignal;

};

FSMatrix<FirstTaskState, FirstTaskSignal> parseInputStream(std::istream& stream) {
    FSMatrix<FirstTaskState, FirstTaskSignal> fsm;
    std::string line;
    std::string stateName;
    std::vector<std::string> signalTokenList;
    std::vector<FirstTaskSignal> signalList;
    std::vector<std::string> lineTokenList;
    bool isFinish;

    // Initialize signal list
    std::getline(stream, line);
    signalTokenList = std::move(split(line, ' '));

    for (const auto& it: signalTokenList) {
        signalList.push_back(FirstTaskSignal(it[0]));
    }

    // Initialize states and insert values in FSM
    while (!stream.eof()) {
        std::getline(stream, line);
        lineTokenList = std::move(split(line, ' '));

        stateName = lineTokenList[0];
        isFinish = *(lineTokenList.end() - 1) == "1";
        FirstTaskState s(stateName[0], isFinish); // assuming that FirstTaskState name in only one symbol

        int i = 0;
        for (auto it = lineTokenList.begin() + 1; it != lineTokenList.end() - 1; ++it) {
            FirstTaskState nextState((*it)[0]);
            fsm.add(s, signalList[i++], nextState, nullptr);
        }
    }

    return fsm;
}

std::string genRightProduction(const FirstTaskState &state, const FSMatrix<FirstTaskState, FirstTaskSignal> &fsm) {
    std::stringstream productionStr;
    std::set<FirstTaskSignal> signalSet = fsm.getSignalSet();
    size_t setLen = signalSet.size();
    size_t i = 1;

    productionStr << std::string(state) + "::=";

    // Get the signal set
    for (const auto& it: signalSet) {
        productionStr << std::string(it);
        productionStr << std::string(fsm.getNextState(state, it));
        if (i++ < setLen)
            productionStr << "|";
    }

    if (state.isFinish()) {
        productionStr << "|e";
    }

    return productionStr.str();
}

std::string genLeftProduction(const FirstTaskState &state, const FSMatrix<FirstTaskState, FirstTaskSignal> &fsm) {
    std::stringstream productionStr;
    std::set<FirstTaskSignal> signalSet = fsm.getSignalSet();
    std::set<FirstTaskState> stateSet = fsm.getStateSet();
    bool isEmptyProduction = true;

    productionStr << std::string(state) << "::=";

    for (const auto& it: stateSet) {
        for (const auto& sig: signalSet) {
            auto nextState = fsm.getNextState(it, sig);
            if (nextState == state) {
                isEmptyProduction = false;
                productionStr << std::string(it) << std::string(sig) << "|";
            }
        }
    }

    if (*(productionStr.str().end() - 1) == '|') {
        std::string s = productionStr.str();
        *(s.end() - 1) = '\0';
        productionStr.str(std::move(s));
    }

    return isEmptyProduction ? "" : productionStr.str();
}

void runFirstTask() {
    std::ifstream fin("/home/oleglevin/ClionProjects/FSM/input");
    auto fsm = parseInputStream(fin);
    std::set<FirstTaskState> stateSet = fsm.getStateSet();

    fsm.dump();

    std::cout << "\nRight grammar:\n";
    for (const auto& it: stateSet) {
        std::cout << genRightProduction(it, fsm) << std::endl;
    }

    std::cout << "\nLeft grammar:\n";
    for (const auto& it: stateSet) {
        std::cout << genLeftProduction(it, fsm) << std::endl;
    }
}

#endif //FSM_FIRSTTASK_H
