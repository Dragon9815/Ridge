#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 frag_vertexColor;

void main() 
{
	gl_Position = vec4(position, 1.0);
	frag_vertexColor = color;
}


#shader fragment
#version 330 core

layout(location=0) out vec4 fragColor;

in vec3 frag_vertexColor;

void main() {
	fragColor = vec4(frag_vertexColor, 1.0);
}