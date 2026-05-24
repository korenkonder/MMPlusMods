/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "gl.hpp"
#include <d3d11.h>

enum dx_blend {
    DX_BLEND_ZERO           = 0x00,
    DX_BLEND_ONE            = 0x01,
    DX_BLEND_SRC_ALPHA      = 0x02,
    DX_BLEND_INV_SRC_ALPHA  = 0x03,
    DX_BLEND_SRC_COLOR      = 0x04,
    DX_BLEND_INV_SRC_COLOR  = 0x05,
    DX_BLEND_DEST_ALPHA     = 0x06,
    DX_BLEND_INV_DEST_ALPHA = 0x07,
    DX_BLEND_DEST_COLOR     = 0x08,
    DX_BLEND_INV_DEST_COLOR = 0x09,
    DX_BLEND_SRC_ALPHA_SAT  = 0x0A,
};

enum dx_blend_write_mask {
    DX_BLEND_WRITE_MASK_RGBA = 0x00,
    DX_BLEND_WRITE_MASK_RGB  = 0x01,
    DX_BLEND_WRITE_MASK_NONE = 0x02,
};

enum dx_buffer_type {
    DX_BUFFER_CONSTANT = 0x00,
    DX_BUFFER_VERTEX   = 0x01,
    DX_BUFFER_INDEX    = 0x02,
    DX_BUFFER_OTHER    = 0x03,
};

enum dx_cull_mode {
    DX_CULL_BACK  = 0x00,
    DX_CULL_FRONT = 0x01,
    DX_CULL_NONE  = 0x02,
};

enum dx_depth_func {
    DX_DEPTH_FUNC_LESS_EQUAL    = 0x00,
    DX_DEPTH_FUNC_GREATER_EQUAL = 0x01,
    DX_DEPTH_FUNC_ALWAYS        = 0x02,
};

enum dx_depth_test {
    DX_DEPTH_NONE  = 0x00,
    DX_DEPTH_TEST  = 0x01,
    DX_DEPTH_WRITE = 0x02,
};

enum dx_primitive {
    DX_PRIMITIVE_TRIANGLE_LIST  = 0x00,
    DX_PRIMITIVE_TRIANGLE_STRIP = 0x01,
    DX_PRIMITIVE_LINE_LIST      = 0x02,
    DX_PRIMITIVE_LINE_STRIP     = 0x03,
};

enum dx_texture_address {
    DX_TEXTURE_ADDRESS_MIRROR = 0x00,
    DX_TEXTURE_ADDRESS_WRAP   = 0x01,
    DX_TEXTURE_ADDRESS_CLAMP  = 0x02,
    DX_TEXTURE_ADDRESS_BORDER = 0x03,
};

enum dx_texture_file_type {
    DX_TEXTURE_FILE_DDS = 0x00,
    DX_TEXTURE_FILE_TXP = 0x01,
};

enum dx_texture_format {
    DX_TEXTURE_FORMAT_R8_UNORM           = 0x00,
    DX_TEXTURE_FORMAT_R8G8_UNORM         = 0x01,
    DX_TEXTURE_FORMAT_B5G6R5_UNORM       = 0x02,
    DX_TEXTURE_FORMAT_R8G8B8A8_UNORM     = 0x03,
    DX_TEXTURE_FORMAT_B8G8R8A8_UNORM     = 0x04,
    DX_TEXTURE_FORMAT_R32_FLOAT          = 0x05,
    DX_TEXTURE_FORMAT_R32G32_FLOAT       = 0x06,
    DX_TEXTURE_FORMAT_R16G16_FLOAT       = 0x07,
    DX_TEXTURE_FORMAT_R11G11B10_FLOAT    = 0x08,
    DX_TEXTURE_FORMAT_R16G16B16A16_FLOAT = 0x09,
    DX_TEXTURE_FORMAT_BC1_UNORM          = 0x0A,
    DX_TEXTURE_FORMAT_BC2_UNORM          = 0x0B,
    DX_TEXTURE_FORMAT_BC3_UNORM          = 0x0C,
    DX_TEXTURE_FORMAT_BC4_UNORM          = 0x0D,
    DX_TEXTURE_FORMAT_BC5_UNORM          = 0x0E,
    DX_TEXTURE_FORMAT_BC6H_UF16          = 0x0F,
    DX_TEXTURE_FORMAT_BC7_UNORM          = 0x10,
    DX_TEXTURE_FORMAT_R32_TYPELESS       = 0x11,
    DX_TEXTURE_FORMAT_B8G8R8X8_TYPELESS  = 0x12,
    DX_TEXTURE_FORMAT_UNKNOWN            = 0x13,
    DX_TEXTURE_FORMAT_MAX                = 0x14,
};

struct dx_input_layout_init_data {
    int32_t index;
    int32_t format;
    int32_t input_slot;
    int32_t offset;
};

static_assert(sizeof(dx_input_layout_init_data) == 0x10, "\"dx_input_layout_init_data\" struct should have a size of 0x10");

struct p_dx_blend_state;
struct p_dx_buffer;
struct p_dx_depth_stencil_state;
struct p_dx_pixel_shader;
struct p_dx_query_occlusion;
struct p_dx_rasterizer;
struct p_dx_rasterizer_state;
struct p_dx_sampler_state;
struct p_dx_state;
struct p_dx_texture;
struct p_dx_vertex_shader;

struct p_dx_blend_state {
    struct dx_blend_state* ptr;

    inline p_dx_blend_state() : ptr() {

    }

    inline p_dx_blend_state(const p_dx_blend_state& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_blend_state() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(bool blend_enable, dx_blend src_blend, dx_blend dst_blend,
        dx_blend src_blend_alpha, dx_blend dst_blend_alpha, D3D11_COLOR_WRITE_ENABLE write_mask);
    void release();

    p_dx_blend_state& operator=(const p_dx_blend_state& other);
};

struct p_dx_buffer {
    struct dx_buffer* ptr;

    inline p_dx_buffer() : ptr() {

    }

    inline p_dx_buffer(const p_dx_buffer& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_buffer() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(dx_buffer_type type, int32_t size);
    void create_immutable(dx_buffer_type type, const void* data, int32_t size);
    void release();

    p_dx_buffer& operator=(const p_dx_buffer& other);
};

struct p_dx_depth_stencil_state {
    struct dx_depth_stencil_state* ptr;

    inline p_dx_depth_stencil_state() : ptr() {

    }

    inline p_dx_depth_stencil_state(const p_dx_depth_stencil_state& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_depth_stencil_state() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(dx_depth_test depth_test, dx_depth_func depth_func);
    void release();

    p_dx_depth_stencil_state& operator=(const p_dx_depth_stencil_state& other);
};

struct p_dx_pixel_shader {
    struct dx_pixel_shader* ptr;

    inline p_dx_pixel_shader() : ptr() {

    }

    inline p_dx_pixel_shader(const p_dx_pixel_shader& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_pixel_shader() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(const void* data, int32_t size);
    void release();

    p_dx_pixel_shader& operator=(const p_dx_pixel_shader& other);
};

struct p_dx_query_occlusion {
    struct dx_query_occlusion* ptr;
};

struct p_dx_rasterizer {
    struct dx_rasterizer* ptr;

    inline p_dx_rasterizer() : ptr() {

    }

    inline p_dx_rasterizer(const p_dx_rasterizer& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_rasterizer() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void release();

    p_dx_rasterizer& operator=(const p_dx_rasterizer& other);
};

struct p_dx_rasterizer_state {
    struct dx_rasterizer_state* ptr;

    inline p_dx_rasterizer_state() : ptr() {

    }

    inline p_dx_rasterizer_state(const p_dx_rasterizer_state& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_rasterizer_state() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(dx_cull_mode cull_mode, bool wireframe, bool depth_clip = true);
    void release();

    p_dx_rasterizer_state& operator=(const p_dx_rasterizer_state& other);
};

struct p_dx_render_target {
    struct dx_render_target* ptr;

    inline p_dx_render_target() : ptr() {

    }

    inline p_dx_render_target(const p_dx_render_target& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_render_target() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(const std::pair<p_dx_texture, int32_t>* color_texs,
        int32_t num, const std::pair<p_dx_texture, int32_t>* depth_tex);
    void release();

    p_dx_render_target& operator=(const p_dx_render_target& other);
};

struct p_dx_sampler_state {
    struct dx_sampler_state* ptr;

    inline p_dx_sampler_state() : ptr() {

    }

    inline ~p_dx_sampler_state() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(dx_texture_address address_u = DX_TEXTURE_ADDRESS_CLAMP,
        dx_texture_address address_v = DX_TEXTURE_ADDRESS_CLAMP,
        int32_t filter = 6, bool anisotropic = false);
    void release();

    p_dx_sampler_state& operator=(const p_dx_sampler_state& other) = delete;
};

struct p_dx_state {
    struct dx_state* ptr;

    void begin_event(const char* str);
    void clear_depth_stencil_view(uint32_t stencil_value, float_t depth_value, bool clear_stencil);
    void clear_render_target_view(float_t r, float_t g, float_t b, float_t a);
    void copy_texture_region(p_dx_texture* dst, int32_t dst_x, int32_t dst_y,
        p_dx_texture* src, int32_t src_x, int32_t src_y, int32_t src_width, int32_t src_height);
    void draw(uint32_t vertex_count, int32_t start_vertex_location = 0);
    void draw_indexed(uint32_t index_count, uint32_t start_index_location = 0, int32_t base_vertex_location = 0);
    void end_event();
    struct dx_render_target* get_curr_render_target(); // Added
    p_dx_texture* get_curr_render_target_textures();
    void get_viewport(int32_t* x, int32_t* y, int32_t* width, int32_t* height);
    void get_viewport(int32_t& x, int32_t& y, int32_t& width, int32_t& height);
    void* map(p_dx_buffer& p_dx_buf, int32_t size = 0);
    void pop();
    void push();
    void set_blend_state(p_dx_blend_state* p_dx_lend);
    void set_depth_stencil_state(p_dx_depth_stencil_state* p_dx_ds);
    void set_index_buffer(p_dx_buffer* p_dx_buf, int32_t format, int32_t offset);
    void set_primitive_topology(dx_primitive prim);
    void set_ps_constant_buffer(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf);
    void set_ps_sampler_state(int32_t start_slot, int32_t num_views, p_dx_sampler_state* p_dx_ss);
    void set_ps_shader(p_dx_pixel_shader* p_dx_ps);
    void set_ps_shader(p_dx_pixel_shader& p_dx_ps);
    void set_ps_textures(int32_t start_slot, int32_t num_views, p_dx_texture* p_dx_tex);
    void set_rasterizer_state(p_dx_rasterizer_state* p_dx_rs);
    void set_render_target(struct dx_render_target* dx_rend_targ); // Added
    void set_render_target(p_dx_render_target* p_dx_rend_targ);
    void set_vertex_buffer(int32_t start_vertex_buffer, int32_t num_vertex_buffers,
        p_dx_buffer** buffers, int32_t* strides, int32_t* offsets);
    void set_viewport(int32_t x, int32_t y, int32_t width, int32_t height);
    void set_vs_constant_buffer(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf);
    void set_vs_sampler_state(int32_t start_slot, int32_t num_views, p_dx_sampler_state* p_dx_ss);
    void set_vs_shader(p_dx_vertex_shader* p_dx_vs);
    void set_vs_shader(p_dx_vertex_shader& p_dx_vs);
    void set_vs_shader_resource(int32_t start_slot, int32_t num_buffers, p_dx_buffer* p_dx_buf);
    void set_vs_textures(int32_t start_slot, int32_t num_views, p_dx_texture* p_dx_tex);
    void unmap(p_dx_buffer& p_dx_buf);

    inline void write_buffer(p_dx_buffer& p_dx_buf, const void* buf, size_t size) {
        if (p_dx_buf.is_null())
            return;

        void* data = map(p_dx_buf);
        if (data)
            memcpy(data, buf, size);
        unmap(p_dx_buf);
    }

    template<typename T>
    inline void write_buffer(p_dx_buffer& p_dx_buf, const T& data) {
        write_buffer(p_dx_buf, &data, sizeof(T));
    }

    template<typename T>
    inline void write_buffer(p_dx_buffer& p_dx_buf, const T* data) {
        write_buffer(p_dx_buf, data, sizeof(T));
    }
};

struct p_dx_texture {
    struct dx_texture* ptr;

    inline p_dx_texture() : ptr() {

    }

    inline p_dx_texture(const p_dx_texture& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_texture() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(const void* data, int32_t size, dx_texture_file_type type = DX_TEXTURE_FILE_TXP);
    void create(uint32_t width, uint32_t height, const void* data);
    void create(uint32_t width, uint32_t height, dx_texture_format format,
        int32_t type, uint32_t mip_levels, bool cube_map, bool generate_mips);
    uint32_t get_height() const;
    uint32_t get_width() const;
    void release();

    p_dx_texture& operator=(const p_dx_texture& other);
};

struct p_dx_vertex_shader {
    struct dx_vertex_shader* ptr;

    inline p_dx_vertex_shader() : ptr() {

    }

    inline p_dx_vertex_shader(const p_dx_vertex_shader& other) : ptr() {
        *this = other;
    }

    inline ~p_dx_vertex_shader() {
        release();
    }

    inline bool is_null() const {
        return !ptr;
    }

    inline bool not_null() const {
        return !!ptr;
    }

    void create(const void* data, int32_t size,
        const dx_input_layout_init_data* input_layout_array = 0, int32_t num_input_layout = 0);
    void release();

    p_dx_vertex_shader& operator=(const p_dx_vertex_shader& other);
};

struct dx_vertex_pixel_shader {
    p_dx_vertex_shader vertex;
    p_dx_pixel_shader pixel;

    inline dx_vertex_pixel_shader() {

    }
};

extern ID3D11Device*& d3d_device;
extern ID3D11DeviceContext*& d3d_device_context;

extern p_dx_blend_state* (*dx_default_states_get_blend_state)(bool blend_enable,
    dx_blend src_blend, dx_blend dst_blend,
    dx_blend src_blend_alpha, dx_blend dst_blend_alpha, dx_blend_write_mask write_mask);
extern p_dx_depth_stencil_state* (*dx_default_states_get_depth_stencil_state)(
    dx_depth_test depth_test, dx_depth_func depth_func);
extern p_dx_rasterizer_state* (*dx_default_states_get_rasterizer_state)(dx_cull_mode cull_mode);
extern p_dx_sampler_state* (*dx_default_states_get_sampler_state)(
    dx_texture_address address_u, dx_texture_address address_v, bool anisotropic);
extern p_dx_texture* (*dx_default_states_get_texture)(int32_t index);
