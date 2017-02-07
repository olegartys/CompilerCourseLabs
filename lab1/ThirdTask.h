//
// Created by oleglevin on 05.02.17.
//

#ifndef FSM_THIRDTASK_H
#define FSM_THIRDTASK_H

#include <string>
#include <sstream>
#include <regex>

#include "IState.h"
#include "ISignal.h"
#include "FSMatrix.h"

extern std::vector<std::string> split(const std::string &s, char delim);

/**
 * Describes set of states for Basic const definition parsing
 */
struct ThirdTaskState : public IState {

    enum {
        STATE_INVALID = - 1, STATE_S, STATE_A, STATE_B, STATE_C, STATE_D, STATE_E, STATE_F, STATE_Z
    };

    ThirdTaskState(int state = STATE_INVALID) :
            mState(state) { }

    virtual ~ThirdTaskState() override = default;

    virtual bool operator<(const IState &rhs) const override {
        return this->mState < dynamic_cast<const ThirdTaskState*>(&rhs)->mState;
    }

    virtual bool operator==(const IState &rhs) const override {
        return this->mState == dynamic_cast<const ThirdTaskState*>(&rhs)->mState;
    }

    virtual operator std::string() const override {
        switch (mState) {
            case STATE_INVALID:
                return "-1";
            case STATE_S:
                return "S";
            case STATE_A:
                return "A";
            case STATE_B:
                return "B";
            case STATE_C:
                return "C";
            case STATE_D:
                return "D";
            case STATE_E:
                return "E";
            case STATE_F:
                return "F";
            case STATE_Z:
                return "Z";
        }
        return std::to_string(mState);
    }

    virtual bool isFinish() const override {
        return mState == STATE_Z;
    }

    int mState;
};

/**
 * Describes set of signals for Basic const definition parsing
 */
struct ThirdTaskSignal : public ISignal {

    enum {
        SIGNAL_INV = -1, SIGNAL_CONST, SIGNAL_EMPTY, SIGNAL_SPACE, SIGNAL_VARNAME, SIGNAL_EQUAL, SIGNAL_AS,
        SIGNAL_DOUBLE, SIGNAL_VALUE
    };

    ThirdTaskSignal(const std::string& signal = "[__null__]") {
        if (!strcasecmp(signal.c_str(), "const")) {
            mSignal.first = SIGNAL_CONST;
        } else if (signal == " ") {
            mSignal.first = SIGNAL_SPACE;
        } else if (signal == "=") {
            mSignal.first = SIGNAL_EQUAL;
        } else if (!strcasecmp(signal.c_str(), "as")) {
            mSignal.first = SIGNAL_AS;
        } else if (!strcasecmp(signal.c_str(), "double")) {
            mSignal.first = SIGNAL_DOUBLE;
        } else if (signal == "") {
            mSignal.first = SIGNAL_EMPTY;
        } else if (signal == "[__null__]") {
            mSignal.first = SIGNAL_INV;
        } else {
            mSignal.first = SIGNAL_VARNAME | SIGNAL_VALUE; // Assume that signal is non-therminal. TODO it seems not very correct
        }

        mSignal.second = signal;
    }

    virtual ~ThirdTaskSignal() override = default;

    virtual bool operator<(const ISignal &rhs) const override {
        return this->mSignal.first < dynamic_cast<const ThirdTaskSignal*>(&rhs)->mSignal.first;
    }

    virtual bool operator==(const ISignal &rhs) const override {
        return this->mSignal.first == dynamic_cast<const ThirdTaskSignal*>(&rhs)->mSignal.first;
    }

    virtual operator std::string() const override {
        return std::to_string(mSignal.first) + ":" + mSignal.second;
    }

    std::pair<int, std::string> mSignal;
};

/**
 * Class for validate double expressions. [0-9]*\\.?[0-9]+
 */
class DoubleValValidator {
public:
    enum {
        STATE_S, STATE_E, STATE_A, STATE_B, STATE_Z
    };

    DoubleValValidator(const std::string& val) :
            mValue(val) { }

    bool validate() const {
        bool isValid = true;

        int state = STATE_S;
        for (const auto& c: mValue) {
            if (isValid) {
                int isDigit = isdigit(c);
                switch (state) {
                    case STATE_S:
                        if (isDigit) {
                            state = STATE_A;
                        } else {
                            state = STATE_E;
                        }
                        break;
                    case STATE_A:
                        if (isDigit) {
                            state = STATE_A;
                        } else if (c == '.') {
                            state = STATE_B;
                        } else {
                            state = STATE_E;
                        }
                        break;
                    case STATE_B:
                        if (isDigit) {
                            state = STATE_Z;
                        } else {
                            state = STATE_E;
                        }
                        break;
                    case STATE_Z:
                        if (isDigit) {
                            state = STATE_Z;
                        } else {
                            state = STATE_E;
                        }
                        break;
                    case STATE_E:
                        isValid = false;
                        break;
                    default:
                        // Never access this point
                        Log::error(TAG, "Never get there!");
                        break;
                }
            } else {
                break;
            }

        }

        return isValid && (state == STATE_Z || state == STATE_A);
    }

protected:
    std::string mValue;

};

/**
 * Struct to be passed to FSM callbacks.
 */
struct CheckerArgWrapper {
    CheckerArgWrapper() :
        mErrCode(1), mString("") { }

    int mErrCode;
    std::string mString;
};

/**
 * Check variable name for correctness
 */
auto __var_checker = [](void* arg) {
    Log::debug(TAG, "Var name found. Checking...");

    CheckerArgWrapper* _arg = (CheckerArgWrapper*)arg;

    if (!isalpha(_arg->mString[0])) {
        goto err_exit;
    } else {
        for (auto c = _arg->mString.begin() + 1; c != _arg->mString.end(); ++c) {
            if (!isalnum(*c)) {
                goto err_exit;
            }
        }

        _arg->mErrCode = 1;
        Log::debug(TAG, "Variable name " + _arg->mString + " is valid");
    }

    return;

err_exit:
    _arg->mErrCode = -1;
    Log::error(TAG, "Variable name " + _arg->mString + " is NOT valid");
};

/**
 * Check value for correctness
 */
auto __val_checker = [](void* arg) {
    Log::debug(TAG, "Value found. Checking...");

    CheckerArgWrapper* _arg = (CheckerArgWrapper*)arg;
    DoubleValValidator validator(_arg->mString);

    if (validator.validate()) {
        _arg->mErrCode = 1;
        Log::debug(TAG, "Value " + _arg->mString + " is valid");
    } else {
        _arg->mErrCode = -1;
        Log::debug(TAG, "Value " + _arg->mString + " is NOT valid");
    }
};

void runThirdTask() {
    std::stringstream str("   Const dsa    as Double = 01.0");

    FSMatrix<ThirdTaskState, ThirdTaskSignal> fsm;

    ThirdTaskState stateS(ThirdTaskState::STATE_S);
    ThirdTaskState stateA(ThirdTaskState::STATE_A);
    ThirdTaskState stateB(ThirdTaskState::STATE_B);
    ThirdTaskState stateC(ThirdTaskState::STATE_C);
    ThirdTaskState stateD(ThirdTaskState::STATE_D);
    ThirdTaskState stateE(ThirdTaskState::STATE_E);
    ThirdTaskState stateF(ThirdTaskState::STATE_F);
    ThirdTaskState stateZ(ThirdTaskState::STATE_Z);

    ThirdTaskSignal signalConst("const");
    ThirdTaskSignal signalSpace(" ");
    ThirdTaskSignal signalEmpty("");
    ThirdTaskSignal signalVarName("varStub");
    ThirdTaskSignal signalEqual("=");
    ThirdTaskSignal signalAs("as");
    ThirdTaskSignal signalDouble("double");
    ThirdTaskSignal signalValue("valStub");

    fsm.add(stateS, signalEmpty, stateS, [](void*) { Log::debug(TAG, "Still in S..."); });
    fsm.add(stateS, signalConst, stateA, [](void* arg) { Log::debug(TAG, "Const found..."); });
    fsm.add(stateA, signalEmpty, stateA, [](void*) { Log::debug(TAG, "Still in A..."); });
    fsm.add(stateA, signalVarName, stateB, __var_checker);
    fsm.add(stateB, signalEmpty, stateB, [](void*) { Log::debug(TAG, "Still in B..."); });
    fsm.add(stateB, signalEqual, stateC, [](void*) { Log::debug(TAG, " = found..."); });
    fsm.add(stateB, signalAs, stateE, [](void*) { Log::debug(TAG, "As found..."); });
    fsm.add(stateE, signalEmpty, stateE, [](void*) { Log::debug(TAG, "Still in E..."); });
    fsm.add(stateE, signalDouble, stateF, [](void*) { Log::debug(TAG, "Double found..."); });
    fsm.add(stateF, signalEmpty, stateF, [](void*) { Log::debug(TAG, "Still in F..."); });
    fsm.add(stateF, signalEqual, stateC, [](void*) { Log::debug(TAG, " = found..."); });
    fsm.add(stateC, signalEmpty, stateC, [](void*) { Log::debug(TAG, "Still in C..."); });
    fsm.add(stateC, signalValue, stateZ, __val_checker);

    fsm.dump();

    std::cout << str.str() << std::endl;

    while(!str.eof()) {
        std::string line;
        std::getline(str, line);

        std::vector<std::string> tokenVec = split(line, ' ');
        ThirdTaskState iterState(ThirdTaskState::STATE_S);
        for (const auto& tok: tokenVec) {
            ThirdTaskSignal sig(tok);
            ThirdTaskState nextState(fsm.getNextState(iterState, sig));

            if (nextState.mState == ThirdTaskState::STATE_INVALID) {
                Log::error(TAG, "Unexpected symbol: " + tok);
                break;
            } else {
                CheckerArgWrapper arg;
                arg.mString = tok;
                fsm.callback(iterState, sig, &arg);
                if (arg.mErrCode < 0) {
                    Log::error(TAG, "Error occured");
                    break;
                } else {
                    iterState = nextState;
                }
            }
        }
    }

    std::cout.flush();
}

#endif //FSM_THIRDTASK_H
