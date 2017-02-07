//
// Created by oleglevin on 30.01.17.
//

#ifndef FSM_FSMATRIX_H
#define FSM_FSMATRIX_H

#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <utility>

#include "IState.h"
#include "ISignal.h"

template <typename TState, typename TSignal,
          typename std::enable_if<std::is_base_of<IState, TState>::value, bool>::type = 0,
          typename std::enable_if<std::is_base_of<ISignal, TSignal>::value, bool>::type = 0>
class FSMatrix final {
public:
    using TCallback = std::function<void(void*)>;

    FSMatrix() = default;

    void add(TState state, TSignal signal, TState nextState, std::function<void(void*)> cb) {
        mStateSignalMatrix[std::make_pair(state, signal)] =
                std::make_pair(nextState, cb);
        mSignalSet.insert(signal);
        mStateSet.insert(state);
    }

    TState getNextState(const TState& state, const TSignal& signal) const {
        //std::cout << "getNextState: " << "state=" << std::string(state) << " signal=" << std::string(signal) << std::endl;
        for (const auto& it: mStateSignalMatrix) {
            //std::cout << "state=" << std::string(it.first.first) << " signal=" << std::string(it.first.second) << std::endl;
            if ((it.first.first == state) && (it.first.second == signal)) {
                //std::cout << "HERE" << std::endl;
                return it.second.first;
            }
        }

        return TState();
    }

    void callback(TState state, TSignal signal, void* data = nullptr) const {
        for (const auto& it: mStateSignalMatrix) {
            if ((it.first.first == state) && (it.first.second == signal)) {
                it.second.second(data);
            }
        }
    }

    std::set<TState> getStateSet() const { return mStateSet; }

    std::set<TSignal> getSignalSet() const { return mSignalSet; }

    void dump() const {
        // Print row of all the signals
        for (const auto& it: mSignalSet) std::cout << "\t" << "\"" << std::string(it) << "\"" << " ";
        std::cout << std::endl;

        // For each row of state for each signal print new state
        for (const auto& state: mStateSet) {
            std::cout << std::string(state) << " ";
            for (const auto& sig: mSignalSet) {
                std::cout << " " << "\t" << std::string(getNextState(state, sig));
            }
            std::cout << "\t" << state.isFinish();
            std::cout << std::endl;
        }
    }

    typename std::map<std::pair<TState, TSignal>, std::pair<TState, TCallback>>::iterator begin() {
        return mStateSignalMatrix.begin();
    }

    typename std::map<std::pair<TState, TSignal>, std::pair<TState, TCallback>>::iterator end() {
        return mStateSignalMatrix.end();
    }

private:
    std::map<std::pair<TState, TSignal>, std::pair<TState, TCallback>> mStateSignalMatrix;
    std::set<TState> mStateSet;
    std::set<TSignal> mSignalSet;

};

#endif //FSM_FSMATRIX_H
