#version 330 core

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 InputUV;

out vec2 UV;

uniform float ScreenWidth;
uniform float ScreenHeight;

uniform float XOff;
uniform float YOff;

void main(){
	vec2 VertexPosition = Position - vec2(ScreenWidth,ScreenHeight);
	VertexPosition += vec2(XOff,YOff);
	VertexPosition /= vec2(ScreenWidth,ScreenHeight);
	
	gl_Position =  vec4(VertexPosition,0,1);

	UV = InputUV;
}

