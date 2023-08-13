#version 460 core
#extension GL_ABC_foobar_feature : enable
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 FragColor;
void main(){
    FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
}