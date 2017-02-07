//
// Created by oleglevin on 30.01.17.
//

#ifndef FSM_ISTATE_H_H
#define FSM_ISTATE_H_H

class IState {
public:
    virtual ~IState() = default;

    virtual bool operator< (const IState&) const = 0;
    virtual bool operator== (const IState&) const = 0;

    virtual operator std::string() const = 0;

    virtual bool isFinish() const = 0;
};

#endif //FSM_ISTATE_H_H
