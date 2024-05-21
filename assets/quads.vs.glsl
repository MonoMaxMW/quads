#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 scale;
layout(location = 2) in vec4 color;

out Data {
    vec4 color;
} passdata;

void main() {
    const vec2[] data = vec2[](
        vec2(+1.0, +1.0),
        vec2(-1.0, +1.0),
        vec2(-1.0, -1.0),
        vec2(+1.0, +1.0),
        vec2(-1.0, -1.0),
        vec2(+1.0, -1.0)
    );

    vec2 p = data[gl_VertexID];
    p *= scale;
    p += pos;
    passdata.color = color;
    gl_Position = vec4(p, 0.0, 1.0);
}