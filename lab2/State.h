//
// Created by oleglevin on 17.02.17.
//

#ifndef REGEX2NFA_STATE_H
#define REGEX2NFA_STATE_H

#include <memory>
#include <list>
#include <set>
#include <iostream>

class State final {
public:
    using StatePtr_t = std::shared_ptr<State>;

    struct ConnectionWrapper {
        ConnectionWrapper(const StatePtr_t& s, char c) :
                mState(s), mChar(c) { }

        bool operator== (const ConnectionWrapper& rhs) {
            if (/*(this->mState == rhs.mState) && */(this->mChar == rhs.mChar)) {
                return true;
            }
            return false;
        }

        StatePtr_t mState;
        char mChar;
    };
    using ConnPtr_t = std::shared_ptr<ConnectionWrapper>;

public:
    State(int id);

    State(int id, const std::set<State::StatePtr_t>&);

    void connect(const StatePtr_t&, char c);

    std::list<ConnPtr_t> getConnections() const { return mConnList; }

    std::set<StatePtr_t> getSubset() const { return mSubset; }

    void deleteConnection(const ConnPtr_t& conn) {
        /*std::cout << "len1=" << mConnList.size() << " ";*/
//        std::cout << "\n";
//        for (auto c: mConnList) {
//            std::cout << c->mChar << " ";
//        }
//        std::cout << "\nDeleting: " << conn->mChar << "\n";
        mConnList.remove(conn);
//        for (auto c: mConnList) {
//            std::cout << c->mChar << " ";
//        }
        /* std::cout << "len2=" << mConnList.size() << " "; */
    }

    int getId() const { return mId; }

    void setFinal() { mFinal = true; }

    bool isFinal() const { return mFinal; }

private:
    int mId;
    std::list<ConnPtr_t> mConnList;
    bool mFinal;

    std::set<StatePtr_t> mSubset;
};


#endif //REGEX2NFA_STATE_H
