#prop Cull on front
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

uniform mat4 m2w;
layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

vec4 MVP_ish(in vec4 position) {
	mat4 viewRotation = view;

	//cut away the view translation
	viewRotation[3][0] = 0;
	viewRotation[3][1] = 0;
	viewRotation[3][2] = 0;
    return projection * viewRotation * m2w * position;
}

out vec2 texCoords;
void main() {
	texCoords = uv;
    gl_Position = MVP_ish(position);
}

#shader fragment
#version 410

in vec2 texCoords;

out vec4 color;
uniform sampler2D skyTexture;
uniform float time = 0;
uniform float intensity = 1;

void main() {
    color = vec4(texture(skyTexture, texCoords).rgb*intensity, 1);
}
