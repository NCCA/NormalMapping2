#version 330 core
// based on http://apoorvaj.io/exploring-bump-mapping-with-webgl.html
layout(location =0) out vec4 FragColour;
uniform sampler2D normalSampler; // 0
uniform sampler2D diffuseSampler; // 1
uniform sampler2D tex_depth; // 2
/*
    The type is controlled by the radio buttons below the canvas.
    0 = No bump mapping
    1 = Normal mapping
    2 = Parallax mapping
    3 = Steep parallax mapping
    4 = Parallax occlusion mapping
*/
uniform int type;
uniform int show_tex;
uniform float depth_scale;
uniform float num_layers;

in vec2 frag_uv;
in vec3 ts_light_pos;
in vec3 ts_view_pos;
in vec3 ts_frag_pos;

vec2 parallax_uv(vec2 uv, vec3 view_dir)
{
    if (type == 2)
    {
        // Parallax mapping
        float depth = texture(tex_depth, uv).r;
        vec2 p = view_dir.xy * (depth * depth_scale) / view_dir.z;
        return uv - p;
    }
    else
    {
        float layer_depth = 1.0 / num_layers;
        float cur_layer_depth = 0.0;
        vec2 delta_uv = view_dir.xy * depth_scale / (view_dir.z * num_layers);
        vec2 cur_uv = uv;

        float depth_from_tex = texture(tex_depth, cur_uv).r;

        for (int i = 0; i < 32; i++)
        {
            cur_layer_depth += layer_depth;
            cur_uv -= delta_uv;
            depth_from_tex = texture(tex_depth, cur_uv).r;
            if (depth_from_tex < cur_layer_depth)
            {
                break;
            }
        }

        if (type == 3)
        {
            // Steep parallax mapping
            return cur_uv;
        }
        else
        {
            // Parallax occlusion mapping
            vec2 prev_uv = cur_uv + delta_uv;
            float next = depth_from_tex - cur_layer_depth;
            float prev = texture(tex_depth, prev_uv).r - cur_layer_depth + layer_depth;
            float weight = next / (next - prev);
            return mix(cur_uv, prev_uv, weight);
        }
    }
}

void main()
{
    vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
    vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);

    // Only perturb the texture coordinates if a parallax technique is selected
    vec2 uv = (type < 2) ? frag_uv : parallax_uv(frag_uv, view_dir);

    vec3 albedo = texture(diffuseSampler, uv).rgb;
    if (show_tex == 0) { albedo = vec3(1,1,1); }
    vec3 ambient = 0.3 * albedo;

    if (type == 0)
    {
        // No bump mapping
        vec3 norm = vec3(0,0,1);
        float diffuse = max(dot(light_dir, norm), 0.0);
        FragColour = vec4(diffuse * albedo + ambient, 1.0);

    }
    else
    {
        // Normal mapping
        vec3 norm = normalize(texture(normalSampler, uv).rgb * 2.0 - 1.0);
        float diffuse = max(dot(light_dir, norm), 0.0);
        FragColour = vec4(diffuse * albedo + ambient, 1.0);
    }
}
