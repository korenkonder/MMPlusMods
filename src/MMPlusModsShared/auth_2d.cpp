/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "auth_2d.hpp"
#include "file_handler.hpp"
#include "task.hpp"

struct aet_comp;
struct aet_marker;
struct aet_scene;

typedef uint16_t aet_layer_flag;
typedef uint8_t aet_layer_quality;

union aet_item {
    void* none;
    uint32_t offset;
    struct aet_video* video;
    struct aet_audio* audio;
    aet_comp* comp;
};

static_assert(sizeof(aet_item) == 0x08, "\"aet_item\" struct should have a size of 0x08");

enum aet_item_type : uint8_t {
    AET_ITEM_TYPE_NONE        = 0x00,
    AET_ITEM_TYPE_VIDEO       = 0x01,
    AET_ITEM_TYPE_AUDIO       = 0x02,
    AET_ITEM_TYPE_COMPOSITION = 0x03,
};

struct aet_layer {
    const char* name;
    float_t start_time;
    float_t end_time;
    float_t offset_time;
    float_t time_scale;
    aet_layer_flag flags;
    aet_layer_quality quality;
    aet_item_type item_type;
    aet_item item;
    aet_layer* parent;
    uint32_t markers_count;
    aet_marker* markers;
    struct aet_layer_video* video;
    struct aet_layer_audio* audio;
};

static_assert(sizeof(aet_layer) == 0x50, "\"aet_layer\" struct should have a size of 0x50");

struct AetSet;

struct AetSet_vtbl {
    AetSet* (FASTCALL* Dispose)(AetSet* This, uint8_t flags);
    void(FASTCALL* ReadFile)(AetSet* This, const char* file_path);
    bool(FASTCALL* LoadFile)(AetSet* This);
    void(FASTCALL* Unload)(AetSet* This);
    bool(FASTCALL* Load)(AetSet* This);
};

static_assert(sizeof(AetSet_vtbl) == 0x28, "\"AetSet_vtbl\" struct should have a size of 0x28");

struct AetSet {
    AetSet_vtbl* __vftable;
    int32_t index;
    aet_scene** scenes;
    uint32_t scenes_count;
    int32_t load_count;
    p_file_handler file_handler;
    bool ready;
    prj::shared_ptr<prj::stack_allocator> alloc_handler;
};

static_assert(sizeof(AetSet) == 0x40, "\"AetSet\" struct should have a size of 0x40");

struct AetObj;

struct AetObj_vtbl {
    AetObj* (FASTCALL* Dispose)(AetObj* This, uint8_t flags);
    void(FASTCALL* Init)(AetObj* This, AetArgs* init_data, aet_scene* scene, aet_comp* comp,
        aet_layer* layer, aet_marker* start_marker, aet_marker* end_marker, resolution_mode mode);
    void(FASTCALL* Ctrl)(AetObj* This);
    void(FASTCALL* Disp)(AetObj* This);
    bool(FASTCALL* StepFrame)(AetObj* This);
    void(FASTCALL* SetPlay)(AetObj* This, bool value, bool repeat);
    void(FASTCALL* SetVisible)(AetObj* This, bool value);
    void(FASTCALL* SetPosition)(AetObj* This, const vec3& value);
    void(FASTCALL* SetScale)(AetObj* This, const vec3& value);
    void(FASTCALL* SetRotation)(AetObj* This, const vec3& value);
    void(FASTCALL* SetAlpha)(AetObj* This, float_t value);
    void(FASTCALL* SetSpriteReplace)(AetObj* This, prj::map<uint32_t, uint32_t>& value);
    void(FASTCALL* SetSpriteTexture)(AetObj* This, prj::map<uint32_t, texture*>& value);
    void(FASTCALL* SetSpriteDiscard)(AetObj* This, prj::map<uint32_t, uint32_t>& value);
    void(FASTCALL* SetColor)(AetObj* This, const vec4& value);
    void(FASTCALL* SetEndTime)(AetObj* This, float_t value);
    void(FASTCALL* SetFrame)(AetObj* This, float_t value);
    void(FASTCALL* SetPrio)(AetObj* This, spr::SprPrio value);
    void(FASTCALL* SetFrameRateControl)(AetObj* This, FrameRateControl* value);
    void(FASTCALL* SetViewport)(AetObj* This, bool value, rectangle* viewport);
    aet_info(FASTCALL* GetInfo)(AetObj* This);
    bool(FASTCALL* GetPlay)(AetObj* This);
    bool(FASTCALL* GetVisible)(AetObj* This);
    float_t(FASTCALL* GetFrame)(AetObj* This);
    bool(FASTCALL* GetEnd)(AetObj* This);
    void(FASTCALL* CtrlComp)(AetObj* This, aet_comp* comp, float_t frame);
    void(FASTCALL* DispComp)(AetObj* This, mat4* mat, aet_comp* comp, float_t frame, float_t opacity);
    void(FASTCALL* CtrlLayer)(AetObj* This, aet_layer* layer, float_t frame);
    void(FASTCALL* DispLayer)(AetObj* This, mat4* mat, aet_layer* layer, float_t frame, float_t opacity);
    void(FASTCALL* DispVideo)(AetObj* This, mat4* mat, aet_layer* layer, float_t frame, float_t opacity);
    void(FASTCALL* DispSprite)(AetObj* This, mat4* mat, aet_layer* layer, float_t opacity);
    void(FASTCALL* DispSpriteSource)(AetObj* This, mat4* mat,
        aet_layer* layer, uint32_t source_index, float_t opacity);
    void(FASTCALL* CalcMat)(AetObj* This, mat4* mat, aet_layer* layer, float_t frame);
};

static_assert(sizeof(AetObj_vtbl) == 0x108, "\"AetObj_vtbl\" struct should have a size of 0x108");

struct AetObj {
    AetObj_vtbl* __vftable;
    aet_scene* scene;
    aet_comp* comp;
    aet_layer* layer;
    float_t loop_start_time;
    float_t loop_end_time;
    AetFlags flags;
    resolution_mode src_mode;
    aet_info info;
    float_t start_time;
    float_t end_time;
    int32_t index;
    int32_t layer_index;
    spr::SprPrio prio;
    vec3 pos;
    vec3 rot;
    vec3 scale;
    vec3 anchor;
    float_t frame_speed;
    vec4 color;
    float_t frame;
    resolution_mode dst_mode;
    vec2 scale_size;
    bool matte;
    spr::SprArgs spr_args;
    float_t opacity;
    prj::map<prj::string, int32_t> layer_sprite;
    prj::string sound_path;
    prj::map<prj::string, prj::string> sound_replace;
    int32_t sound_queue_index;
    prj::map<uint32_t, uint32_t> sprite_replace;
    prj::map<uint32_t, texture*> sprite_texture;
    prj::map<uint32_t, uint32_t> sprite_discard;
    FrameRateControl* frame_rate_control;
    bool set_viewport;
    rectangle viewport;
    bool sound_voice;
    bool use_float;
};

static_assert(sizeof(AetObj) == 0x2A0, "\"AetObj\" struct should have a size of 0x2A0");

class AetMgr : public app::Task {
public:
    prj::map<int32_t, AetSet> sets;
    prj::map<uint32_t, AetObj> objects;
    prj::list<prj::map<uint32_t, AetObj>::iterator> free_objects;
    uint32_t load_counter;
};

static_assert(sizeof(AetMgr) == 0xA0, "\"AetMgr\" struct should have a size of 0xA0");

uint32_t(FASTCALL* aet_manager_init_aet_object)(AetArgs& args, uint32_t id)
    = (uint32_t(FASTCALL*)(AetArgs & args, uint32_t id))0x00000001402CA220;
void (FASTCALL* aet_manager_free_aet_object)(uint32_t id)
    = (void (FASTCALL*)(uint32_t id))0x00000001402CA320;
void (FASTCALL* aet_manager_read_file)(int32_t set_id, const prj::string& mdata_dir)
    = (void (FASTCALL*)(int32_t set_id, const prj::string & mdata_dir))0x00000001402C9FA0;
void (FASTCALL* aet_manager_unload_set)(int32_t set_id)
    = (void (FASTCALL*)(int32_t set_id))0x0000001402CA040;
bool (FASTCALL* aet_manager_load_file)(int32_t set_id)
    = (bool (FASTCALL*)(int32_t set_id))0x00000001402CA020;
void (FASTCALL* aet_manager_set_obj_play)(uint32_t id, bool value)
    = (void (FASTCALL*)(uint32_t id, bool value))0x00000001402CA380;
void (FASTCALL* aet_manager_set_obj_alpha)(uint32_t id, float_t value)
    = (void (FASTCALL*)(uint32_t id, float_t value))0x00000001402CA420;
void (FASTCALL* aet_manager_set_obj_frame)(uint32_t id, float_t value)
    = (void (FASTCALL*)(uint32_t id, float_t value))0x00000001402CA4B0;
bool (FASTCALL* aet_manager_set_obj_frame_rate_control)(uint32_t id, FrameRateControl* value)
    = (bool (FASTCALL*)(uint32_t id, FrameRateControl * value))0x00000001402CA4D0;
void (FASTCALL* aet_manager_set_obj_visible)(uint32_t id, bool value)
    = (void (FASTCALL*)(uint32_t id, bool value))0x0000001402CA4F0;
float_t(FASTCALL* aet_manager_get_obj_frame)(uint32_t id)
    = (float_t(FASTCALL*)(uint32_t id))0x00000001402CA500;
bool (FASTCALL* aet_manager_get_obj_end)(uint32_t id)
    = (bool (FASTCALL*)(uint32_t id))0x00000001402CA510;
void (FASTCALL* aet_manager_init_aet_layout_0)(AetComp* comp, int32_t aet_id, const char* layer_name,
    AetFlags flags, resolution_mode mode, const char* start_marker, float_t start_time)
    = (void (FASTCALL*)(AetComp * comp, int32_t aet_id, const char* layer_name,
        AetFlags flags, resolution_mode mode, const char* start_marker, float_t start_time))0x00000001402CA590;

AetMgr& aet_manager = *(AetMgr*)0x00000001414AB3D0;

AetArgs::AetArgs() : layer_name(), start_marker(), end_marker(),
flags(), index(), layer(), frame_rate_control(), sound_voice() {
    start_time = -1.0f;
    end_time = -1.0f;
    prio = spr::SPR_PRIO_DEFAULT;
    mode = RESOLUTION_MODE_HD;
    scale = 1.0f;
    frame_speed = 1.0f;
    color = 1.0f;
    sound_queue_index = 1;
}

AetArgs::~AetArgs() {

}

aet_layout_data::aet_layout_data() : width(), height() {
    mat = mat4_identity;
    opacity = 1.0f;
    color = 0xFFFFFFFF;
    mode = RESOLUTION_MODE_HD;
    sprite_index = -1;
}

void aet_layout_data::put_sprite(int32_t spr_id, spr::SprAttr attr,
    spr::SprPrio prio, const vec2* pos, const aet_layout_data* layout) {
    if (!layout)
        return;

    spr::SprArgs args;
    aet_layout_data::set_args(layout, &args);
    args.id.id = spr_id;
    args.attr = attr;
    args.prio = prio;
    if (pos)
        *(vec2*)&args.trans.x += *pos;
    spr::put_sprite(args);
}

void aet_layout_data::set_args(const aet_layout_data* layout, spr::SprArgs* args) {
    if (!layout || !args)
        return;

    args->trans = layout->position;
    args->center = layout->anchor;
    args->scale.x = layout->mat.row0.x;
    args->scale.y = layout->mat.row1.y;
    args->scale.z = layout->mat.row2.z;
}

AetComp::AetComp() {

}

AetComp::~AetComp() {

}

void AetComp::Add(const char* name, const aet_layout_data& data) {
    this->data.insert({ name, data });
}

void AetComp::Clear() {
    data.clear();
}

const aet_layout_data* AetComp::Find(const char* name) {
    auto elem = data.find(name);
    if (elem != data.end())
        return &elem->second;
    return 0;
}

void AetComp::put_number_sprite(int32_t value, int32_t max_digits,
    AetComp* comp, const char** names, const int32_t* spr_ids,
    spr::SprPrio prio, const vec2* pos, bool all_digits) {
    if (!comp || !names || !spr_ids)
        return;

    for (int32_t i = 0; i < max_digits; i++) {
        int32_t digit = value % 10;
        value /= 10;

        const aet_layout_data* layout = comp->Find(names[i]);
        if (layout)
            aet_layout_data::put_sprite(spr_ids[digit],
                spr::SPR_ATTR_CTR_CC, prio, pos, layout);

        if (!all_digits && !value)
            break;
    }
}

int32_t aet_database_get_aet_id_by_name(const prj::string& name) {
    struct aet_db_aet {
        int32_t id;
        prj::string_range name;
        aet_info info;
    };

    static_assert(sizeof(aet_db_aet) == 0x20, "\"aet_db_aet\" struct should have a size of 0x20");

    const aet_db_aet* (FASTCALL * aet_database_struct__get_aet_by_name)
        (size_t This, const prj::string_range & name)
        = (const aet_db_aet * (FASTCALL*)(size_t This, const prj::string_range & name))0x00000001518B8120;
    return aet_database_struct__get_aet_by_name(0, prj::string_range(*(prj::string*)&name))->id;
}

int32_t aet_database_get_aet_set_id_by_name(const prj::string& name) {
    struct aet_db_aet_set {
        uint32_t id;
        prj::string_range name;
        prj::string_range file_name;
        uint32_t index;
        uint32_t sprite_set_id;
    };

    static_assert(sizeof(aet_db_aet_set) == 0x30, "\"aet_db_aet_set\" struct should have a size of 0x30");

    const aet_db_aet_set* (FASTCALL* aet_database_struct__get_aet_set_by_name)
        (size_t This, const prj::string_range& name)
        = (const aet_db_aet_set * (FASTCALL*)(size_t This, const prj::string_range & name))0x000000001518B7BE0;
    return aet_database_struct__get_aet_set_by_name(0, prj::string_range(*(prj::string*)&name))->id;
}
