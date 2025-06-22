/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "camera.hpp"
#include <Helpers.h>

HOOK(void, FASTCALL, camera_update, 0x00000001402FB0F0) {
    originalcamera_update();

    extern bool reflect_full;
    if (reflect_full) {
        extern void get_reflect_mat();
        get_reflect_mat();
    }
}

void camera_patch() {
    INSTALL_HOOK(camera_update);
}
