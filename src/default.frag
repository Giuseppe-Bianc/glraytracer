#version 460 core
#extension GL_ABC_foobar_feature : enable
#extension GL_ARB_gpu_shader_fp64 : enable
out vec4 FragColor;
// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;


void main() {
	FragColor = texture(tex0, texCoord);
}