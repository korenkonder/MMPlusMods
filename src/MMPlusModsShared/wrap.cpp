/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "wrap.hpp"
#include "../KKdLib/vec.hpp"
#include "types.hpp"
#include <unordered_map>
#include <vector>
#include <Helpers.h>

ID3D11Device*& d3d_device = *(ID3D11Device**)0x0000000141148230;
ID3D11DeviceContext*& d3d_device_context = *(ID3D11DeviceContext**)0x0000000141148238;

p_dx_blend_state* (*dx_default_states_get_blend_state)(bool blend_enable,
    dx_blend src_blend, dx_blend dst_blend,
    dx_blend src_blend_alpha, dx_blend dst_blend_alpha, dx_blend_write_mask write_mask)
    = (p_dx_blend_state * (*)(bool blend_enable,
        dx_blend src_blend, dx_blend dst_blend,
        dx_blend src_blend_alpha, dx_blend dst_blend_alpha, dx_blend_write_mask write_mask))0x00000001404CECC0;
p_dx_depth_stencil_state* (*dx_default_states_get_depth_stencil_state)(
    dx_depth_test depth_test, dx_depth_func depth_func)
    = (p_dx_depth_stencil_state * (*)(
        dx_depth_test depth_test, dx_depth_func depth_func))0x00000001404CED20;
p_dx_rasterizer_state* (*dx_default_states_get_rasterizer_state)(dx_cull_mode cull_mode)
    = (p_dx_rasterizer_state * (*)(dx_cull_mode cull_mode))0x00000001404CED80;
p_dx_sampler_state* (*dx_default_states_get_sampler_state)(
    dx_texture_address address_u, dx_texture_address address_v, bool anisotropic)
    = (p_dx_sampler_state * (*)(
        dx_texture_address address_u, dx_texture_address address_v, bool anisotropic))0x00000001404CED60;
p_dx_texture* (*dx_default_states_get_texture)(int32_t index)
    = (p_dx_texture * (*)(int32_t index))0x00000001404CED50;

static void (FASTCALL* dx_state__update)(dx_state* This)
    = (void (FASTCALL*)(dx_state * This))0x00000001402BF300;

struct dx_state_stack {
    int32_t depth;
    std::vector<dx_state> stack;

    inline dx_state_stack() {
        depth = 0;
    }

    inline ~dx_state_stack() {

    }
};

std::unordered_map<p_dx_state*, dx_state_stack> p_dx_state_stack;

void p_dx_blend_state::create(bool blend_enable, dx_blend src_blend, dx_blend dst_blend,
    dx_blend src_blend_alpha, dx_blend dst_blend_alpha, D3D11_COLOR_WRITE_ENABLE write_mask) {
    static void (*p_dx_blend_state__create)(p_dx_blend_state * This, bool blend_enable, dx_blend src_blend, dx_blend dst_blend, dx_blend src_blend_alpha, dx_blend dst_blend_alpha, D3D11_COLOR_WRITE_ENABLE write_mask)
        = (void (*)(p_dx_blend_state * This, bool blend_enable, dx_blend src_blend, dx_blend dst_blend, dx_blend src_blend_alpha, dx_blend dst_blend_alpha, D3D11_COLOR_WRITE_ENABLE write_mask))0x00000001402BE4B0;
    p_dx_blend_state__create(this, blend_enable, src_blend, dst_blend, src_blend_alpha, dst_blend_alpha, write_mask);
}

void p_dx_blend_state::release() {
    static void (*p_dx_blend_state__release)(p_dx_blend_state * This)
        = (void (*)(p_dx_blend_state * This))0x00000001402BE540;
    p_dx_blend_state__release(this);
}

p_dx_blend_state& p_dx_blend_state::operator=(const p_dx_blend_state& other) {
    static p_dx_blend_state* (FASTCALL * p_dx_blend_state__move)(p_dx_blend_state * dst, const p_dx_blend_state * src)
        = (p_dx_blend_state * (FASTCALL*)(p_dx_blend_state * dst, const p_dx_blend_state * src))0x000000015229CEA0;
    return *p_dx_blend_state__move(this, &other);
}

void p_dx_buffer::create(dx_buffer_type type, int32_t size) {
    static void (*p_dx_buffer__create)(p_dx_buffer * This, dx_buffer_type type, int32_t size)
        = (void (*)(p_dx_buffer * This, dx_buffer_type type, int32_t size))0x00000001402BD0B0;
    p_dx_buffer__create(this, type, size);
}

void p_dx_buffer::create_immutable(dx_buffer_type type, const void* data, int32_t size) {
    static void (*p_dx_buffer__create_immutable)(p_dx_buffer * This, dx_buffer_type type, const void* data, int32_t size)
        = (void (*)(p_dx_buffer * This, dx_buffer_type type, const void* data, int32_t size))0x00000001402BD120;
    p_dx_buffer__create_immutable(this, type, data, size);
}

void p_dx_buffer::release() {
    static void (*p_dx_buffer__release)(p_dx_buffer * This)
        = (void (*)(p_dx_buffer * This))0x00000001402BD290;
    p_dx_buffer__release(this);
}

p_dx_buffer& p_dx_buffer::operator=(const p_dx_buffer& other) {
    static p_dx_buffer* (FASTCALL * p_dx_buffer__move)(p_dx_buffer * dst, const p_dx_buffer * src)
        = (p_dx_buffer * (FASTCALL*)(p_dx_buffer * dst, const p_dx_buffer * src))0x00000001521F7010;
    return *p_dx_buffer__move(this, &other);
}

void p_dx_depth_stencil_state::create(dx_depth_test depth_test, dx_depth_func depth_func) {
    release();

    dx_depth_stencil_state* ptr = (dx_depth_stencil_state*)_operator_new(sizeof(dx_depth_stencil_state));
    ptr->uses = 0;
    ptr->free_next = 0;
    ptr->depth_stencil_state = 0;

    const std::pair<bool, bool> depth_enable_write_mask_array[] = {
        { false, false },
        { false,  true },
        {  true,  true },
    };

    const D3D11_COMPARISON_FUNC depth_func_array[] = {
        D3D11_COMPARISON_LESS_EQUAL,
        D3D11_COMPARISON_GREATER_EQUAL,
        D3D11_COMPARISON_ALWAYS,
    };

    D3D11_DEPTH_STENCIL_DESC desc;
    desc.DepthEnable = depth_enable_write_mask_array[depth_test].first ? TRUE : FALSE;
    desc.DepthWriteMask = depth_enable_write_mask_array[depth_test].second
        ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc = depth_func_array[depth_func];
    desc.StencilEnable = FALSE;
    desc.StencilReadMask = 0xFF;
    desc.StencilWriteMask = 0xFF;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    desc.BackFace = desc.FrontFace;
    d3d_device->CreateDepthStencilState(&desc, &ptr->depth_stencil_state);
    this->ptr = ptr;

    ptr->uses++;
}

void p_dx_depth_stencil_state::release() {
    static void (*p_dx_depth_stencil_state__release)(p_dx_depth_stencil_state * This)
        = (void (*)(p_dx_depth_stencil_state * This))0x00000001402BE180;
    p_dx_depth_stencil_state__release(this);
}

p_dx_depth_stencil_state& p_dx_depth_stencil_state::operator=(const p_dx_depth_stencil_state& other) {
    static p_dx_depth_stencil_state* (FASTCALL * p_dx_depth_stencil_state__move)(p_dx_depth_stencil_state * dst, const p_dx_depth_stencil_state * src)
        = (p_dx_depth_stencil_state * (FASTCALL*)(p_dx_depth_stencil_state * dst, const p_dx_depth_stencil_state * src))0x0000000152287700;
    return *p_dx_depth_stencil_state__move(this, &other);
}

void p_dx_pixel_shader::create(const void* data, int32_t size) {
    static void (*p_dx_pixel_shader__create)(p_dx_pixel_shader * This, const void* data, int32_t size)
        = (void (*)(p_dx_pixel_shader * This, const void* data, int32_t size))0x00000001402BDC90;
    p_dx_pixel_shader__create(this, data, size);
}

void p_dx_pixel_shader::release() {
    static void (*p_dx_pixel_shader__release)(p_dx_pixel_shader * This)
        = (void (*)(p_dx_pixel_shader * This))0x00000001402BDCF0;
    p_dx_pixel_shader__release(this);
}

p_dx_pixel_shader& p_dx_pixel_shader::operator=(const p_dx_pixel_shader& other) {
    static p_dx_pixel_shader* (FASTCALL * p_dx_pixel_shader__move)(p_dx_pixel_shader * dst, const p_dx_pixel_shader * src)
        = (p_dx_pixel_shader * (FASTCALL*)(p_dx_pixel_shader * dst, const p_dx_pixel_shader * src))0x00000001402BDC40;
    return *p_dx_pixel_shader__move(this, &other);
}

void p_dx_rasterizer::release() {
    static void (*p_dx_rasterizer__release)(p_dx_rasterizer * This)
        = (void (*)(p_dx_rasterizer * This))0x00000001402BDFF0;
    p_dx_rasterizer__release(this);
}

p_dx_rasterizer& p_dx_rasterizer::operator=(const p_dx_rasterizer& other) {
    static p_dx_rasterizer* (FASTCALL * p_dx_rasterizer__move)(p_dx_rasterizer * dst, const p_dx_rasterizer * src)
        = (p_dx_rasterizer * (FASTCALL*)(p_dx_rasterizer * dst, const p_dx_rasterizer * src))0x00000001522582F0;
    return *p_dx_rasterizer__move(this, &other);
}

void p_dx_rasterizer_state::create(dx_cull_mode cull_mode, bool wireframe, bool depth_clip) {
    release();

    dx_rasterizer_state* ptr = (dx_rasterizer_state*)_operator_new(sizeof(dx_depth_stencil_state));
    ptr->uses = 0;
    ptr->free_next = 0;
    ptr->rasterizer_state = 0;

    const D3D11_CULL_MODE cull_mode_array[] = {
        D3D11_CULL_NONE,
        D3D11_CULL_FRONT,
        D3D11_CULL_BACK,
    };

    D3D11_RASTERIZER_DESC desc;
    desc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    desc.CullMode = cull_mode_array[cull_mode];
    desc.FrontCounterClockwise = TRUE;
    desc.DepthBias = 0;
    desc.DepthBiasClamp = 0.0f;
    desc.SlopeScaledDepthBias = 0.0f;
    desc.DepthClipEnable = depth_clip ? TRUE : FALSE;
    desc.ScissorEnable = FALSE;
    desc.MultisampleEnable = FALSE;
    desc.AntialiasedLineEnable = FALSE;
    d3d_device->CreateRasterizerState(&desc, &ptr->rasterizer_state);
    this->ptr = ptr;

    ptr->uses++;
}

void p_dx_rasterizer_state::release() {
    static void (*p_dx_rasterizer_state__release)(p_dx_rasterizer_state * This)
        = (void (*)(p_dx_rasterizer_state * This))0x00000001402BDFF0;
    p_dx_rasterizer_state__release(this);
}

p_dx_rasterizer_state& p_dx_rasterizer_state::operator=(const p_dx_rasterizer_state& other) {
    static p_dx_rasterizer_state* (FASTCALL * p_dx_rasterizer_state__move)(p_dx_rasterizer_state * dst, const p_dx_rasterizer_state * src)
        = (p_dx_rasterizer_state * (FASTCALL*)(p_dx_rasterizer_state * dst, const p_dx_rasterizer_state * src))0x00000001522582F0;
    return *p_dx_rasterizer_state__move(this, &other);
}

void p_dx_render_target::create(const std::pair<p_dx_texture, int32_t>* color_texs, int32_t num, const std::pair<p_dx_texture, int32_t>* depth_tex) {
    static void (*p_dx_render_target__create)(p_dx_render_target * This, const std::pair<p_dx_texture, int32_t> * color_texs, int32_t num, const std::pair<p_dx_texture, int32_t> * depth_tex)
        = (void (*)(p_dx_render_target * This, const std::pair<p_dx_texture, int32_t> * color_texs, int32_t num, const std::pair<p_dx_texture, int32_t> * depth_tex))0x00000001402BCE50;
    p_dx_render_target__create(this, color_texs, num, depth_tex);
}

void p_dx_render_target::release() {
    static void (*p_dx_render_target__release)(p_dx_render_target * This)
        = (void (*)(p_dx_render_target * This))0x00000001402BCEE0;
    p_dx_render_target__release(this);
}

p_dx_render_target& p_dx_render_target::operator=(const p_dx_render_target& other) {
    static p_dx_render_target* (FASTCALL * p_dx_render_target__move)(p_dx_render_target * dst, const p_dx_render_target * src)
        = (p_dx_render_target * (FASTCALL*)(p_dx_render_target * dst, const p_dx_render_target * src))0x00000001402BCE00;
    return *p_dx_render_target__move(this, &other);
}

void p_dx_sampler_state::create(dx_texture_address address_u, dx_texture_address address_v, int32_t filter, bool anisotropic) {
    const vec4 border_color = 0.0f;
    static void (*p_dx_sampler_state__create)(p_dx_sampler_state * This, dx_texture_address address_u, dx_texture_address address_v, int32_t filter, bool anisotropic, const vec4 * border_color)
        = (void (*)(p_dx_sampler_state * This, dx_texture_address address_u, dx_texture_address address_v, int32_t filter, bool anisotropic, const vec4 * border_color))0x00000001402BDD40;
    p_dx_sampler_state__create(this, address_u, address_v, filter, anisotropic, &border_color);
}

void p_dx_sampler_state::release() {
    static void (*p_dx_sampler_state__release)(p_dx_sampler_state * This)
        = (void (*)(p_dx_sampler_state * This))0x00000001402BDE80;
    p_dx_sampler_state__release(this);
}

void p_dx_state::begin_event(const char* str) {
    static void (FASTCALL * p_dx_state__begin_event)(p_dx_state * This, const char* str)
        = (void (FASTCALL*)(p_dx_state * This, const char* str))0x00000001402C0480;
    p_dx_state__begin_event(this, str);
}

void p_dx_state::clear_depth_stencil_view(uint32_t stencil_value, float_t depth_value, bool clear_stencil) {
    dx_state__update(ptr);

    if (!ptr->curr_render_target->depth_stencil_view)
        return;

    dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_clear_depth_stencil_view));
    cmd->type = DX_COMMAND_ClearDepthStencilView;
    dx_command_clear_depth_stencil_view& clear_depth_stencil_view = *(dx_command_clear_depth_stencil_view*)cmd->data;
    clear_depth_stencil_view.pDepthStencilView = ptr->curr_render_target->depth_stencil_view;
    clear_depth_stencil_view.ClearFlags = (clear_stencil ? D3D11_CLEAR_STENCIL : 0) | D3D11_CLEAR_DEPTH;
    clear_depth_stencil_view.Depth = depth_value;
    clear_depth_stencil_view.Stencil = (uint8_t)stencil_value;
    enum_or(ptr->flags, DX_STATE_FLAG_2);
}

void p_dx_state::clear_render_target_view(float_t r, float_t g, float_t b, float_t a) {
    dx_state__update(ptr);

    for (int32_t i = 0; i < ptr->curr_render_target->num_render_target_views; i++) {
        dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_clear_render_target_view));
        cmd->type = DX_COMMAND_ClearRenderTargetView;
        dx_command_clear_render_target_view& clear_render_target_view = *(dx_command_clear_render_target_view*)cmd->data;
        clear_render_target_view.pRenderTargetView = ptr->curr_render_target->render_target_views[i];
        clear_render_target_view.ColorRGBA[0] = r;
        clear_render_target_view.ColorRGBA[1] = g;
        clear_render_target_view.ColorRGBA[2] = b;
        clear_render_target_view.ColorRGBA[3] = a;
    }
    enum_or(ptr->flags, DX_STATE_FLAG_2);
}

void p_dx_state::copy_texture_region(p_dx_texture* dst, int32_t dst_x, int32_t dst_y,
    p_dx_texture* src, int32_t src_x, int32_t src_y, int32_t src_width, int32_t src_height) {
    static void (FASTCALL * p_dx_state__copy_texture_region)(p_dx_state * This, p_dx_texture * dst, int32_t dst_x, int32_t dst_y,
        p_dx_texture * src, int32_t src_x, int32_t src_y, int32_t src_width, int32_t src_height)
        = (void (FASTCALL*)(p_dx_state * This, p_dx_texture * dst, int32_t dst_x, int32_t dst_y,
            p_dx_texture * src, int32_t src_x, int32_t src_y, int32_t src_width, int32_t src_height))0x00000001402C05F0;
    p_dx_state__copy_texture_region(this, dst, dst_x, dst_y, src, src_x, src_y, src_width, src_height);
}

void p_dx_state::draw(uint32_t vertex_count, int32_t start_vertex_location) {
    dx_state__update(ptr);

    dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_draw));
    cmd->type = DX_COMMAND_Draw;
    dx_command_draw& draw = *(dx_command_draw*)cmd->data;
    draw.VertexCount = vertex_count;
    draw.StartVertexLocation = start_vertex_location;
    enum_or(ptr->flags, DX_STATE_FLAG_2);
}

void p_dx_state::draw_indexed(uint32_t index_count, uint32_t start_index_location, int32_t base_vertex_location) {
    dx_state__update(ptr);

    dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_draw_indexed));
    cmd->type = DX_COMMAND_DrawIndexed;
    dx_command_draw_indexed& draw_indexed = *(dx_command_draw_indexed*)cmd->data;
    draw_indexed.IndexCount = index_count;
    draw_indexed.StartIndexLocation = start_index_location;
    draw_indexed.BaseVertexLocation = base_vertex_location;
    enum_or(ptr->flags, DX_STATE_FLAG_2);
}

void p_dx_state::end_event() {
    static void (FASTCALL * p_dx_state__end_event)(p_dx_state * This)
        = (void (FASTCALL*)(p_dx_state * This))0x00000001402C0590;
    p_dx_state__end_event(this);
}

p_dx_texture* p_dx_state::get_curr_render_target_textures() {
    dx_render_target* curr_render_target = ptr->curr_render_target;
    if (ptr->update_flags & DX_STATE_UPDATE_RENDER_TARGET)
        curr_render_target = ptr->render_target;
    if (!curr_render_target)
        curr_render_target = ptr->default_render_target;
    return curr_render_target->textures;
}

void p_dx_state::get_viewport(int32_t* x, int32_t* y, int32_t* width, int32_t* height) {
    if (ptr->width >= 0) {
        *x = ptr->x;
        *y = ptr->y;
        *width = ptr->width;
        *height = ptr->height;
    }
    else {
        *x = ptr->curr_x;
        *y = ptr->curr_y;
        *width = ptr->curr_width;
        *height = ptr->curr_height;
    }
}

void p_dx_state::get_viewport(int32_t& x, int32_t& y, int32_t& width, int32_t& height) {
    if (ptr->width >= 0) {
        x = ptr->x;
        y = ptr->y;
        width = ptr->width;
        height = ptr->height;
    }
    else {
        x = ptr->curr_x;
        y = ptr->curr_y;
        width = ptr->curr_width;
        height = ptr->curr_height;
    }
}

void* p_dx_state::map(p_dx_buffer& p_dx_buf, int32_t size) {
    dx_state* ptr = this->ptr;
    enum_or(ptr->flags, DX_STATE_FLAG_2);

    dx_buffer* dx_buf = p_dx_buf.ptr;
    dx_buf->flags |= 0x01;
    if (!(dx_buf->flags & 0x02))
        return dx_buf->data;

    if (!size)
        size = dx_buf->size;

    dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_map) + size);
    cmd->type = DX_COMMAND_Map;
    dx_command_map& map = *(dx_command_map*)cmd->data;
    map.pResource = dx_buf->buffer;
    map.Subresource = 0;
    map.MapType = D3D11_MAP_WRITE_DISCARD;
    map.MapFlags = 0;
    map.Size = size;
    return &cmd->data[sizeof(dx_command_map)];

    static p_dx_buffer* (FASTCALL * p_dx_state__map)(p_dx_state* This, p_dx_buffer & p_dx_buf, int32_t size)
        = (p_dx_buffer * (FASTCALL*)(p_dx_state* This, p_dx_buffer & p_dx_buf, int32_t size))0x00000001402C0240;
    return p_dx_state__map(this, p_dx_buf, size);
}

void p_dx_state::pop() {
    auto elem = p_dx_state_stack.find(this);
    if (elem == p_dx_state_stack.end() || elem->second.depth < 1)
        return;

    dx_state* new_state = &elem->second.stack.back();
    dx_state* old_state = ptr;

    dx_state_update_flags update_flags = new_state->update_flags;
    if (new_state->render_target != old_state->curr_render_target) {
        new_state->curr_render_target = old_state->curr_render_target;
        enum_or(update_flags, DX_STATE_UPDATE_RENDER_TARGET);
    }

    if (new_state->x != old_state->curr_x || new_state->y != old_state->curr_y
        || new_state->width != old_state->curr_width || new_state->height != old_state->curr_height) {
        new_state->curr_x = old_state->curr_x;
        new_state->curr_y = old_state->curr_y;
        new_state->curr_width = old_state->curr_width;
        new_state->curr_height = old_state->curr_height;
        enum_or(update_flags, DX_STATE_UPDATE_VIEWPORT);
    }

    if (new_state->vs_shader != old_state->curr_vs_shader) {
        new_state->curr_vs_shader = old_state->curr_vs_shader;
        enum_or(update_flags, DX_STATE_UPDATE_VS_SHADER);
    }

    if (new_state->ps_shader != old_state->curr_ps_shader) {
        new_state->curr_ps_shader = old_state->curr_ps_shader;
        enum_or(update_flags, DX_STATE_UPDATE_PS_SHADER);
    }

    new_state->vertex_buffer_begin = 12;
    new_state->vertex_buffer_end = -1;
    for (int32_t i = 0; i < 12; i++)
        if (memcmp(&new_state->vertex_buffer[i], &old_state->curr_vertex_buffer[i],
            sizeof(dx_buffer_offset_stride))) {
            new_state->curr_vertex_buffer[i] = old_state->curr_vertex_buffer[i];
            new_state->vertex_buffer_begin = min_def(new_state->vertex_buffer_begin, i);
            new_state->vertex_buffer_end = max_def(new_state->vertex_buffer_end, i);
            enum_or(update_flags, DX_STATE_UPDATE_VERTEX_BUFFER);
        }

    if (new_state->index_buffer_format != old_state->curr_index_buffer_format
        || new_state->index_buffer != old_state->curr_index_buffer
        || new_state->index_buffer_offset != old_state->curr_index_buffer_offset) {
        new_state->curr_index_buffer_format = old_state->curr_index_buffer_format;
        new_state->curr_index_buffer = old_state->curr_index_buffer;
        new_state->curr_index_buffer_offset = old_state->curr_index_buffer_offset;
        enum_or(update_flags, DX_STATE_UPDATE_INDEX_BUFFER);
    }

    if (new_state->primitive_topology != old_state->curr_primitive_topology) {
        new_state->curr_primitive_topology = old_state->curr_primitive_topology;
        enum_or(update_flags, DX_STATE_UPDATE_PRIMITIVE_TOPOLOGY);
    }

    new_state->VSConstantBuffersStartSlot = 4;
    new_state->VSConstantBuffersEndSlot = -1;
    for (int32_t i = 0; i < 4; i++)
        if (new_state->VSConstantBuffers[i] != old_state->VSCurrConstantBuffers[i]) {
            new_state->VSCurrConstantBuffers[i] = old_state->VSCurrConstantBuffers[i];
            new_state->VSConstantBuffersStartSlot = min_def(new_state->VSConstantBuffersStartSlot, i);
            new_state->VSConstantBuffersEndSlot = max_def(new_state->VSConstantBuffersEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_VS_BUFFER);
        }

    new_state->VSShaderStartSlot = 16;
    new_state->VSShaderEndSlot = -1;
    for (int32_t i = 0; i < 16; i++)
        if (new_state->VSShaderResources[i] != old_state->VSCurrShaderResources[i]) {
            new_state->VSCurrShaderResources[i] = old_state->VSCurrShaderResources[i];
            new_state->VSShaderStartSlot = min_def(new_state->VSShaderStartSlot, i);
            new_state->VSShaderEndSlot = max_def(new_state->VSShaderEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_VS_RESOURCE);
        }

    new_state->VSTextureStartSlot = 16;
    new_state->VSTextureEndSlot = -1;
    for (int32_t i = 0; i < 16; i++)
        if (new_state->VSTextures[i] != old_state->VSCurrTextures[i]) {
            new_state->VSCurrTextures[i] = old_state->VSCurrTextures[i];
            new_state->VSTextureStartSlot = min_def(new_state->VSTextureStartSlot, i);
            new_state->VSTextureEndSlot = max_def(new_state->VSTextureEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_VS_TEXTURE);
        }

    new_state->VSSamplerStartSlot = 16;
    new_state->VSSamplerEndSlot = -1;
    for (int32_t i = 0; i < 16; i++)
        if (new_state->VSSamplers[i] != old_state->VSCurrSamplers[i]) {
            new_state->VSSamplers[i] = old_state->VSCurrSamplers[i];
            new_state->VSSamplerStartSlot = min_def(new_state->VSSamplerStartSlot, i);
            new_state->VSSamplerEndSlot = max_def(new_state->VSSamplerEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_VS_SAMPLER);
        }

    new_state->PSConstantBuffersStartSlot = 4;
    new_state->PSConstantBuffersEndSlot = -1;
    for (int32_t i = 0; i < 4; i++)
        if (new_state->PSConstantBuffers[i] != old_state->PSCurrConstantBuffers[i]) {
            new_state->PSCurrConstantBuffers[i] = old_state->PSCurrConstantBuffers[i];
            new_state->PSConstantBuffersStartSlot = min_def(new_state->PSConstantBuffersStartSlot, i);
            new_state->PSConstantBuffersEndSlot = max_def(new_state->PSConstantBuffersEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_PS_BUFFER);
        }

    new_state->PSTextureStartSlot = 24;
    new_state->PSTextureEndSlot = -1;
    for (int32_t i = 0; i < 24; i++)
        if (new_state->PSTextures[i] != old_state->PSCurrTextures[i]) {
            new_state->PSCurrTextures[i] = old_state->PSCurrTextures[i];
            new_state->PSTextureStartSlot = min_def(new_state->PSTextureStartSlot, i);
            new_state->PSTextureEndSlot = max_def(new_state->PSTextureEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_PS_TEXTURE);
        }

    new_state->PSSamplerStartSlot = 16;
    new_state->PSSamplerEndSlot = -1;
    for (int32_t i = 0; i < 16; i++)
        if (new_state->PSSamplers[i] != old_state->PSCurrSamplers[i]) {
            new_state->PSCurrSamplers[i] = old_state->PSCurrSamplers[i];
            new_state->PSSamplerStartSlot = min_def(new_state->PSSamplerStartSlot, i);
            new_state->PSSamplerEndSlot = max_def(new_state->PSSamplerEndSlot, i);
            enum_or(update_flags, DX_STATE_UPDATE_PS_SAMPLER);
        }

    if (new_state->RSRasterizerState != old_state->RSCurrRasterizerState) {
        new_state->RSCurrRasterizerState = old_state->RSCurrRasterizerState;
        enum_or(update_flags, DX_STATE_UPDATE_RASTERIZER_STATE);
    }

    if (new_state->OMDepthStencilState != old_state->OMCurrDepthStencilState) {
        new_state->OMCurrDepthStencilState = old_state->OMCurrDepthStencilState;
        enum_or(update_flags, DX_STATE_UPDATE_DEPTH_STENCIL_STATE);
    }

    if (new_state->OMBlendState != old_state->OMCurrBlendState) {
        new_state->OMCurrBlendState = old_state->OMCurrBlendState;
        enum_or(update_flags, DX_STATE_UPDATE_BLEND_STATE);
    }

    *ptr = *new_state;
    ptr->update_flags = update_flags;

    if (--elem->second.depth >= 1)
        elem->second.stack.pop_back();
    else
        p_dx_state_stack.erase(this);
}

void p_dx_state::push() {
    auto elem = p_dx_state_stack.find(this);
    if (elem != p_dx_state_stack.end()) {
        if (elem->second.depth < 1)
            elem->second.depth = 1;
        else
            elem->second.depth++;
        elem->second.stack.push_back(*ptr);
    }
    else {
        elem = p_dx_state_stack.insert({ this, {} }).first;
        elem->second.depth = 1;
        elem->second.stack.push_back(*ptr);
    }
}

void p_dx_state::set_blend_state(p_dx_blend_state* p_dx_blend) {
    dx_state* ptr = this->ptr;
    ptr->OMBlendState = p_dx_blend->ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_BLEND_STATE);
}

void p_dx_state::set_depth_stencil_state(p_dx_depth_stencil_state* p_dx_ds) {
    dx_state* ptr = this->ptr;
    ptr->OMDepthStencilState = p_dx_ds->ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_DEPTH_STENCIL_STATE);
}

void p_dx_state::set_index_buffer(p_dx_buffer* p_dx_buf, int32_t format, int32_t offset) {
    dx_state* ptr = this->ptr;
    *(int32_t*)&ptr->index_buffer_format = format;
    ptr->index_buffer = p_dx_buf->ptr;
    *(int32_t*)&ptr->index_buffer_offset = offset;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_INDEX_BUFFER);
}

void p_dx_state::set_primitive_topology(dx_primitive prim) {
    dx_state* ptr = this->ptr;
    ptr->primitive_topology = prim;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PRIMITIVE_TOPOLOGY);
}

void p_dx_state::set_ps_constant_buffer(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_buffers && start_slot + i < 4; i++)
        ptr->PSConstantBuffers[start_slot + i] = p_dx_buf->ptr;
    ptr->PSConstantBuffersStartSlot = min_def(ptr->PSConstantBuffersStartSlot, start_slot);
    ptr->PSConstantBuffersEndSlot = max_def(ptr->PSConstantBuffersEndSlot, start_slot + num_buffers - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PS_BUFFER);
}

void p_dx_state::set_ps_sampler_state(int32_t start_slot, int32_t num_views, p_dx_sampler_state* p_dx_ss) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_views && start_slot + i < 16; i++)
        ptr->PSSamplers[start_slot + i] = p_dx_ss[i].ptr;
    ptr->PSSamplerStartSlot = min_def(ptr->PSSamplerStartSlot, start_slot);
    ptr->PSSamplerEndSlot = max_def(ptr->PSSamplerEndSlot, start_slot + num_views - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PS_SAMPLER);
}

void p_dx_state::set_ps_shader(p_dx_pixel_shader* p_dx_ps) {
    dx_state* ptr = this->ptr;
    ptr->ps_shader = p_dx_ps->ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PS_SHADER);
}

void p_dx_state::set_ps_shader(p_dx_pixel_shader& p_dx_ps) {
    dx_state* ptr = this->ptr;
    ptr->ps_shader = p_dx_ps.ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PS_SHADER);
}

void p_dx_state::set_ps_textures(int32_t start_slot, int32_t num_views, p_dx_texture* p_dx_tex) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_views && start_slot + i < 24; i++)
        ptr->PSTextures[start_slot + i] = p_dx_tex[i].ptr;
    ptr->PSTextureStartSlot = min_def(ptr->PSTextureStartSlot, start_slot);
    ptr->PSTextureEndSlot = max_def(ptr->PSTextureEndSlot, start_slot + num_views - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_PS_TEXTURE);
}

void p_dx_state::set_rasterizer_state(p_dx_rasterizer_state* p_dx_rs) {
    dx_state* ptr = this->ptr;
    ptr->RSRasterizerState = p_dx_rs->ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_RASTERIZER_STATE);
}

void p_dx_state::set_render_target(p_dx_render_target* p_dx_rend_targ) {
    dx_state* ptr = this->ptr;
    if (p_dx_rend_targ->ptr)
        ptr->render_target = p_dx_rend_targ->ptr;
    else
        ptr->render_target = ptr->default_render_target;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_RENDER_TARGET);
}

void p_dx_state::set_vertex_buffer(int32_t start_vertex_buffer, int32_t num_vertex_buffers,
    p_dx_buffer** buffers, int32_t* strides, int32_t* offsets) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_vertex_buffers && start_vertex_buffer + i < 12; i++) {
        ptr->vertex_buffer[start_vertex_buffer + i].buffer = buffers[i]->ptr;
        ptr->vertex_buffer[start_vertex_buffer + i].stride = strides[i];
        ptr->vertex_buffer[start_vertex_buffer + i].offset = offsets[i];
    }
    ptr->vertex_buffer_begin = min_def(ptr->vertex_buffer_begin, start_vertex_buffer);
    ptr->vertex_buffer_end = max_def(ptr->vertex_buffer_end, start_vertex_buffer + num_vertex_buffers - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VERTEX_BUFFER);
}

void p_dx_state::set_viewport(int32_t x, int32_t y, int32_t width, int32_t height) {
    dx_state* ptr = this->ptr;
    ptr->x = x;
    ptr->y = y;
    ptr->width = width;
    ptr->height = height;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VIEWPORT);
}

void p_dx_state::set_vs_constant_buffer(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_buffers && start_slot + i < 4; i++)
        ptr->VSConstantBuffers[start_slot + i] = p_dx_buf->ptr;
    ptr->VSConstantBuffersStartSlot = min_def(ptr->VSConstantBuffersStartSlot, start_slot);
    ptr->VSConstantBuffersEndSlot = max_def(ptr->VSConstantBuffersEndSlot, start_slot + num_buffers - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_BUFFER);
}

void p_dx_state::set_vs_sampler_state(int32_t start_slot, int32_t num_views, p_dx_sampler_state* p_dx_ss) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_views && start_slot + i < 16; i++)
        ptr->VSSamplers[start_slot + i] = p_dx_ss[i].ptr;
    ptr->VSSamplerStartSlot = min_def(ptr->VSSamplerStartSlot, start_slot);
    ptr->VSSamplerEndSlot = max_def(ptr->VSSamplerEndSlot, start_slot + num_views - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_SAMPLER);
}

void p_dx_state::set_vs_shader(p_dx_vertex_shader* p_dx_vs) {
    dx_state* ptr = this->ptr;
    ptr->vs_shader = p_dx_vs->ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_SHADER);
}

void p_dx_state::set_vs_shader(p_dx_vertex_shader& p_dx_vs) {
    dx_state* ptr = this->ptr;
    ptr->vs_shader = p_dx_vs.ptr;
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_SHADER);
}

void p_dx_state::set_vs_shader_resource(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_buffers && start_slot + i < 16; i++) {
        ptr->VSShaderResources[start_slot + i] = p_dx_buf->ptr;
        ptr->VSTextures[start_slot + i] = 0;
        ptr->VSCurrTextures[start_slot + i] = 0;
    }
    ptr->VSShaderStartSlot = min_def(ptr->VSShaderStartSlot, start_slot);
    ptr->VSShaderEndSlot = max_def(ptr->VSShaderEndSlot, start_slot + num_buffers - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_RESOURCE);
}

void p_dx_state::set_vs_textures(int32_t start_slot, int32_t num_views, p_dx_texture* p_dx_tex) {
    dx_state* ptr = this->ptr;
    for (int32_t i = 0; i < num_views && start_slot + i < 16; i++) {
        ptr->VSShaderResources[start_slot + i] = 0;
        ptr->VSTextures[start_slot + i] = p_dx_tex[i].ptr;
        ptr->VSCurrShaderResources[start_slot + i] = 0;
    }
    ptr->VSTextureStartSlot = min_def(ptr->VSTextureStartSlot, start_slot);
    ptr->VSTextureEndSlot = max_def(ptr->VSTextureEndSlot, start_slot + num_views - 1);
    enum_or(ptr->update_flags, DX_STATE_UPDATE_VS_TEXTURE);
}

void p_dx_state::unmap(p_dx_buffer& p_dx_buf) {
    dx_state* ptr = this->ptr;

    dx_buffer* dx_buf = p_dx_buf.ptr;
    dx_buf->flags &= ~0x01;
    if (dx_buf->flags & 0x02) {
        dx_command* cmd = ptr->command_array->get_next_command(sizeof(dx_command_unmap));
        cmd->type = DX_COMMAND_Unmap;
        dx_command_unmap& unmap = *(dx_command_unmap*)cmd->data;
        unmap.pResource = dx_buf->buffer;
        unmap.Subresource = 0;
    }
    enum_or(ptr->flags, DX_STATE_FLAG_2);
    return;

    static void (FASTCALL * p_dx_state__unmap)(p_dx_state * This, p_dx_buffer & p_dx_buf)
        = (void (FASTCALL*)(p_dx_state * This, p_dx_buffer & p_dx_buf))0x00000001402C02B0;
    p_dx_state__unmap(this, p_dx_buf);
}

void p_dx_texture::create(const void* data, int32_t size, dx_texture_file_type type) {
    static void (*p_dx_texture__create)(p_dx_texture * This, const void* data, int32_t size, dx_texture_file_type type)
        = (void (*)(p_dx_texture * This, const void* data, int32_t size, dx_texture_file_type type))0x00000001402BC480;
    p_dx_texture__create(this, data, size, type);
}

void p_dx_texture::create(uint32_t width, uint32_t height, const void* data) {
    static void (*p_dx_texture__create)(p_dx_texture * This, uint32_t width, uint32_t height, const void* data)
        = (void (*)(p_dx_texture * This, uint32_t width, uint32_t height, const void* datae))0x00000001402BC500;
    p_dx_texture__create(this, width, height, data);
}

void p_dx_texture::create(uint32_t width, uint32_t height, dx_texture_format format,
    int32_t type, uint32_t mip_levels, bool cube_map, bool generate_mips) {
    static void (*p_dx_texture__create)(p_dx_texture * This, uint32_t width, uint32_t height, dx_texture_format format,
        int32_t type, uint32_t mip_levels, bool cube_map, bool generate_mips)
        = (void (*)(p_dx_texture * This, uint32_t width, uint32_t height, dx_texture_format format,
            int32_t type, uint32_t mip_levels, bool cube_map, bool generate_mips))0x00000001402BC670;
    p_dx_texture__create(this, width, height, format, type, mip_levels, cube_map, generate_mips);
}

uint32_t p_dx_texture::get_height() const {
    return ptr->height;
}

uint32_t p_dx_texture::get_width() const {
    return ptr->width;
}

void p_dx_texture::release() {
    static void (*p_dx_texture__release)(p_dx_texture * This)
        = (void (*)(p_dx_texture * This))0x00000001402BC7A0;
    p_dx_texture__release(this);
}

p_dx_texture& p_dx_texture::operator=(const p_dx_texture& other) {
    static p_dx_texture* (FASTCALL * p_dx_texture__move)(p_dx_texture * dst, const p_dx_texture * src)
        = (p_dx_texture * (FASTCALL*)(p_dx_texture * dst, const p_dx_texture * src))0x00000001402BC400;
    return *p_dx_texture__move(this, &other);
}

void p_dx_vertex_shader::create(const void* data, int32_t size,
    const dx_input_layout_init_data* input_layout_array, int32_t num_input_layout) {
    static void (*p_dx_vertex_shader__create)(p_dx_vertex_shader * This, const void* data, int32_t size,
        const dx_input_layout_init_data * input_layout_array, int32_t num_input_layout)
        = (void (*)(p_dx_vertex_shader * This, const void* data, int32_t size,
            const dx_input_layout_init_data * input_layout_array, int32_t num_input_layout))0x00000001402BD960;
    p_dx_vertex_shader__create(this, data, size, input_layout_array, num_input_layout);
}

void p_dx_vertex_shader::release() {
    static void (*p_dx_vertex_shader__release)(p_dx_vertex_shader * This)
        = (void (*)(p_dx_vertex_shader * This))0x00000001402BD9E0;
    p_dx_vertex_shader__release(this);
}

p_dx_vertex_shader& p_dx_vertex_shader::operator=(const p_dx_vertex_shader& other) {
    static p_dx_vertex_shader* (FASTCALL * p_dx_vertex_shader__move)(p_dx_vertex_shader * dst, const p_dx_vertex_shader * src)
        = (p_dx_vertex_shader * (FASTCALL*)(p_dx_vertex_shader * dst, const p_dx_vertex_shader * src))0x00000001402BD910;
    return *p_dx_vertex_shader__move(this, &other);
}
