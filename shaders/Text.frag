#version 330 core

in vec2 UV;

out vec4 FinalColor;

uniform vec4 Color;
uniform sampler2D TextureSampler;
void main(){

	FinalColor = Color*texture2D(TextureSampler,UV);

}
