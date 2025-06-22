/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/mat.hpp"

extern void (FASTCALL* matrix_stack_data_push_matrix)();
extern void (FASTCALL* matrix_stack_data_pop_matrix)();
extern void (FASTCALL* matrix_stack_data_load_matrix)(const mat4& mat);
extern void (FASTCALL* matrix_stack_data_get_matrix)(__int64 a1, mat4& mat);
