EXTERN ?originalspr__SprArgs__put_sprite_line_list_mid@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_scale_mid@@3P6AXXZEA : QWORD
EXTERN ?originalspr__calc_sprite_texture_param_mid1@@3P6AXXZEA : QWORD
EXTERN ?originalspr__calc_sprite_texture_param_mid2@@3P6AXXZEA : QWORD
EXTERN ?originalspr__sub_1405B9550_mid1@@3P6AXXZEA : QWORD
EXTERN ?originalspr__sub_1405B9550_mid2@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_fill_vertex_array_mid@@3P6AXXZEA : QWORD

EXTERN ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA : QWORD

.CODE
?implOfspr__SprArgs__put_sprite_line_list_mid@@YAXXZ PROC PUBLIC
    lea rcx, [rsp+20h]
    push rax
    push rdx

    mov rax, [rcx+0F8h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rcx, [rcx+rdx*8]

    pop rdx
    pop rax
    jmp ?originalspr__SprArgs__put_sprite_line_list_mid@@3P6AXXZEA
?implOfspr__SprArgs__put_sprite_line_list_mid@@YAXXZ ENDP

?implOfspr__draw_sprite_scale_mid@@YAXXZ PROC PUBLIC
    push rax
    push rdx

    mov rax, [rbx+0F8h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rcx, [rcx+rdx*8]

    pop rdx
    pop rax
    jmp ?originalspr__draw_sprite_scale_mid@@3P6AXXZEA
?implOfspr__draw_sprite_scale_mid@@YAXXZ ENDP

?implOfspr__calc_sprite_texture_param_mid1@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rax, [rbx+0F8h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rax, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__calc_sprite_texture_param_mid1@@3P6AXXZEA
?implOfspr__calc_sprite_texture_param_mid1@@YAXXZ ENDP

?implOfspr__calc_sprite_texture_param_mid2@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rax, [rbx+0F8h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rax, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__calc_sprite_texture_param_mid2@@3P6AXXZEA
?implOfspr__calc_sprite_texture_param_mid2@@YAXXZ ENDP

?implOfspr__sub_1405B9550_mid1@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rax, [r13+0F8h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rax, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__sub_1405B9550_mid1@@3P6AXXZEA
?implOfspr__sub_1405B9550_mid1@@YAXXZ ENDP

?implOfspr__sub_1405B9550_mid2@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov r11, [r13+0F8h]
    lea rdx, [r11+r11*2]
    mov r11, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [r11+8]
    lea r11, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__sub_1405B9550_mid2@@3P6AXXZEA
?implOfspr__sub_1405B9550_mid2@@YAXXZ ENDP

?implOfspr__draw_sprite_fill_vertex_array_mid@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rbx, [r8+0F8h]
    lea rdx, [rbx+rbx*2]
    mov rbx, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rbx+8]
    lea rbx, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_fill_vertex_array_mid@@3P6AXXZEA
?implOfspr__draw_sprite_fill_vertex_array_mid@@YAXXZ ENDP
END