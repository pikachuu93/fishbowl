#include "shaders.hpp"

const char *PRIMATIVE_VERT = SHADER(
attribute vec3 vertex;

uniform float rotation;
uniform vec2  size;
uniform vec3  color;

mat3 rotationZ(float theta);

varying vec3 c;

void main()
{
    float warp = size.x / size.y;

    mat4 frustum = mat4(1.0,  0.0,   0.0,     0.0,
                        0.0,  warp,   0.0,     0.0,
                        0.0,  0.0, 2.0/100.0, 1.0,
                        0.0,  0.0,   0.0,     1.0);

    mat3 globalRotation = rotationZ(rotation);

    vec3 v = 10.0 * globalRotation * vertex;

    c = color;
    gl_Position   = frustum * vec4(v.xzy + vec3(0.0, 0.0, 30.0), 1.0);
}

mat3 rotationZ(float theta)
{
    return mat3( cos(theta), sin(theta), 0.0,
                -sin(theta), cos(theta), 0.0,
                     0.0,     0.0,      1.0);
}
);

const char *PRIMATIVE_FRAG = SHADER(
varying vec3 c;
void main()
{
    gl_FragColor = vec4(c, 1.0);
}
);
