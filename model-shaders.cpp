#include "shaders.hpp"

const char *MODEL_VERT = SHADER(
attribute vec3 vertex;
attribute vec3 texCoord;
attribute vec3 normal;

uniform float time;
uniform float rotation;
uniform vec2  size;

uniform vec3  position;
uniform vec3  yawPitchRoll;

varying vec3 tex;
varying vec3 norm;

mat3 rotationX(float theta);
mat3 rotationY(float theta);
mat3 rotationZ(float theta);

void main()
{
    float warp = size.x / size.y;

    mat4 frustum = mat4(1.0,  0.0,   0.0,     0.0,
                        0.0,  warp,   0.0,     0.0,
                        0.0,  0.0, 2.0/100.0, 1.0,
                        0.0,  0.0,   0.0,     1.0);

    mat3 animation = rotationZ(0.3 * sin(vertex.x / 120.0 + time / 2.0));
    vec3 v = animation * vertex;

    mat3 ypr = rotationZ(yawPitchRoll.x) *
               rotationY(yawPitchRoll.y) *
               rotationX(yawPitchRoll.z);

    v = (ypr * v / 3600.0) + position;

    mat3 globalRotation = rotationZ(rotation);

    v = 10.0 * globalRotation * v;

    gl_Position = frustum * vec4(v.xzy + vec3(0.0, 0.0, 30.0), 1.0);

    tex  = texCoord;
    norm = (globalRotation * ypr * animation * normal).xzy;
}

mat3 rotationX(float theta)
{
    return mat3(1.0,    0.0,         0.0,
                0.0, cos(theta),  -sin(theta),
                0.0, sin(theta),  cos(theta));
}

mat3 rotationY(float theta)
{
    return mat3(cos(theta), 0.0, -sin(theta),
                     0.0,   1.0,   0.0,
                sin(theta), 0.0, cos(theta));
}

mat3 rotationZ(float theta)
{
    return mat3( cos(theta), sin(theta), 0.0,
                -sin(theta), cos(theta), 0.0,
                     0.0,      0.0,      1.0);
}
);

const char *MODEL_FRAG = SHADER(
varying vec3 tex;
varying vec3 norm;

uniform sampler2D texSampler;

void main()
{
    float a = dot(normalize(norm), normalize(vec3(3.0, -3.0, -3.0)));

    if (a < 0.1)
        a = 0.1;

    gl_FragColor = vec4(a * texture(texSampler, tex.xy).rgb, 1.0);
}
);
