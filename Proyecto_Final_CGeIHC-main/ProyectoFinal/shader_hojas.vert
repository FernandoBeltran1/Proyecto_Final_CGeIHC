#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 normal;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform float u_time;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float seed  = rand(vec2(floor(pos.x * 2.0), floor(pos.z * 2.0)));
    float freq  = 0.8 + seed * 1.2;
    float ampX  = 0.05 + seed * 0.08;
    float ampY  = 0.03 + seed * 0.05;
    float ampZ  = 0.04 + seed * 0.06;
    float phase = seed * 6.2831;

    vec3 animPos = pos;
    animPos.x += sin(u_time * freq + phase) * ampX;
    animPos.y += sin(u_time * freq * 1.3 + phase) * ampY;
    animPos.z += cos(u_time * freq * 0.9 + phase) * ampZ;

    gl_Position = projection * view * model * vec4(animPos, 1.0);
    vCol        = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
    TexCoord    = tex;
    Normal      = mat3(transpose(inverse(model))) * normal;
    FragPos     = (model * vec4(animPos, 1.0)).xyz;
}