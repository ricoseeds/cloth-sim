#version 330 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
//layout (location = 2) in vec2 texCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

out vec3 FragPos;
out vec3 Normal;
//out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(pos, 1.0f));			// vertex position in world space
    Normal = mat3(transpose(inverse(model))) * normal;	// normal direction in world space

	//TexCoord = texCoord;

	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}