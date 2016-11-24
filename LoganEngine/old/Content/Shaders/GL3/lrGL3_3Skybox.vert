#version 330

uniform mat4 lr_ModelMatrix;
uniform mat4 lr_ViewMatrix;
uniform mat4 lr_ProjectionMatrix;
uniform mat4 lr_ModelViewMatrix;
uniform mat4 lr_ModelViewProjectionMatrix;
uniform mat3 lr_NormalMatrix;

layout(location = 0) in vec3 lr_vPosition;

out vec3 lr_fPosition;

void main()
{
    lr_fPosition = vec3(lr_vPosition);

    gl_Position = lr_ProjectionMatrix * vec4((lr_ViewMatrix * vec4(10.0 * lr_vPosition,0.0)).xyz,1.0);
}
