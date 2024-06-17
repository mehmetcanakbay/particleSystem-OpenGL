#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 offsetLifetime;

out vec4 m_Color;
out float m_Lifetime;

void main() {
	m_Color = vec4(1.0);
	m_Lifetime = offsetLifetime.w;
	gl_Position = vec4(position.x*0.005, position.y*0.005, position.z, 1.0)+vec4(offsetLifetime.x, offsetLifetime.y, offsetLifetime.z, 0.0);
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec4 m_Color;
in float m_Lifetime;

void main(){

	color = vec4(m_Color.r,m_Color.g, m_Color.b, max(m_Lifetime, 0.0));
};
