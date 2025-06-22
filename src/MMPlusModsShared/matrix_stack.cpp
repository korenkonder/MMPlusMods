/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "matrix_stack.hpp"

void (FASTCALL* matrix_stack_data_push_matrix)()
    = (void (FASTCALL*)())0x00000001404CE280;
void (FASTCALL* matrix_stack_data_pop_matrix)()
    = (void (FASTCALL*)())0x00000001404CE2E0;
void (FASTCALL* matrix_stack_data_load_matrix)(const mat4& mat)
    = (void (FASTCALL*)(const mat4 & mat))0x00000001404CE330;
void (FASTCALL* matrix_stack_data_get_matrix)(__int64 a1, mat4& mat)
    = (void (FASTCALL*)(__int64 a1, mat4 & mat))0x00000001404CE5C0;
