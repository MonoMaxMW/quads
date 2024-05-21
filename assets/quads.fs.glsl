#version 330

layout(location = 0) out vec4 fragcolor;

in Data {
    vec4 color;
} passdata;

void main() {
    fragcolor = passdata.color;
}