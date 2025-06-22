/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "app.hpp"

namespace app {
    int32_t get_value_scaled(int32_t value, float_t scale) {
        if (scale < 1.0f)
            return max_def((int32_t)((float_t)value * scale), 4);
        return value;
    }
}
