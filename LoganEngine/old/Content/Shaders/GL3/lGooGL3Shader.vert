#version 330

layout(location = 0) in vec2 lGooPos_v;
layout(location = 1) in vec2 lGooTexCoord_v;

uniform mat3 lGooModelMatrix;
uniform mat3 lGooProjectionMatrix;

out vec2 lGooTexCoord_f;

void main()
{
    gl_Position = vec4(lGooProjectionMatrix *  lGooModelMatrix * vec3(lGooPos_v,1.0),1.0);

    lGooTexCoord_f = lGooTexCoord_v.xy;
}
