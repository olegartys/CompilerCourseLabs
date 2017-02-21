//
// Created by oleglevin on 17.02.17.
//

#ifndef REGEX2NFA_STATE_H
#define REGEX2NFA_STATE_H

#include <memory>
#include <list>

class State final {
public:
    using StatePtr_t = std::shared_ptr<State>;

    struct ConnectionWrapper {
        ConnectionWrapper(const StatePtr_t& s, char c) :
                mState(s), mChar(c) { }

        StatePtr_t mState;
        char mChar;
    };
    using ConnPtr_t = std::shared_ptr<ConnectionWrapper>;

public:
    State(int id);

    void connect(const StatePtr_t&, char c);

    std::list<ConnPtr_t> getConnections() const { return mConnList; }

    int getId() const { return mId; }

    void setFinal() { mFinal = true; }

    bool isFinal() const { return mFinal; }

private:
    int mId;
    std::list<ConnPtr_t> mConnList;
    bool mFinal;

};


#endif //REGEX2NFA_STATE_H
