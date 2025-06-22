/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    Counter& counter = *(Counter*)0x000000014114857C;

    void Counter::Increment() {
        value++;
    }

    int32_t Counter::GetValue() {
        return value;
    }

    void Counter::Reset() {
        value = 0;
    }
}