pushaq MACRO
    sub rsp, 100h
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    sub rsp, 30h
ENDM

popaq MACRO
    add rsp, 30h
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 100h
ENDM

pushaq_no_rax MACRO
    sub rsp, 100h
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    sub rsp, 30h
ENDM

popaq_no_rax MACRO
    add rsp, 30h
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    add rsp, 100h
ENDM

pushxmm MACRO
    movdqu [rsp-30h], xmm0
    movdqu [rsp-20h], xmm1
    movdqu [rsp-10h], xmm2
    movdqu [rsp], xmm3
    movdqu [rsp+10h], xmm4
    movdqu [rsp+20h], xmm5
    sub rsp, 60h
ENDM


popxmm MACRO
    add rsp, 60h
    movdqu xmm5, [rsp+20h]
    movdqu xmm4, [rsp+10h]
    movdqu xmm3, [rsp]
    movdqu xmm2, [rsp-10h]
    movdqu xmm1, [rsp+20h]
    movdqu xmm0, [rsp-30h]
ENDM

EXTERN ?auth_3d_object__pre_disp_mid_impl@@YAXAEAUmat4@@PEAUauth_3d@@PEAUrob_chara@@@Z : PROC
EXTERN ?originalauth_3d_object__pre_disp_mid@@3P6AXXZEA : QWORD

EXTERN ?auth_3d_object_hrc__pre_disp_mid_impl@@YAXPEAUauth_3d_object_hrc@@PEAUauth_3d@@PEAUrob_chara@@@Z : PROC
EXTERN ?originalauth_3d_object_hrc__pre_disp_mid@@3P6AXXZEA : QWORD

EXTERN ?auth_3d_curve_free_mid_impl@@YAX_K@Z : PROC
EXTERN ?originalauth_3d_curve_free_mid@@3P6AXXZEA : QWORD

EXTERN ?dx_default_states_get_rasterizer_state_swap@mdl@@YAPEAUp_dx_rasterizer_state@@AEAUrender_data_context@@W4dx_cull_mode@@@Z : PROC
EXTERN ?originaldraw_object_material_set_default_mid@mdl@@3P6AXXZEA : QWORD

EXTERN ?DispScenes@GltParticleManagerX@Glitter@@QEAAXAEAUrender_data_context@@AEAUcam_data@@W4DispType@2@_N@Z : PROC
EXTERN ?glt_particle_manager_x@Glitter@@3PEAVGltParticleManagerX@1@EA : QWORD

EXTERN ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesOpaque@@3P6AXXZEA : QWORD
EXTERN ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesAlpha@@3P6AXXZEA : QWORD
EXTERN ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesType2@@3P6AXXZEA : QWORD
EXTERN ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesNormal@@3P6AXXZEA : QWORD

EXTERN ?rndr__RenderManager__rndpass_pre_proc_mid_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z : PROC
EXTERN ?originalrndr__RenderManager__rndpass_pre_proc_mid@@3P6AXXZEA : QWORD

EXTERN ?rndr__RenderManager__pass_ss_sss_mid_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z : PROC
EXTERN ?originalrndr__RenderManager__pass_ss_sss_mid@@3P6AXXZEA : QWORD

EXTERN ?rndr__RenderManager__pass_3d__DispGlitterXScenesLocal_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z : PROC
EXTERN ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesLocal@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_InitScript_tail_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_InitScript_tail@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_InitStageAuth_head_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_InitStageAuth_head@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_WaitForAuthObject_mid_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_WaitForAuthObject_mid@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_WaitForAuthHandle_tail_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_WaitForAuthHandle_tail@@3P6AXXZEA : QWORD

EXTERN ?pv_game_ctrl_restart_mid_impl@@YAXXZ : PROC
EXTERN ?originalpv_game_ctrl_restart_mid@@3P6AXXZEA : QWORD

EXTERN ?pv_game_ctrl_stop_mid_impl@@YAXXZ : PROC
EXTERN ?originalpv_game_ctrl_stop_mid@@3P6AXXZEA : QWORD

.DATA
pv_game_load_InitScript_tail_jump_false QWORD 000000014025C2DDh
pv_game_load_InitStageAuth_head_jump_false QWORD 000000014025EF42h

.CODE
?implOfauth_3d_object__pre_disp_mid@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    lea rcx, [rbp-40h]
    mov rdx, rsi
    mov r8, rax
    call ?auth_3d_object__pre_disp_mid_impl@@YAXAEAUmat4@@PEAUauth_3d@@PEAUrob_chara@@@Z
    popxmm
    popaq
    jmp ?originalauth_3d_object__pre_disp_mid@@3P6AXXZEA
?implOfauth_3d_object__pre_disp_mid@@YAXXZ ENDP

?implOfauth_3d_object_hrc__pre_disp_mid@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov rcx, rdi
    mov rdx, rsi
    mov r8, rax
    call ?auth_3d_object_hrc__pre_disp_mid_impl@@YAXPEAUauth_3d_object_hrc@@PEAUauth_3d@@PEAUrob_chara@@@Z
    popxmm
    popaq
    jmp ?originalauth_3d_object_hrc__pre_disp_mid@@3P6AXXZEA
?implOfauth_3d_object_hrc__pre_disp_mid@@YAXXZ ENDP

?implOfauth_3d_curve_free_mid@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov rcx, rbx
    call ?auth_3d_curve_free_mid_impl@@YAX_K@Z
    popxmm
    popaq
    jmp ?originalauth_3d_curve_free_mid@@3P6AXXZEA
?implOfauth_3d_curve_free_mid@@YAXXZ ENDP

?implOfdraw_object_material_set_default_mid@mdl@@YAXXZ PROC PUBLIC
    pushaq_no_rax
    mov edx, ecx
    mov rcx, rsi
    call ?dx_default_states_get_rasterizer_state_swap@mdl@@YAPEAUp_dx_rasterizer_state@@AEAUrender_data_context@@W4dx_cull_mode@@@Z
    popaq_no_rax
    jmp ?originaldraw_object_material_set_default_mid@mdl@@3P6AXXZEA
?implOfdraw_object_material_set_default_mid@mdl@@YAXXZ ENDP

?implOfrndr__RenderManager__rndpass_pre_proc_mid@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov rdx, 00000001417582D0h
    call ?rndr__RenderManager__rndpass_pre_proc_mid_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__rndpass_pre_proc_mid@@3P6AXXZEA
?implOfrndr__RenderManager__rndpass_pre_proc_mid@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_ss_sss_mid@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov rdx, rdi
    mov rcx, rbx
    call ?rndr__RenderManager__pass_ss_sss_mid_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__pass_ss_sss_mid@@3P6AXXZEA
?implOfrndr__RenderManager__pass_ss_sss_mid@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesOpaque@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov byte ptr [rsp+20h], 0
    mov r9d, 0
    mov r8, r15
    mov rdx, rbx
    mov rcx, ?glt_particle_manager_x@Glitter@@3PEAVGltParticleManagerX@1@EA
    call ?DispScenes@GltParticleManagerX@Glitter@@QEAAXAEAUrender_data_context@@AEAUcam_data@@W4DispType@2@_N@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesOpaque@@3P6AXXZEA
?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesOpaque@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesAlpha@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov byte ptr [rsp+20h], 0
    mov r9d, 2
    mov r8, r15
    mov rdx, rbx
    mov rcx, ?glt_particle_manager_x@Glitter@@3PEAVGltParticleManagerX@1@EA
    call ?DispScenes@GltParticleManagerX@Glitter@@QEAAXAEAUrender_data_context@@AEAUcam_data@@W4DispType@2@_N@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesAlpha@@3P6AXXZEA
?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesAlpha@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesType2@@YAXXZ PROC PUBLIC
    test rdi, rdi
    jz end_func
    pushaq
    pushxmm
    mov byte ptr [rsp+20h], 0
    mov r9d, 3
    mov r8, r15
    mov rdx, rbx
    mov rcx, ?glt_particle_manager_x@Glitter@@3PEAVGltParticleManagerX@1@EA
    call ?DispScenes@GltParticleManagerX@Glitter@@QEAAXAEAUrender_data_context@@AEAUcam_data@@W4DispType@2@_N@Z
    popxmm
    popaq
end_func:
    jmp ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesType2@@3P6AXXZEA
?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesType2@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesNormal@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    mov byte ptr [rsp+20h], 0
    mov r9d, 1
    mov r8, r15
    mov rdx, rbx
    mov rcx, ?glt_particle_manager_x@Glitter@@3PEAVGltParticleManagerX@1@EA
    call ?DispScenes@GltParticleManagerX@Glitter@@QEAAXAEAUrender_data_context@@AEAUcam_data@@W4DispType@2@_N@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesNormal@@3P6AXXZEA
?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesNormal@@YAXXZ ENDP

?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesLocal@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    lea rdx, [r15-1220h]
    mov rcx, rbx
    call ?rndr__RenderManager__pass_3d__DispGlitterXScenesLocal_impl@@YAXAEAUrender_data_context@@AEAURenderManager@rndr@@@Z
    popxmm
    popaq
    jmp ?originalrndr__RenderManager__pass_3d__DispGlitterXScenesLocal@@3P6AXXZEA
?implOfrndr__RenderManager__pass_3d__DispGlitterXScenesLocal@@YAXXZ ENDP

?implOfpv_game_load_InitScript_tail@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_load_InitScript_tail_impl@@YA_NXZ
    test al, al
    jz end_func
    popaq
    jmp pv_game_load_InitScript_tail_jump_false
end_func:
    popaq
    jmp ?originalpv_game_load_InitScript_tail@@3P6AXXZEA
?implOfpv_game_load_InitScript_tail@@YAXXZ ENDP

?implOfpv_game_load_InitStageAuth_head@@YAXXZ PROC PUBLIC
    pushaq
    pushxmm
    call ?pv_game_load_InitStageAuth_head_impl@@YA_NXZ
    test al, al
    jz end_func
    popxmm
    popaq
    jmp pv_game_load_InitStageAuth_head_jump_false
end_func:
    popxmm
    popaq
    jmp ?originalpv_game_load_InitStageAuth_head@@3P6AXXZEA
?implOfpv_game_load_InitStageAuth_head@@YAXXZ ENDP

?implOfpv_game_load_WaitForAuthObject_mid@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_load_WaitForAuthObject_mid_impl@@YA_NXZ
    test al, al
    jz end_func
    popaq
    or dl, 1
    jmp ?originalpv_game_load_WaitForAuthObject_mid@@3P6AXXZEA
end_func:
    popaq
    jmp ?originalpv_game_load_WaitForAuthObject_mid@@3P6AXXZEA
?implOfpv_game_load_WaitForAuthObject_mid@@YAXXZ ENDP

?implOfpv_game_load_WaitForAuthHandle_tail@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_load_WaitForAuthHandle_tail_impl@@YA_NXZ
    test al, al
    jz end_func
    popaq
    or bl, 1
    jmp ?originalpv_game_load_WaitForAuthHandle_tail@@3P6AXXZEA
end_func:
    popaq
    jmp ?originalpv_game_load_WaitForAuthHandle_tail@@3P6AXXZEA
?implOfpv_game_load_WaitForAuthHandle_tail@@YAXXZ ENDP

?implOfpv_game_ctrl_restart_mid@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_ctrl_restart_mid_impl@@YAXXZ
    popaq
    jmp ?originalpv_game_ctrl_restart_mid@@3P6AXXZEA
?implOfpv_game_ctrl_restart_mid@@YAXXZ ENDP

?implOfpv_game_ctrl_stop_mid@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_ctrl_stop_mid_impl@@YAXXZ
    popaq
    jmp ?originalpv_game_ctrl_stop_mid@@3P6AXXZEA
?implOfpv_game_ctrl_stop_mid@@YAXXZ ENDP
END