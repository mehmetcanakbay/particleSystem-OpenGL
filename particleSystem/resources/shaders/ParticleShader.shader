#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

out vec4 m_Color;

void main(){
	m_Color = color;
	gl_Position = position;
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec4 m_Color;

void main(){
	color = m_Color;
};
