#version 460 core
#extension GL_ABC_foobar_feature : enable
#extension GL_ARB_gpu_shader_fp64 : enable
layout (location = 0) in vec3 aPos;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}