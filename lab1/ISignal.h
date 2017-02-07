//
// Created by oleglevin on 30.01.17.
//

#ifndef FSM_ISIGNAL_H
#define FSM_ISIGNAL_H

class ISignal {
public:
    virtual ~ISignal() = default;

    virtual bool operator< (const ISignal&) const = 0;
    virtual bool operator== (const ISignal&) const = 0;

    virtual operator std::string() const = 0;
};

#endif //FSM_ISIGNAL_H
