#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 offset;
layout(location=2) in vec4 vertexColor;

out vec4 m_vertexColor;

void main() {
	m_vertexColor = vertexColor;
	gl_Position = vec4(position.x*0.005, position.y*0.005, position.z, 1.0)+vec4(offset.x, offset.y, offset.z, 0.0);
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec4 m_vertexColor;

float nrand(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
	// color = vec4(1.0);
	color = m_vertexColor;
};
