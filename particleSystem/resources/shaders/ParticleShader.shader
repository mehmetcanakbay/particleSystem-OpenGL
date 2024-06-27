#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 offsetLifetime;

out float m_Lifetime;
out vec2 m_screenPos;

void main() {
	m_Lifetime = offsetLifetime.w;
	gl_Position = vec4(position.x*0.005, position.y*0.005, position.z, 1.0)+vec4(offsetLifetime.x, offsetLifetime.y, offsetLifetime.z, 0.0);
	m_screenPos = vec2(gl_Position.x, gl_Position.y);
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in float m_Lifetime;
in vec2 m_screenPos;

float nrand(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
	float randVal = nrand(m_screenPos);
	color = vec4(randVal, 0.6, 1.0, max(m_Lifetime, 0.0));
};
