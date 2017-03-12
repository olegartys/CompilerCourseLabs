//
// Created by oleglevin on 12.03.17.
//

#ifndef PUSHDOWN_AUTO_FIRST_TASK_H
#define PUSHDOWN_AUTO_FIRST_TASK_H

#include <iostream>
#include <stack>
#include <sstream>

#include <cassert>

enum state_t {
    S1, S2, S3, S4
};

enum stack_sym_t {
    Z
};

void run_first_task() {
    std::string str("1010");
    size_t str_len = str.length();
    std::stack<stack_sym_t> stack_mem;

    bool accepted = true;

    size_t cur_pos = 0;
    state_t cur_state = S1;
    char cur_sym;
    while (cur_pos != str_len && accepted) {
        cur_sym = str[cur_pos];

        switch (cur_state) {
            case S1:
                if (stack_mem.empty()) {
                    switch (cur_sym) {
                        case '0':
                            accepted = false;
                            break;

                        case '1':
                            stack_mem.push(Z);
                            cur_pos++;
                            break;

                        default:
                            assert(false);
                    }
                } else if (stack_mem.top() == Z) {
                    switch (cur_sym) {
                        case '0':
                            cur_state = S2;
                            break;

                        case '1':
                            stack_mem.push(Z);
                            cur_pos++;
                            break;

                        default:
                            assert(false);
                    }
                } else {
                    assert(true);
                }
                break; // S1

            case S2:
                if (stack_mem.empty()) {
                    switch (cur_sym) {
                        case '0':
                            accepted = false;
                            break;

                        case '1':
                            // accepted = false; // FIXME
                            cur_state = S3;
                            break;

                        default:
                            assert(false);
                    }
                } else if (stack_mem.top() == Z) {
                    switch (cur_sym) {
                        case '0':
                            stack_mem.pop();
                            cur_pos++;
                            break;

                        case '1':
                            accepted = false;
                            break;

                        default:
                            assert(false);
                    }
                } else {
                    assert(true);
                }
                break; // S2

                // S3-S4 are almost copies of S1-S2
            case S3:
                if (stack_mem.empty()) {
                    switch (cur_sym) {
                        case '0':
                            accepted = false;
                            break;

                        case '1':
                            stack_mem.push(Z);
                            cur_pos++;
                            break;

                        default:
                            assert(false);
                    }
                } else if (stack_mem.top() == Z) {
                    switch (cur_sym) {
                        case '0':
                            cur_state = S4;
                            break;

                        case '1':
                            stack_mem.push(Z);
                            cur_pos++;
                            break;

                        default:
                            assert(false);
                    }
                } else {
                    assert(true);
                }
                break; // S3

            case S4:
                if (stack_mem.empty()) {
                    switch (cur_sym) {
                        case '0':
                            accepted = false;
                            break;

                        case '1':
                            accepted = false; // FIXME
                            // cur_state = S3;
                            break;

                        default:
                            assert(false);
                    }
                } else if (stack_mem.top() == Z) {
                    switch (cur_sym) {
                        case '0':
                            stack_mem.pop();
                            cur_pos++;
                            break;

                        case '1':
                            accepted = false;
                            break;

                        default:
                            assert(false);
                    }
                } else {
                    assert(true);
                }
                break; // S4
        }
    }

    if (accepted && stack_mem.empty() && cur_state != S2) {
        std::cout << "Match" << std::endl;
    } else {
        std::cout << "Does not match" << std::endl;
    }
}

#endif //PUSHDOWN_AUTO_FIRST_TASK_H
