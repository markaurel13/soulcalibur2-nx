#version 460

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_uv0;
layout(location = 4) in vec2 in_uv1;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_spec_fog;
layout(location = 2) out vec4 out_uv01;
layout(location = 3) out vec4 out_uv23;

layout(std140, binding = 0) uniform VtxBlock {
    vec4 u_wvp[4];
    vec4 u_norm_mat[4];
    vec4 u_world_mat[4];
    vec4 u_light_dir[8];
    vec4 u_light_pos[8];
    vec4 u_light_color[8];
    vec4 u_light_ambient[8];
    vec4 u_light_specular[8];
    vec4 u_light_atten[8];
    vec4 u_global_ambient;
    vec4 u_mat_diffuse;
    vec4 u_mat_ambient;
    vec4 u_mat_emissive;
    vec4 u_light_view;
    uvec4 u_light_ctrl; // x=num_lights, y=flags, z=sources, w=zero_diffuse_rgb
    vec4 u_fog_params;  // x=has_fog, y=mode, z=start, w=end
    vec4 u_fog_planes[4];
    vec4 u_mv_mat[4];
    vec4 u_tex_mat0[4];
    vec4 u_tex_mat1[4];
    uvec4 u_tex_ctrl;   // x=stage0, y=stage1, z=normalize_normals, w=0
};

vec2 evalTexStage(uint ctrl, vec4 m0, vec4 m1, vec4 m2, vec4 m3, vec3 cam_n, bool have_cam_n) {
    uint uv_src = ctrl & 3u;
    bool has_mat = (ctrl & 4u) != 0u;
    uint gen_kind = (ctrl >> 4u) & 3u;

    if (gen_kind == 0u) {
        vec2 uv = (uv_src == 1u) ? in_uv1 : in_uv0;
        if (!has_mat) return uv;
        return vec2(
            uv.x * m0.x + uv.y * m1.x + m2.x,
            uv.x * m0.y + uv.y * m1.y + m2.y
        );
    }

    vec3 g = vec3(0.0);
    if (gen_kind == 1u) {
        g = cam_n;
    } else {
        vec3 cam_p = in_pos.x * u_mv_mat[0].xyz + in_pos.y * u_mv_mat[1].xyz + in_pos.z * u_mv_mat[2].xyz + u_mv_mat[3].xyz;
        if (gen_kind == 2u) {
            g = cam_p;
        } else {
            float plen = length(cam_p);
            vec3 ray = (plen > 1e-8) ? (cam_p / plen) : vec3(0.0);
            g = have_cam_n ? (ray - 2.0 * dot(ray, cam_n) * cam_n) : ray;
        }
    }

    return vec2(
        g.x * m0.x + g.y * m1.x + g.z * m2.x + m3.x,
        g.x * m0.y + g.y * m1.y + g.z * m2.y + m3.y
    );
}

void main() {
    gl_Position = in_pos.x * u_wvp[0] + in_pos.y * u_wvp[1] + in_pos.z * u_wvp[2] + u_wvp[3];

    vec4 col = in_color;
    vec3 spec = vec3(0.0);

    bool lt_enabled = (u_light_ctrl.y & 1u) != 0u;
    bool has_normal = (u_light_ctrl.y & 2u) != 0u;
    bool lt_norm = (u_light_ctrl.y & 4u) != 0u;
    bool local_viewer = (u_light_ctrl.y & 8u) != 0u;

    vec3 transformed_n = vec3(0.0);
    bool have_transformed_n = false;
    if (has_normal && (lt_enabled || ((u_tex_ctrl.x >> 4u) & 3u) != 0u || ((u_tex_ctrl.y >> 4u) & 3u) != 0u)) {
        transformed_n = in_norm.x * u_norm_mat[0].xyz + in_norm.y * u_norm_mat[1].xyz + in_norm.z * u_norm_mat[2].xyz;
        have_transformed_n = true;
    }

    if (lt_enabled && has_normal) {
        vec3 n = transformed_n;
        if (lt_norm) {
            float l2 = dot(n, n);
            if (l2 > 1e-8) n *= inversesqrt(l2);
        }

        vec3 wp = in_pos.x * u_world_mat[0].xyz + in_pos.y * u_world_mat[1].xyz + in_pos.z * u_world_mat[2].xyz + u_world_mat[3].xyz;
        vec4 md = ((u_light_ctrl.z & 1u) != 0u) ? in_color : u_mat_diffuse;
        vec3 ma = ((u_light_ctrl.z & 2u) != 0u) ? in_color.rgb : u_mat_ambient.rgb;
        vec3 me = ((u_light_ctrl.z & 4u) != 0u) ? in_color.rgb : u_mat_emissive.rgb;

        vec3 lit = me + u_global_ambient.rgb * ma;
        vec3 v = u_light_view.xyz;
        if (local_viewer) {
            v = v - wp;
            float vl2 = dot(v, v);
            v = (vl2 > 1e-8) ? (v * inversesqrt(vl2)) : vec3(0.0);
        }

        uint num_lights = u_light_ctrl.x;
        for (uint li = 0u; li < num_lights; ++li) {
            vec3 ldir = u_light_dir[li].xyz;
            float atten = 1.0;
            if (u_light_pos[li].w > 0.5) {
                vec3 delta = u_light_pos[li].xyz - wp;
                float d = length(delta);
                float denom = u_light_atten[li].x + u_light_atten[li].y * d + u_light_atten[li].z * d * d;
                atten = (d <= u_light_atten[li].w && denom > 0.0) ? (1.0 / denom) : 0.0;
                ldir = (d > 1e-8) ? (delta / d) : vec3(0.0);
            }
            float lambert = dot(n, ldir);
            float pos_lambert = max(0.0, lambert);
            lit += (md.rgb * u_light_color[li].rgb * pos_lambert + u_light_ambient[li].rgb * ma) * atten;

            if (u_light_view.w > 0.0 && lambert > 0.0) {
                vec3 h = ldir + v;
                float hl2 = dot(h, h);
                if (hl2 > 1e-8) h *= inversesqrt(hl2);
                float ndoth = max(1e-6, dot(n, h));
                float sp = atten * pow(ndoth, u_light_view.w);
                spec += u_light_specular[li].rgb * sp;
            }
        }
        col = vec4(clamp(lit, 0.0, 1.0), md.a);
        spec = clamp(spec, 0.0, 1.0);
    }

    if (u_light_ctrl.w != 0u) {
        col.rgb = vec3(0.0);
    }

    float fog_f = 1.0;
    if (u_fog_params.x > 0.5) {
        float d;
        if (u_fog_planes[3].x > 0.5) {
            float ex = dot(vec4(in_pos, 1.0), u_fog_planes[1]);
            float ey = dot(vec4(in_pos, 1.0), u_fog_planes[2]);
            float ez = dot(vec4(in_pos, 1.0), u_fog_planes[0]);
            d = sqrt(ex * ex + ey * ey + ez * ez);
        } else {
            d = dot(vec4(in_pos, 1.0), u_fog_planes[0]);
        }
        float mode = u_fog_params.y;
        if (mode > 2.5) {
            float rng = u_fog_params.w - u_fog_params.z;
            fog_f = (rng != 0.0) ? ((u_fog_params.w - d) / rng) : ((d < u_fog_params.w) ? 1.0 : 0.0);
        } else if (mode > 1.5) {
            float e = u_fog_planes[3].y * d;
            fog_f = exp(-e * e);
        } else if (mode > 0.5) {
            fog_f = exp(-u_fog_planes[3].y * d);
        }
        fog_f = clamp(fog_f, 0.0, 1.0);
    }

    vec3 cam_n = transformed_n;
    if (have_transformed_n && u_tex_ctrl.z != 0u) {
        float nl2 = dot(cam_n, cam_n);
        if (nl2 > 1e-8) cam_n *= inversesqrt(nl2);
    }

    vec2 uv0 = evalTexStage(u_tex_ctrl.x, u_tex_mat0[0], u_tex_mat0[1], u_tex_mat0[2], u_tex_mat0[3], cam_n, have_transformed_n);
    vec2 uv1 = evalTexStage(u_tex_ctrl.y, u_tex_mat1[0], u_tex_mat1[1], u_tex_mat1[2], u_tex_mat1[3], cam_n, have_transformed_n);

    out_color = col;
    out_spec_fog = vec4(spec, fog_f);
    out_uv01 = vec4(uv0, uv1);
    out_uv23 = vec4(0.0);
}
