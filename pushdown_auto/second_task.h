//
// Created by oleglevin on 12.03.17.
//

#ifndef PUSHDOWN_AUTO_SECOND_TASK_H_H
#define PUSHDOWN_AUTO_SECOND_TASK_H_H

#include <iostream>
#include <stack>

#include <cassert>
#include <list>

inline char invert(char c) {
    return c == '1' ? '0' : '1';
}

enum stack_sym_t {
    INC
};

void run_second_task() {
    std::string str("11110");
    auto cur_pos = str.rbegin();
    std::stack <stack_sym_t> stack_mem;
    bool finished = false;

    // Using std::list for fast push_front
    std::list<char> result_str;

    stack_mem.push(INC);
    while (!finished) {
        if (stack_mem.empty()) {
            result_str.push_front(invert(*cur_pos));
        } else if (stack_mem.top() == INC) {
            switch (*cur_pos) {
                case '0':
                    result_str.push_front(invert('1'));
                    stack_mem.pop();
                    break;

                case '1':
                    result_str.push_front(invert('0'));
                    break;

                    // This situation is normal only when we got overflow, otherwise assert
                default:
                    if (cur_pos == str.rend()) {
                        result_str.push_front(invert('1'));
                        stack_mem.pop();
                        break;
                    } else {
                        assert(false);
                    }
            }
        } else {
            assert(false);
        }

        if (cur_pos != str.rend()) cur_pos++;

        if (stack_mem.empty() && cur_pos == str.rend()) {
            finished = true;
        }
    }

    for (const auto &c: result_str) {
        std::cout << c;
    }
}

#endif //PUSHDOWN_AUTO_SECOND_TASK_H_H
